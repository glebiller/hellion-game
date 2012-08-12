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


//
// extern includes
//
#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include "Common\Base\hkBase.h"
#include "Common\Base\Thread\JobQueue\hkJobQueue.h"
#include "Common\Base\Memory\System\Util\hkMemoryInitUtil.h"
#include "Common\Base\Memory\Allocator\Malloc\hkMallocAllocator.h"
#include "Physics\Dynamics\World\hkpWorld.h"
#include "Physics\Dynamics\Entity\hkpRigidBody.h"
#include "Physics\Dynamics\Entity\hkpEntityListener.h"
#include "Physics\Dynamics\Entity\hkpEntityActivationListener.h"
#include "Physics\Dynamics\Phantom\hkpSimpleShapePhantom.h"
#include "Physics\Collide\Dispatch\hkpAgentRegisterUtil.h"
#include "Physics\Collide\Shape\Convex\Box\hkpBoxShape.h"
#include "Physics\Utilities\CharacterControl\CharacterProxy\hkpCharacterProxy.h"
#include "Physics\Utilities\Destruction\BreakOffParts\hkpBreakOffPartsUtil.h"
#include "Physics\Utilities\Dynamics\Inertia\hkpInertiaTensorComputer.h"

#ifdef __HAVOK_VDB__
#include <Common/Visualize/hkVisualDebugger.h>
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>
#endif
#pragma warning( pop )

//
// core includes
//
#include "BaseTypes.h"
#include "Interface.h"

//
// system includes
//
#include "Scene.h"
#include "Object/Object.h"
#include "Object/ObjectPhysics.h"
#include "Object/ObjectCharacter.h"
#include "Task.h"
#include "Collision.h"
#include "System.h"

//
// Collision system includes
//
#include "Collision.h"


extern ManagerInterfaces    g_Managers;

// Grain sizes of the parallel jobs. The less is the grainsize the better is
// the load balance, and the higher is the parallel overhead.
static const u32    PhysicSystemTaskGrainSize = 8;


