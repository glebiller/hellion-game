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

#include "Task.h"

#include <Manager/IServiceManager.h>

#include "Scene.h"
#include "System.h"
#include "Object/PhysicObject.h"

extern IServiceManager* g_serviceManager;

// Grain sizes of the parallel jobs. The less is the grainsize the better is
// the load balance, and the higher is the parallel overhead.
static const u32    PhysicSystemTaskGrainSize = 8;

///
/// @inheritDoc
///
PhysicTask::PhysicTask(ISystemScene* pScene) 
    : ISystemTask(pScene) {

}

///
/// @inheritDoc
///
PhysicTask::~PhysicTask() {
}

///
/// @inheritDoc
///
Error PhysicTask::initialize() {


    return Errors::Success;
}

///
/// @inheritDoc
///
void PhysicTask::Update(f32 DeltaTime) {
    if (DeltaTime > 0.0f) {
        m_DeltaTime = DeltaTime;

        GetSystemScene<PhysicScene>()->getDynamicsWorld_()->stepSimulation(DeltaTime);

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

///
/// @inheritDoc
///
void PhysicTask::stepUpdate() {

}

///
/// @inheritDoc
///
void PhysicTask::updateCompletion() {
    //
    // Iterate through all the active objects and have them update themselves
    //  and post their changes.
    //
    auto& activeObjects = m_ActiveObjects;
    for (auto it : activeObjects) {
        it->Update(m_DeltaTime);
    }
}