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

//
// extern includes
//
#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include <Common/Base/hkBase.h>
#include <Common/Base/System/hkBaseSystem.h>
#include <Common/Base/Memory/System/Util/hkMemoryInitUtil.h>
#include <Common/Base/Memory/Allocator/Malloc/hkMallocAllocator.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

#include <Physics/Dynamics/World/hkpWorld.h>
#include <Physics/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics/Utilities/Destruction/BreakOffParts/hkpBreakOffPartsUtil.h>

// Keycode
#include <Common/Base/keycode.cxx>

// This excludes libraries that are not going to be linked
// from the project configuration, even if the keycodes are
// present

#undef HK_FEATURE_PRODUCT_AI
#undef HK_FEATURE_PRODUCT_ANIMATION
#undef HK_FEATURE_PRODUCT_CLOTH
#undef HK_FEATURE_PRODUCT_DESTRUCTION
#undef HK_FEATURE_PRODUCT_BEHAVIOR
#define HK_FEATURE_REFLECTION_PHYSICS
#define HK_CLASSES_FILE <Common/Serialize/Classlist/hkClasses.h>
#define HK_EXCLUDE_FEATURE_MemoryTracker
#define HK_EXCLUDE_FEATURE_SerializeDeprecatedPre700
#define HK_EXCLUDE_FEATURE_RegisterVersionPatches
#define HK_EXCLUDE_LIBRARY_hkGeometryUtilities
#include <Common/Base/Config/hkProductFeatures.cxx>


#pragma warning( pop )

//
// core includes
//
#include "BaseTypes.h"
#include "Interface.h"

//
// system includes
//
#include "Collision.h"
#include "System.h"
#include "Scene.h"


extern ManagerInterfaces    g_Managers;


u32 HavokPhysicsSystem::s_idMainThread = 0;
tbb::atomic<u32> HavokPhysicsSystem::s_threadNumberCount;
WorkerMemoryRouterMap_t HavokPhysicsSystem::s_workerMemoryRouterMap;


///////////////////////////////////////////////////////////////////////////////
// HavokPhysicsSystem - Constructor
HavokPhysicsSystem::HavokPhysicsSystem(
    void
)
    : ISystem() {
}


///////////////////////////////////////////////////////////////////////////////
// ~HavokPhysicsSystem - Destructor
HavokPhysicsSystem::~HavokPhysicsSystem(
    void
) {
    if (m_bInitialized) {
        hkBaseSystem::quit();
        hkMemoryInitUtil::quit();
    }
}


///////////////////////////////////////////////////////////////////////////////
// GetSystemType - Returns System type for this System
System::Type HavokPhysicsSystem::GetSystemType(void) {
    return System::Types::Physic;
}


///////////////////////////////////////////////////////////////////////////////
// Initialize - Initializes this System with the given properties
Error
HavokPhysicsSystem::Initialize(
    Properties::Array Properties
) {
    ASSERT(!m_bInitialized);
    //
    // Save the main thread id
    //
    s_idMainThread = ::GetCurrentThreadId();
    //
    // Base Havok init
    //
    hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault(hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo(1024 * 1024));
    hkBaseSystem::init(memoryRouter, ErrorReport, this);
    //
    // Init each worker threads.
    //
    g_Managers.pTask->NonStandardPerThreadCallback(
        reinterpret_cast<ITaskManager::JobFunction>(AllocateThreadResources), this
    );
    //
    // Create the job queue
    //
    hkJobQueueCinfo info;
    info.m_jobQueueHwSetup.m_numCpuThreads = g_Managers.pTask->GetRecommendedJobCount();
    m_jobQueue = new hkJobQueue(info);
    //
    // Hide some errors.
    //
    hkError::getInstance().setEnabled(0x2a2cde91, false);
    hkError::getInstance().setEnabled(0x6e8d163b, false);
    hkError::getInstance().setEnabled(0xad345a23, false);
    hkError::getInstance().setEnabled(0x11fce585, false);
    hkError::getInstance().setEnabled(0x34df5494, false);
#ifdef __HAVOK_VDB__
    hkError::getInstance().setEnabled(0x1293ADE8, false);
    hkError::getInstance().setEnabled(0x1293ADEF, false);
#endif
    m_bInitialized = true;
    return Errors::Success;
}


