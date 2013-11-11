// Copyright © 2008-2009 Intel Corporation
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

#include "Task.h"

#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include <Common\Base\hkBase.h>
#include <Common\Base\Thread\JobQueue\hkJobQueue.h>
#include <Common\Base\Memory\System\Util\hkMemoryInitUtil.h>
#include <Common\Base\Memory\Allocator\Malloc\hkMallocAllocator.h>
#include <Physics2012\Dynamics\World\hkpWorld.h>
#include <Physics2012\Dynamics\Entity\hkpRigidBody.h>
#include <Physics2012\Dynamics\Entity\hkpEntityListener.h>
#include <Physics2012\Dynamics\Entity\hkpEntityActivationListener.h>
#include <Physics2012\Dynamics\Phantom\hkpSimpleShapePhantom.h>
#include <Physics2012\Collide\Dispatch\hkpAgentRegisterUtil.h>
#include <Physics2012\Collide\Shape\Convex\Box\hkpBoxShape.h>
#include <Physics2012\Utilities\CharacterControl\CharacterProxy\hkpCharacterProxy.h>
#include <Physics2012\Utilities\Destruction\BreakOffParts\hkpBreakOffPartsUtil.h>
#include <Physics2012\Utilities\Dynamics\Inertia\hkpInertiaTensorComputer.h>

#ifdef __HAVOK_VDB__
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics2012/Utilities/VisualDebugger/hkpPhysicsContext.h>
#endif
#pragma warning( pop )

#include "Scene.h"
#include "System.h"
#include "Object/PhysicObject.h"
#include "Manager\IServiceManager.h"

extern IServiceManager* g_serviceManager;

// Grain sizes of the parallel jobs. The less is the grainsize the better is
// the load balance, and the higher is the parallel overhead.
static const u32    PhysicSystemTaskGrainSize = 8;

///
/// @inheritDoc
///
PhysicTask::PhysicTask(ISystemScene* pScene) 
    : ISystemTask(pScene) {
    m_jobQueue = GetSystemScene<PhysicScene>()->GetSystem<PhysicSystem>()->getJobQueue();

#ifdef __HAVOK_VDB__
    hkArray<hkProcessContext*> contexts;
    {
        // The visual debugger so we can connect remotely to the simulation
        // The context must exist beyond the use of the VDB instance, and you can make
        // whatever contexts you like for your own viewer types.
        m_pPhysicsContext = new hkpPhysicsContext();
        hkpPhysicsContext::registerAllPhysicsProcesses(); // all the physics viewers
        m_pPhysicsContext->addWorld(m_pWorld); // add the physics world so the viewers can see it
        contexts.pushBack(m_pPhysicsContext);
    }
    m_pVisualDebugger = new hkVisualDebugger(contexts);
    m_pVisualDebugger->serve();
#endif
}

///
/// @inheritDoc
///
PhysicTask::~PhysicTask(void) {
 #ifdef __HAVOK_VDB__
    {
        m_pVisualDebugger->shutdown();
        m_pVisualDebugger->removeReference();
        m_pPhysicsContext->removeReference();
    }
#endif
}

///
/// @inheritDoc
///
void PhysicTask::Update(f32 DeltaTime) {
    if (DeltaTime > 0.0f) {
        m_DeltaTime = DeltaTime;

        // Udpate Havok multi thread
        hkpStepResult result = GetSystemScene<PhysicScene>()->getWorld()->initMtStep(m_jobQueue , hkReal(DeltaTime));
        ASSERT(result == hkpStepResult::HK_STEP_RESULT_SUCCESS);
        g_serviceManager->getTaskManager()->ParallelFor(this, 
            reinterpret_cast<ITaskManager::ParallelForFunction>(stepUpdateS),
            this, 0, m_cJobs, PhysicSystemTaskGrainSize);
        updateCompletion();
        
        // Update scene
        m_pSystemScene->Update(DeltaTime);
    }
}

