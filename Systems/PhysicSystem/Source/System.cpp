// Copyright � 2008-2009 Intel Corporation
// All Rights Reserved
//
// Permission is granted to use, copy, distribute and prepare derivative works of this
// software for any purpose and without fee, provided, that the above copyright notice
// and this statement appear in all copies.  Intel makes no representations about the
// suitability of this software for any purpose.  THIS SOFTWARE IS PROVIDED "AS IS."
// INTEL SPECIFICALLY DISCLAIMS ALL WARRANTIES, EXPRESS OR IMPLIED, AND ALL LIABILITY,
// INCLUDING CONSEQUENTIAL AND OTHER INDIRECT DAMAGES, FOR THE USE OF THIS SOFTWARE,
// INCLUDING LIABILITY FOR INFRINGEMENT OF ANY PROPRIETARY RIGHTS, AND INCLUDING THE
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  Intel does not
// assume any responsibility for any errors which may appear in this software nor any
// responsibility to update it.

#include "System.h"

#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

#include <Physics2012/Dynamics/World/hkpWorld.h>
#include <Physics2012/Collide/Dispatch/hkpAgentRegisterUtil.h>

#include <Common/Base/keycode.cxx>

#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION_2012
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR
#undef HK_FEATURE_PRODUCT_PHYSICS
#define HK_FEATURE_REFLECTION_PHYSICS_2012
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#define HK_EXCLUDE_FEATURE_MemoryTracker
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches
#define HK_EXCLUDE_LIBRARY_hkGeometryUtilities
#include <Common/Base/Config/hkProductFeatures.cxx>
#pragma warning( pop )

#include <boost/functional/factory.hpp>
#include <boost/bind.hpp>

#include "Manager/ServiceManager.h"
#include "System.h"
#include "Scene.h"

extern IServiceManager* g_serviceManager;

u32 PhysicSystem::s_idMainThread = 0;
tbb::atomic<u32> PhysicSystem::s_threadNumberCount;
tbb::concurrent_hash_map<u32, hkMemoryRouter*> PhysicSystem::s_workerMemoryRouterMap;

/**
 * @inheritDoc
 */
PhysicSystem::PhysicSystem() : ISystem() {
    m_SceneFactory = boost::factory<PhysicScene*>();

    //m_propertySetters["Imageset"] = boost::bind(&GuiSystem::setImagesetResourceGroup, this, _1);
}

/**
 * @inheritDoc
 */
PhysicSystem::~PhysicSystem() {
    if (m_bInitialized) {
        hkBaseSystem::quit();
        hkMemoryInitUtil::quit();
    }
}

/**
 * @inheritDoc
 */
Error PhysicSystem::initialize() {
    ASSERT(!m_bInitialized);
    
    s_idMainThread = ::GetCurrentThreadId();
    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);

    hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault(hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo(1024 * 1024));
    hkBaseSystem::init(memoryRouter, ErrorReport, this);

    g_serviceManager->getTaskManager()->NonStandardPerThreadCallback(
        reinterpret_cast<ITaskManager::JobFunction>(AllocateThreadResources), this
    );

    hkJobQueueCinfo info;
    info.m_jobQueueHwSetup.m_numCpuThreads = g_serviceManager->getTaskManager()->GetRecommendedJobCount();
    m_jobQueue = new hkJobQueue(info);

    /*hkError::getInstance().setEnabled(0x6e8d163b, false);
    hkError::getInstance().setEnabled(0xad345a23, false);
    hkError::getInstance().setEnabled(0x11fce585, false);
    hkError::getInstance().setEnabled(0x34df5494, false);
    */
    hkError::getInstance().setEnabled(0x2a2cde91, false);
#ifdef HAVOK_VDB_ENABLED
    hkError::getInstance().setEnabled(0x1293ADE8, false);
    hkError::getInstance().setEnabled(0x1293ADEF, false);
#endif

    g_serviceManager->getLogService()->log(LOGOG_LEVEL_INFO, "System initialized");
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void PhysicSystem::AllocateThreadResources(PhysicSystem* pSystem) {
    //
    // Do not initialize main thread.
    // Already done in mainInit
    //
    u32 currentThreadId = ::GetCurrentThreadId();
    if (currentThreadId == s_idMainThread) {
        pSystem->s_threadNumberCount.fetch_and_increment();
        return;
    }

    _MM_SET_FLUSH_ZERO_MODE(_MM_FLUSH_ZERO_ON);
    HK_THREAD_LOCAL_SET(hkThreadNumber, pSystem->s_threadNumberCount.fetch_and_increment());

    hkMemoryRouter* memoryRouter = new(malloc(sizeof(hkMemoryRouter))) hkMemoryRouter();
    hkMemorySystem::getInstance().threadInit(*memoryRouter, "PhysicSystemWorker");
    hkResult result = hkBaseSystem::initThread(memoryRouter);
    ASSERT(result == HK_SUCCESS);

    tbb::concurrent_hash_map<u32, hkMemoryRouter*>::accessor a;
    s_workerMemoryRouterMap.insert(a, ::GetCurrentThreadId());
    a->second = memoryRouter;
    ASSERT(hkMemoryRouter::getInstancePtr() != NULL);
}

/**
 * @inheritDoc
 */
void PhysicSystem::FreeThreadResources(PhysicSystem* pSystem) {
    //
    // If the main thread serves as a worker of the scheduler's thread pool,
    // do not deinitialize havok's data here, 'cause doing so crashes hkBaseSystem::quit()
    //
    u32 currentThreadId = ::GetCurrentThreadId();

    if (currentThreadId == s_idMainThread) {
        return;
    }

    tbb::concurrent_hash_map<u32, hkMemoryRouter*>::const_accessor a;
    if (s_workerMemoryRouterMap.find(a, ::GetCurrentThreadId())) {
        hkBaseSystem::quitThread();
        hkMemoryRouter* memoryRouter = a->second;
        hkMemorySystem::getInstance().threadQuit(*memoryRouter);
        delete memoryRouter;
    }
}

/**
 * @inheritDoc
 */
void PhysicSystem::ErrorReport(const char* pString, void* pErrorOutputObject) {
    PhysicSystem* pSystem = reinterpret_cast<PhysicSystem*>(pErrorOutputObject);
    g_serviceManager->getLogService()->log(LOGOG_LEVEL_ERROR, pString);
    //ASSERTMSG1(false, "[PhysicSystem Error] %s", pString);
}