///////////////////////////////////////////////////////////////////////////////
// HavokPhysicsTask - Constructor
HavokPhysicsTask::HavokPhysicsTask(
    HavokPhysicsScene* pScene,
    hkJobQueue* pJobQueue,
    hkpWorld* pWorld
)
    : ISystemTask(pScene)
    , m_pScene(pScene)
    , m_jobQueue(pJobQueue)
    , m_pWorld(pWorld) {
    ASSERT(m_pScene != NULL);
    ASSERT(m_jobQueue != NULL);
    ASSERT(m_pWorld != NULL);
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


///////////////////////////////////////////////////////////////////////////////
// ~HavokPhysicsTask - Destructor
HavokPhysicsTask::~HavokPhysicsTask(
    void
) {
    {
        //m_pWorld->removeEntityListener( this );
#ifdef __HAVOK_VDB__
        m_pVisualDebugger->shutdown();
        m_pVisualDebugger->removeReference();
        m_pPhysicsContext->removeReference();
#endif
    }
}


///////////////////////////////////////////////////////////////////////////////
// GetSystemType - Returns System type for this Task
SystemProto::Type HavokPhysicsTask::GetSystemType(void) {
    return SystemProto::Physic;
}


///////////////////////////////////////////////////////////////////////////////
// SetObjectActivation - Add or removes the given object from actively tracked objects
void
HavokPhysicsTask::SetObjectActivation(
    HavokPhysicsObject* pObject,
    bool bActivated
) {
    if (bActivated) {
        if (!pObject->m_bStatic) {
            //
            // Make sure it's not already in the list.
            //
#ifdef _DEBUG
            for (std::list<HavokPhysicsObject*>::iterator it = m_ActiveObjects.begin();
                    it != m_ActiveObjects.end(); it++) {
                if (*it == pObject) {
                    return;
                }
            }

#endif
            m_ActiveObjects.push_back(pObject);
            m_pWorld->markForWrite();
            pObject->m_pBody->addEntityActivationListener(this);
            m_pWorld->unmarkForWrite();
        }
    } else {
        m_ActiveObjects.remove(pObject);
        m_pWorld->markForWrite();
        pObject->m_pBody->removeEntityActivationListener(this);
        m_pWorld->unmarkForWrite();
    }
}


///////////////////////////////////////////////////////////////////////////////
// Update - Update the system (this is were all the work gets done)
void
HavokPhysicsTask::Update(
    f32 DeltaTime
) {
    //
    // Make sure that the time step is greater than 0.
    //
    if (DeltaTime > 0.0f) {
        if (DeltaTime > 0.04f) {
            DeltaTime = 0.04f;
        }

        /*
        //
        // Iterate through all the active object and clear our their contact information.
        //
        for ( std::list<HavokPhysicsObject*>::iterator it=m_ActiveObjects.begin();
              it != m_ActiveObjects.end(); it++ )
        {
            (*it)->m_aIntersectionInfo.clear();
        }
        */
        //
        // Copy the delta time.
        //
        m_DeltaTime = DeltaTime;
        //m_pWorld->stepDeltaTime( hkReal( DeltaTime ) );
        //
        // Start the stepping.
        //
        hkpStepResult result = m_pWorld->initMtStep(m_jobQueue , hkReal(DeltaTime));
        ASSERT(result == hkpStepResult::HK_STEP_RESULT_SUCCESS);
        //
        // Issue jobs for multi-threaded stepping.
        //
        g_Managers.pTask->ParallelFor(
            this,
            reinterpret_cast<ITaskManager::ParallelForFunction>(StepUpdateS),
            this,
            0,
            m_cJobs,
            PhysicSystemTaskGrainSize
        );
        //
        // End the stepping.
        //
        UpdateCompletion();
    }
}


///////////////////////////////////////////////////////////////////////////////
// StepUpdateS - Advance the world owned by this thread (static version)
void
HavokPhysicsTask::StepUpdateS(
    HavokPhysicsTask* pTask,
    u32 uStart,
    u32 uEnd
) {
    UNREFERENCED_PARAM(uStart);
    UNREFERENCED_PARAM(uEnd);
    //
    // Call non-static version for given pTask
    //
    pTask->StepUpdate();
}


///////////////////////////////////////////////////////////////////////////////
// StepUpdate - Advance the world owned by this thread
void
HavokPhysicsTask::StepUpdate(
    void
) {
    //
    // Step the world.
    //
    m_jobQueue->processAllJobs();
}


///////////////////////////////////////////////////////////////////////////////
// UpdateCompletion - Callback function called when all threads have finished
void
HavokPhysicsTask::UpdateCompletion(
    void
) {
    //
    // End the world stepping.
    //
    hkpStepResult result = m_pWorld->finishMtStep();
    ASSERT(result == hkpStepResult::HK_STEP_RESULT_SUCCESS);
    //
    // Iterate through all the active objects and have them update themselves
    //  and post their changes.
    //
    std::list<HavokPhysicsObject*>& ActiveObjects = m_ActiveObjects;

    for (std::list<HavokPhysicsObject*>::iterator it = ActiveObjects.begin();
            it != ActiveObjects.end(); it++) {
        (*it)->Update();
    }

    m_pWorld->lock();
    //
    // Process collision request (need to make this use multithreaded jobs)
    //
    /*HavokPhysicsSystem* pSystem = (HavokPhysicsSystem*)m_pScene->GetSystem();
    pSystem->GetService()->ProcessRequests( m_pScene );

    //
    // Process contact tracking per subject then observer.
    //
    for ( HavokPhysicsScene::CollisionTrackerIt itSubject=m_pScene->m_CollisionTracker.begin();
          itSubject != m_pScene->m_CollisionTracker.end(); itSubject++ )
    {
        hkpRigidBody* pBody = itSubject->first->m_pBody;

        //
        // Get the body information for the contact.
        //
        hkVector4 Position = pBody->getPosition();
        hkQuaternion Rotation = pBody->getRotation();
        hkVector4 LinearVelocity = pBody->getLinearVelocity();
        hkVector4 AngularVelocity = pBody->getAngularVelocity();
        hkAabb AABB;
        pBody->getCollidable()->getShape()->getAabb( hkTransform::getIdentity(), 0.0f, AABB );


        //
        // Create the contact information for the subject.
        //
        IIntersectionObject::Info IntersectionInfo;

        IntersectionInfo.pszName = itSubject->first->GetName();
        Position.store3( &IntersectionInfo.Position.x );
        Rotation.getImag().store3( &IntersectionInfo.Orientation.x );
        IntersectionInfo.Orientation.w = Rotation.getReal();
        LinearVelocity.store3( IntersectionInfo.LinearVelocity );
        AngularVelocity.store3( IntersectionInfo.AngularVelocity );
        AABB.m_min.store3( IntersectionInfo.AABBMin );
        AABB.m_max.store3( IntersectionInfo.AABBMax );

        //
        // Iterate through all the observers for this subject.
        //
        for ( std::vector<HavokPhysicsObject*>::iterator itObserver=itSubject->second.begin();
              itObserver != itSubject->second.end(); itObserver++ )
        {
            //
            // Add the contact information to the observer object.
            //
            (*itObserver)->AddIntersection( IntersectionInfo );
        }
    }*/
    //
    // Process character proxies
    //
    std::list<HavokCharacterObject*> CharacterObjects = m_pScene->GetCharacters();

    for (std::list<HavokCharacterObject*>::iterator it = CharacterObjects.begin(); it != CharacterObjects.end(); it++) {
        (*it)->Update(m_DeltaTime);
    }

#ifdef __HAVOK_VDB__

    if (m_pVisualDebugger != NULL) {
        //m_pPhysicsContext->syncTimers();
        m_pVisualDebugger->step();
    }

#endif
    m_pWorld->unlock();
}


///////////////////////////////////////////////////////////////////////////////
// entityAddedCallback - Callback for when an entity is added by Havok
void
HavokPhysicsTask::entityAddedCallback(
    hkpEntity* pEntity
) {
    HavokPhysicsObject* pObject = reinterpret_cast<HavokPhysicsObject*>(pEntity->getUserData());
    //ASSERT( pObject != NULL );

    if (pObject != NULL && !pObject->m_bStatic) {
        m_ActiveObjects.push_back(pObject);
        pEntity->addEntityActivationListener(this);
    }
}


///////////////////////////////////////////////////////////////////////////////
// entityRemovedCallback - Callback for when an entity is removed by Havok
void
HavokPhysicsTask::entityRemovedCallback(
    hkpEntity* pEntity
) {
    HavokPhysicsObject* pObject = reinterpret_cast<HavokPhysicsObject*>(pEntity->getUserData());
    ASSERT(pObject != NULL);

    if (!pObject->m_bStatic) {
        m_ActiveObjects.remove(pObject);
        pEntity->removeEntityActivationListener(this);
    }
}


///////////////////////////////////////////////////////////////////////////////
// entityActivatedCallback - Callback for when an entity is actived by Havok
void
HavokPhysicsTask::entityActivatedCallback(
    hkpEntity* pEntity
) {
    HavokPhysicsObject* pObject = reinterpret_cast<HavokPhysicsObject*>(pEntity->getUserData());
    ASSERT(pObject != NULL);
    m_ActiveObjects.push_back(pObject);
}


///////////////////////////////////////////////////////////////////////////////
// entityDeactivatedCallback - Callback for when an entity is deactived by Havok
void
HavokPhysicsTask::entityDeactivatedCallback(
    hkpEntity* pEntity
) {
    HavokPhysicsObject* pObject = reinterpret_cast<HavokPhysicsObject*>(pEntity->getUserData());
    ASSERT(pObject != NULL);
    pObject->m_aIntersectionInfo.clear();
    m_ActiveObjects.remove(pObject);
}