///
/// @inheritDoc
///
void PhysicTask::stepUpdateS(PhysicTask* pTask, u32 uStart, u32 uEnd) {
    pTask->stepUpdate();
}

///////////////////////////////////////////////////////////////////////////////
// SetObjectActivation - Add or removes the given object from actively tracked objects
void PhysicTask::setObjectActivation(PhysicObject* pObject, bool bActivated) {
    if (bActivated) {
        if (!pObject->isStatic()) {
#ifdef _DEBUG
            for (auto it : m_ActiveObjects) {
                if (it == pObject) {
                    return;
                }
            }
#endif
            m_ActiveObjects.push_back(pObject);
            GetSystemScene<PhysicScene>()->getWorld()->markForWrite();
            pObject->getBody()->addEntityActivationListener(this);
            GetSystemScene<PhysicScene>()->getWorld()->unmarkForWrite();
        }
    } else {
        m_ActiveObjects.remove(pObject);
        GetSystemScene<PhysicScene>()->getWorld()->markForWrite();
        pObject->getBody()->removeEntityActivationListener(this);
        GetSystemScene<PhysicScene>()->getWorld()->unmarkForWrite();
    }
}

///
/// @inheritDoc
///
void PhysicTask::stepUpdate() {
    m_jobQueue->processAllJobs();
}

///
/// @inheritDoc
///
void PhysicTask::updateCompletion() {
    //
    // End the world stepping.
    //
    hkpStepResult result = GetSystemScene<PhysicScene>()->getWorld()->finishMtStep();
    ASSERT(result == hkpStepResult::HK_STEP_RESULT_SUCCESS);

    //
    // Iterate through all the active objects and have them update themselves
    //  and post their changes.
    //
    auto& activeObjects = m_ActiveObjects;
    for (auto it : activeObjects) {
        it->Update(m_DeltaTime);
    }

    GetSystemScene<PhysicScene>()->getWorld()->lock();

    /*std::list<HavokCharacterObject*> CharacterObjects = m_pScene->GetCharacters();
    for (std::list<HavokCharacterObject*>::iterator it = CharacterObjects.begin(); it != CharacterObjects.end(); it++) {
        (*it)->Update(m_DeltaTime);
    }*/

#ifdef __HAVOK_VDB__
    if (m_pVisualDebugger != NULL) {
        //m_pPhysicsContext->syncTimers();
        m_pVisualDebugger->step();
    }
#endif

    GetSystemScene<PhysicScene>()->getWorld()->unlock();
}

///
/// @inheritDoc
///
void PhysicTask::entityAddedCallback(hkpEntity* pEntity) {
    PhysicObject* pObject = reinterpret_cast<PhysicObject*>(pEntity->getUserData());
    if (pObject != NULL && !pObject->isStatic()) {
        m_ActiveObjects.push_back(pObject);
        pEntity->addEntityActivationListener(this);
    }
}

///
/// @inheritDoc
///
void PhysicTask::entityRemovedCallback(hkpEntity* pEntity) {
    PhysicObject* pObject = reinterpret_cast<PhysicObject*>(pEntity->getUserData());
    ASSERT(pObject != NULL);
    if (!pObject->isStatic()) {
        m_ActiveObjects.remove(pObject);
        pEntity->removeEntityActivationListener(this);
    }
}

///
/// @inheritDoc
///
void PhysicTask::entityActivatedCallback(hkpEntity* pEntity) {
    PhysicObject* pObject = reinterpret_cast<PhysicObject*>(pEntity->getUserData());
    ASSERT(pObject != NULL);
    m_ActiveObjects.push_back(pObject);
}

///
/// @inheritDoc
///
void PhysicTask::entityDeactivatedCallback(hkpEntity* pEntity) {
    PhysicObject* pObject = reinterpret_cast<PhysicObject*>(pEntity->getUserData());
    ASSERT(pObject != NULL);
    m_ActiveObjects.remove(pObject);
}