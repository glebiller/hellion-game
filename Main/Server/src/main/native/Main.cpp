/*
 *
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Product and Trade Secret source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2013 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 *
 */

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
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

#include <windows.h>

#include "fr_kissy_hellion_server_service_PhysicService.h"

HINSTANCE               g_hInstance;

///
/// DLL main.
///
/// @param  hModule     The module.
/// @param  Reason      The reason.
/// @param  pReserved   The reserved.
///
/// @return .
///
BOOL APIENTRY DllMain(HMODULE hModule, DWORD Reason, LPVOID pReserved) {
    switch (Reason) {
        case DLL_PROCESS_ATTACH:
            g_hInstance = hModule;
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}   

extern "C" JNIEXPORT void JNICALL Java_fr_kissy_hellion_server_service_PhysicService_initialize(JNIEnv* env, jobject object) {
    //
    // Initialize the base system including our memory system
    //
    // Allocate 0.5MB of physics solver buffer.
    hkMemoryRouter* memoryRouter = hkMemoryInitUtil::initDefault( hkMallocAllocator::m_defaultMallocAllocator, hkMemorySystem::FrameInfo( 500* 1024 ) );
    hkBaseSystem::init( memoryRouter, errorReport );

    //
    // Initialize the multi-threading classes, hkJobQueue, and hkJobThreadPool
    //

    // They can be used for all Havok multithreading tasks. In this exmaple we only show how to use
    // them for physics, but you can reference other multithreading demos in the demo framework
    // to see how to multithread other products. The model of usage is the same as for physics.
    // The hkThreadpool has a specified number of threads that can run Havok jobs.  These can work
    // alongside the main thread to perform any Havok multi-threadable computations.
    // The model for running Havok tasks in Spus and in auxilary threads is identical.  It is encapsulated in the
    // class hkJobThreadPool.  On PlayStation(R)3 we initialize the SPU version of this class, which is simply a SPURS taskset.
    // On other multi-threaded platforms we initialize the CPU version of this class, hkCpuJobThreadPool, which creates a pool of threads
    // that run in exactly the same way.  On the PlayStation(R)3 we could also create a hkCpuJobThreadPool.  However, it is only
    // necessary (and advisable) to use one Havok PPU thread for maximum efficiency. In this case we simply use this main thread
    // for this purpose, and so do not create a hkCpuJobThreadPool.
    //hkJobThreadPool* threadPool;

    // We can cap the number of threads used - here we use the maximum for whatever multithreaded platform we are running on. This variable is
    // set in the following code sections.
    int totalNumThreadsUsed;

    // Get the number of physical threads available on the system
    hkHardwareInfo hwInfo;
    hkGetHardwareInfo(hwInfo);
    totalNumThreadsUsed = hwInfo.m_numThreads;

    // We use one less than this for our thread pool, because we must also use this thread for our simulation
    hkCpuJobThreadPoolCinfo threadPoolCinfo;
    threadPoolCinfo.m_numThreads = totalNumThreadsUsed - 1;

    // This line enables timers collection, by allocating 200 Kb per thread.  If you leave this at its default (0),
    // timer collection will not be enabled.
    threadPoolCinfo.m_timerBufferPerThreadAllocation = 200000;
    threadPool = new hkCpuJobThreadPool( threadPoolCinfo );

    // We also need to create a Job queue. This job queue will be used by all Havok modules to run multithreaded work.
    // Here we only use it for physics.
    hkJobQueueCinfo info;
    info.m_jobQueueHwSetup.m_numCpuThreads = totalNumThreadsUsed;
    //hkJobQueue* jobQueue = new hkJobQueue(info);
    jobQueue = new hkJobQueue(info);

    //
    // Enable monitors for this thread.
    //

    // Monitors have been enabled for thread pool threads already (see above comment).
    hkMonitorStream::getInstance().resize(200000);

    //
    // <PHYSICS-ONLY>: Create the physics world.
    // At this point you would initialize any other Havok modules you are using.
    //
    //hkpWorld* physicsWorld;
    {
        // The world cinfo contains global simulation parameters, including gravity, solver settings etc.
        hkpWorldCinfo worldInfo;

        // Set the simulation type of the world to multi-threaded.
        worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;

        // Flag objects that fall "out of the world" to be automatically removed - just necessary for this physics scene
        worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;

        physicsWorld = new hkpWorld(worldInfo);

        // When the simulation type is SIMULATION_TYPE_MULTITHREADED, in the debug build, the sdk performs checks
        // to make sure only one thread is modifying the world at once to prevent multithreaded bugs. Each thread
        // must call markForRead / markForWrite before it modifies the world to enable these checks.
        physicsWorld->markForWrite();
        
        // Register all collision agents, even though only box - box will be used in this particular example.
        // It's important to register collision agents before adding any entities to the world.
        hkpAgentRegisterUtil::registerAllAgents( physicsWorld->getCollisionDispatcher() );

        // We need to register all modules we will be running multi-threaded with the job queue
        physicsWorld->registerWithJobQueue( jobQueue );

        // Create all the physics rigid bodies
        setupPhysics( physicsWorld );
    }

    //
    // Initialize the VDB
    //
    hkArray<hkProcessContext*> contexts;

    // <PHYSICS-ONLY>: Register physics specific visual debugger processes
    // By default the VDB will show debug points and lines, however some products such as physics and cloth have additional viewers
    // that can show geometries etc and can be enabled and disabled by the VDB app.
    //hkpPhysicsContext* context;
    {
        // The visual debugger so we can connect remotely to the simulation
        // The context must exist beyond the use of the VDB instance, and you can make
        // whatever contexts you like for your own viewer types.
        context = new hkpPhysicsContext();
        hkpPhysicsContext::registerAllPhysicsProcesses(); // all the physics viewers
        context->addWorld(physicsWorld); // add the physics world so the viewers can see it
        contexts.pushBack(context);

        // Now we have finished modifying the world, release our write marker.
        physicsWorld->unmarkForWrite();
    }

    //hkVisualDebugger* vdb = new hkVisualDebugger(contexts);
    vdb = new hkVisualDebugger(contexts);
    vdb->serve();
}

extern "C" JNIEXPORT void JNICALL Java_fr_kissy_hellion_server_service_PhysicService_step(JNIEnv* env, jobject object, jdouble deltaTime) {
    // <PHYSICS-ONLY>:
    // Step the physics world. This single call steps using this thread and all threads
    // in the threadPool. For other products you add jobs, call process all jobs and wait for completion.
    // See the multithreading chapter in the user guide for details
    {
        physicsWorld->stepMultithreaded( jobQueue, threadPool, deltaTime );
    }

    // Step the visual debugger. We first synchronize the timer data
    context->syncTimers( threadPool );
    vdb->step();

    // Clear accumulated timer data in this thread and all slave threads
    hkMonitorStream::getInstance().reset();
    threadPool->clearTimerData();

    // Moving entities
    for (hkpEntity* entity : movingEntities) {
        hkVector4 position = entity->getMotion()->getPosition();
        Proto::Object* object = objectUpdated.add_objects();
        object->set_id("id");
        object->set_name("name");
        Proto::SystemObject* systemObject = object->add_systemobjects();
        systemObject->set_systemtype(Proto::SystemType::Physic);
        systemObject->set_type("Physic");
        Proto::Property* property = systemObject->add_properties();
        property->set_name("Position");
        property->add_value(boost::lexical_cast<std::string>(position.getComponent(0)));
        property->add_value(boost::lexical_cast<std::string>(position.getComponent(1)));
        property->add_value(boost::lexical_cast<std::string>(position.getComponent(2)));
    }
}

extern "C" JNIEXPORT jbyteArray JNICALL Java_fr_kissy_hellion_server_service_PhysicService_entitiesUpdates(JNIEnv* env, jobject object) {
    int size = objectUpdated.ByteSize();
    jbyteArray result = env->NewByteArray(size);
    jbyte* rawjBytes = env->GetByteArrayElements(result, false);
    objectUpdated.SerializeToArray(rawjBytes, size);
    return result;
};

extern "C" JNIEXPORT void JNICALL Java_fr_kissy_hellion_server_service_PhysicService_addEntity(JNIEnv *, jobject, jbyteArray buffer) {
    /*PojoMessage pm;
    jbyte *bufferElems = env->GetByteArrayElements(buffer, 0);
    int len = env->GetArrayLength(buffer);
    try {
        pm.ParseFromArray(reinterpret_cast(bufferElems),
            len);
        // handle message here
    }
    catch (...) {}
    env->ReleaseByteArrayElements(buffer, bufferElems, JNI_ABORT);*/

    physicsWorld->lock();
    hkVector4 groundPos(0.0f, 0.0f, 0.0f);
    hkVector4 posy = groundPos;

    const hkReal radius = 1.5f;
    const hkReal sphereMass = 150.0f;

    hkVector4 relPos(0.0f, radius + 0.0f, 50.0f);

    hkpRigidBodyCinfo info;
    hkMassProperties massProperties;
    hkpInertiaTensorComputer::computeSphereVolumeMassProperties(radius, sphereMass, massProperties);

    info.m_mass = massProperties.m_mass;
    info.m_centerOfMass = massProperties.m_centerOfMass;
    info.m_inertiaTensor = massProperties.m_inertiaTensor;
    info.m_shape = new hkpSphereShape(radius);
    info.m_position.setAdd4(posy, relPos);
    info.m_motionType = hkpMotion::MOTION_SPHERE_INERTIA;


    hkpRigidBody* sphereRigidBody = new hkpRigidBody(info);
    g_ball = sphereRigidBody;
    EntityListener* entityListener = new EntityListener();
    sphereRigidBody->addEntityActivationListener(entityListener);
    sphereRigidBody->addEntityListener(entityListener);

    physicsWorld->addEntity(sphereRigidBody);
    sphereRigidBody->removeReference();
    info.m_shape->removeReference();

    hkVector4 vel(0.0f, 0.0f, -100.0f);
    sphereRigidBody->applyLinearImpulse(vel);
    physicsWorld->unlock();
}
