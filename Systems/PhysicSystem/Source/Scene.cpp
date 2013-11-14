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
#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6385 6323 6386 )
#include <Common/Base/hkBase.h>
#include <Common/SceneData/Graph/hkxNode.h>
#include <Common/SceneData/Scene/hkxScene.h>
#include <Common/Serialize/Util/hkLoader.h>
#include <Common/Serialize/Util/hkRootLevelContainer.h>

#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
#include <Physics2012/Dynamics/Entity/hkpEntityListener.h>
#include <Physics2012/Dynamics/Entity/hkpEntityActivationListener.h>
#include <Physics2012/Dynamics/Phantom/hkpSimpleShapePhantom.h>
#include <Physics2012/Dynamics/World/hkpWorld.h>
#include <Physics2012/Dynamics/Collide/Deprecated/hkpCollisionListener.h>
#include <Physics2012/Collide/Dispatch/hkpAgentRegisterUtil.h>
#include <Physics2012/Collide/Filter/Group/hkpGroupFilterSetup.h>
#include <Physics2012/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h>
#include <Physics2012/Collide/Shape/Compound/Collection/List/hkpListShape.h>
#include <Physics2012/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h>
#include <Physics2012/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h>
#include <Physics2012/Collide/Shape/Compound/Tree/Mopp/hkpMoppCompilerInput.h>
#include <Physics2012/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h>
#include <Physics2012/Collide/Shape/Convex/Box/hkpBoxShape.h>
#include <Physics2012/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h>
#include <Physics2012/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics2012/Utilities/Destruction/BreakOffParts/hkpBreakOffPartsUtil.h>
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
#include <Physics2012/Utilities/Serialize/hkpPhysicsData.h>

#ifdef __HAVOK_VDB__
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>
#endif
#pragma warning( pop )

#include "System.h"
#include "Task.h"
#include "Object/PhysicObject.h"
#include "Object/CharacterPhysicObject.h"
#include "Object/MovablePhysicObject.h"
#include "Object/TerrainPhysicObject.h"

const Math::Vector3 PhysicScene::sm_kDefaultGravity(0.0f, -9.8f, 0.0f);
const Math::Vector3 PhysicScene::sm_kDefaultUp(0.0f, 1.0f, 0.0f);

///////////////////////////////////////////////////////////////////////////////
// HavokPhysicsScene - Default constructor
PhysicScene::PhysicScene(ISystem* pSystem)
    : ISystemScene(pSystem) 
    , m_pWorld(nullptr) {
    m_TaskFactory = boost::factory<PhysicTask*>();

    m_ObjectFactories["Character"] = boost::factory<CharacterPhysicObject*>();
    m_ObjectFactories["Movable"] = boost::factory<MovablePhysicObject*>();
    m_ObjectFactories["Terrain"] = boost::factory<TerrainPhysicObject*>();
}

///////////////////////////////////////////////////////////////////////////////
// ~HavokPhysicsScene - Default destructor
PhysicScene::~PhysicScene(void) {
    if (m_bInitialized) {
        m_pWorld->markForWrite();
        m_pWorld->removeReference();
    }
}

///
/// @inheritDoc
///
Error PhysicScene::initialize(void) {
    ASSERT(!m_bInitialized);
    
    //
    // Create the world
    //
    {
        hkpWorldCinfo worldInfo;
        worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
        worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;
        worldInfo.setBroadPhaseWorldSize(10000.0f);
        worldInfo.m_collisionTolerance = 0.03f;
        m_pWorld = new hkpWorld(worldInfo);
    }

    //
    // Start editing the world.
    //
    m_pWorld->markForWrite();
    hkpAgentRegisterUtil::registerAllAgents(m_pWorld->getCollisionDispatcher());
    m_pWorld->registerWithJobQueue(GetSystem<PhysicSystem>()->getJobQueue());
    m_pWorld->unmarkForWrite();

    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void PhysicScene::Update(f32 DeltaTime) {
    
}
