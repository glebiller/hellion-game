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

#include "Scene.h"

#include <boost/functional/factory.hpp>

#include "System.h"
#include "Task.h"
#include "Object/CharacterPhysicObject.h"
#include "Object/MovablePhysicObject.h"
#include "Object/TerrainPhysicObject.h"

const Math::Vector3 PhysicScene::sm_kDefaultGravity(0.0f, -9.8f, 0.0f);
const Math::Vector3 PhysicScene::sm_kDefaultUp(0.0f, 1.0f, 0.0f);

///////////////////////////////////////////////////////////////////////////////
// HavokPhysicsScene - Default constructor
PhysicScene::PhysicScene(ISystem* pSystem)
    : ISystemScene(pSystem) {
    //m_TaskFactory = boost::factory<PhysicTask*>();

    m_ObjectFactories[Schema::SystemComponentType::PhysicPosition] = boost::factory<PhysicObject*>();
    //m_ObjectFactories["Character"] = boost::factory<CharacterPhysicObject*>();
    //m_ObjectFactories["Movable"] = boost::factory<MovablePhysicObject*>();
    //m_ObjectFactories["Terrain"] = boost::factory<TerrainPhysicObject*>();
}

///////////////////////////////////////////////////////////////////////////////
// ~HavokPhysicsScene - Default destructor
PhysicScene::~PhysicScene() {
    if (m_bInitialized) {
    }
}

///
/// @inheritDoc
///
Error PhysicScene::initialize() {
    ASSERT(!m_bInitialized);

    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void PhysicScene::Update(f32 DeltaTime) {
    for (auto object : m_pObjects) {
        PhysicObject* pObject = static_cast<PhysicObject*>(object.second);
        pObject->Update(DeltaTime);
    }
}

void PhysicScene::createTask() {
    m_pSystemTask = new PhysicTask(this);
}