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

#include <boost/functional/factory.hpp>
#include <boost/bind.hpp>

#include <btBulletDynamicsCommon.h>

#include "System.h"
#include "Manager/ServiceManager.h"
#include "Scene.h"

extern IServiceManager* g_serviceManager;

u32 PhysicSystem::s_idMainThread = 0;
tbb::atomic<u32> PhysicSystem::s_threadNumberCount;

/**
 * @inheritDoc
 */
PhysicSystem::PhysicSystem() : ISystem(),
                               collisionConfiguration_(new btDefaultCollisionConfiguration()),
                               collisionDispatcher_(new btCollisionDispatcher(collisionConfiguration_)),
                               broadphaseInterface_(new btDbvtBroadphase()) {
//m_propertySetters["Imageset"] = boost::bind(&GuiSystem::setImagesetResourceGroup, this, _1);
}

/**
 * @inheritDoc
 */
PhysicSystem::~PhysicSystem() {
    delete broadphaseInterface_;
    delete collisionDispatcher_;
    delete collisionConfiguration_;
    if (m_bInitialized) {
    }
}

/**
 * @inheritDoc
 */
Error PhysicSystem::initialize() {
    ASSERT(!m_bInitialized);

    m_bInitialized = true;
    return Errors::Success;
}

ISystemScene* PhysicSystem::createScene() {
    m_pSystemScene = new PhysicScene(this);
    return m_pSystemScene;
}

/**
 * @inheritDoc
 */
void PhysicSystem::AllocateThreadResources(PhysicSystem* pSystem) {

}

/**
 * @inheritDoc
 */
void PhysicSystem::FreeThreadResources(PhysicSystem* pSystem) {

}

/**
 * @inheritDoc
 */
void PhysicSystem::ErrorReport(const char* pString, void* pErrorOutputObject) {
    PhysicSystem* pSystem = reinterpret_cast<PhysicSystem*>(pErrorOutputObject);

}