///////////////////////////////////////////////////////////////////////////////
// GetProperties - Properties for this System are returned in Properties
void
HavokPhysicsSystem::GetProperties(
    Properties::Array& Properties
) {
    UNREFERENCED_PARAM(Properties);
}


///////////////////////////////////////////////////////////////////////////////
// SetProperties - Set properties for this System
void
HavokPhysicsSystem::SetProperties(
    Properties::Array Properties
) {
    UNREFERENCED_PARAM(Properties);
    ASSERT(m_bInitialized);
}

///////////////////////////////////////////////////////////////////////////////
// CreateScene - Creates and returns a new Scene
ISystemScene*
HavokPhysicsSystem::CreateScene(
    void
) {
    return new HavokPhysicsScene(this);
}


///////////////////////////////////////////////////////////////////////////////
// DestroyScene - Destroys the given Scene, free all associated resources
Error
HavokPhysicsSystem::DestroyScene(
    ISystemScene* pSystemScene
) {
    ASSERT(pSystemScene != NULL);
    HavokPhysicsScene* pScene = reinterpret_cast<HavokPhysicsScene*>(pSystemScene);
    SAFE_DELETE(pScene);
    return Errors::Success;
}


///////////////////////////////////////////////////////////////////////////////
// AllocateThreadResources - Allocate resources need to support multiple threads
void
HavokPhysicsSystem::AllocateThreadResources(
    HavokPhysicsSystem* pSystem
) {
    //
    // Do not initialize main thread.
    // Already done in mainInit
    //
    u32 currentThreadId = ::GetCurrentThreadId();

    if (currentThreadId == s_idMainThread) {
        pSystem->s_threadNumberCount.fetch_and_increment();
        return;
    }

    HK_THREAD_LOCAL_SET(hkThreadNumber, pSystem->s_threadNumberCount.fetch_and_increment());
    //
    // Create thread memory for the thread.
    //
    hkMemoryRouter* memoryRouter = new(malloc(sizeof(hkMemoryRouter))) hkMemoryRouter();
    hkMemorySystem::getInstance().threadInit(*memoryRouter, "PhysicSystemWorker");
    hkResult result = hkBaseSystem::initThread(memoryRouter);
    ASSERT(result == HK_SUCCESS);
    WorkerMemoryRouterMap_t::accessor a;
    s_workerMemoryRouterMap.insert(a, ::GetCurrentThreadId());
    a->second = memoryRouter;
    ASSERT(hkMemoryRouter::getInstancePtr() != NULL);
}


///////////////////////////////////////////////////////////////////////////////
// FreeThreadResources - Free resources allocated to support multiple threads
void
HavokPhysicsSystem::FreeThreadResources(
    HavokPhysicsSystem* pSystem
) {
    //
    // If the main thread serves as a worker of the scheduler's thread pool,
    // do not deinitialize havok's data here, 'cause doing so crashes hkBaseSystem::quit()
    //
    u32 currentThreadId = ::GetCurrentThreadId();

    if (currentThreadId == s_idMainThread)
    { return; }

    //
    // Free up Havok resources and free the stack space.
    //
    WorkerMemoryRouterMap_t::const_accessor a;

    if (s_workerMemoryRouterMap.find(a, ::GetCurrentThreadId())) {
        hkBaseSystem::quitThread();
        hkMemoryRouter* memoryRouter = a->second;
        hkMemorySystem::getInstance().threadQuit(*memoryRouter);
        SAFE_DELETE(memoryRouter);
    }
}


///////////////////////////////////////////////////////////////////////////////
// ErrorReport - Callback for Havok to report an error
void
HavokPhysicsSystem::ErrorReport(
    const char* pString,
    void* pErrorOutputObject
) {
    HavokPhysicsSystem* pSystem = reinterpret_cast<HavokPhysicsSystem*>(pErrorOutputObject);
    Debug::Print("[%s Error]: %s", pSystem->GetName(), pString);
    //ASSERTMSG( false, pString );
}
