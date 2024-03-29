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
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>

#include "Scene.h"

#include "System.h"
#include "Task.h"
#include "Object/CharacterPhysicObject.h"
#include "Object/MovablePhysicObject.h"
#include "Object/TerrainPhysicObject.h"
#include "DebugDrawer.h"

///////////////////////////////////////////////////////////////////////////////
// HavokPhysicsScene - Default constructor
PhysicScene::PhysicScene(ISystem* pSystem, const Schema::SystemScene* systemScene)
        : ISystemScene(pSystem, systemScene),
          constraintSolver_(new btSequentialImpulseConstraintSolver()) {
    dynamicsWorld_ = new btDiscreteDynamicsWorld(GetSystem<PhysicSystem>()->getCollisionDispatcher(),
                                                 GetSystem<PhysicSystem>()->getBroadphaseInterface(),
                                                 constraintSolver_,
                                                 GetSystem<PhysicSystem>()->getCollisionConfiguration());
    dynamicsWorld_->setGravity(btVector3(0, -9.8f, 0));

    m_ObjectFactories[Schema::ComponentType::PhysicPosition] = boost::factory<PhysicObject*>();
    m_ObjectFactories[Schema::ComponentType::PhysicCharacter] = boost::factory<CharacterPhysicObject*>();
    //m_ObjectFactories["Movable"] = boost::factory<MovablePhysicObject*>();
    m_ObjectFactories[Schema::ComponentType::PhysicTerrain] = boost::factory<TerrainPhysicObject*>();
    m_ObjectFactories[Schema::ComponentType::PhysicDebug] = boost::factory<DebugDrawer*>();
}

///////////////////////////////////////////////////////////////////////////////
// ~HavokPhysicsScene - Default destructor
PhysicScene::~PhysicScene() {
    delete dynamicsWorld_;
    delete constraintSolver_;
}

/**
 * @inheritDoc
 */
void PhysicScene::Update(float DeltaTime) {
    for (auto object : m_pObjects) {
        PhysicObject* pObject = static_cast<PhysicObject*>(object.second);
        pObject->Update(DeltaTime);
    }
    dynamicsWorld_->debugDrawWorld();
}

void PhysicScene::createTask() {
    m_pSystemTask = new PhysicTask(this);
}