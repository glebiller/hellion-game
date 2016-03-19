// Copyright 2008-2009 Intel Corporation
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

#include "Object/TerrainPhysicObject.h"

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
#include <Physics2012/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldBaseCinfo.h>
#include <Physics2012/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h>
#include <Physics2012/Utilities/Serialize/hkpPhysicsData.h>

#ifdef __HAVOK_VDB__
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>
#endif
#pragma warning( pop )

#include <OgreImage.h>
#include <OgreColourValue.h>
#include <OgreResourceGroupManager.h>

#include "Scene.h"
#include "Shape/TerrainShape.h"

///
/// @inheritDoc
///
TerrainPhysicObject::TerrainPhysicObject(ISystemScene* pSystemScene, UObject* entity)
    : PhysicObject(pSystemScene, entity) {

}

///
/// @inheritDoc
///
TerrainPhysicObject::~TerrainPhysicObject() {
}

///
/// @inheritDoc
///
Error TerrainPhysicObject::initialize() {


    //
    // Generate terrain
    //
    Ogre::Image img;
    img.load("terrain/terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    const int xRes = 257;
    const int zRes = 257;
    hkUint16* pHeightData = hkAllocate<hkUint16>(xRes * zRes, HK_MEMORY_CLASS_PHYSICS);
    {
        for (int x = 0; x < xRes; x++) {
            for (int z = 0; z < zRes; z++) {
                float height = img.getColourAt(x, z, 0).r;
                // Convert to 16 bit
                pHeightData[x * zRes + z] = static_cast<hkUint16>(hkUint16(-1) * height);
            }
        }
    }
    //
    // Create the shape
    //
    hkpSampledHeightFieldBaseCinfo ci;
    ci.m_xRes = xRes;
    ci.m_zRes = zRes;
    ci.m_maxHeight = 100.0f;
    ci.m_minHeight = 0.0f;
    ci.m_scale = hkVector4(4.0f, 1.0f, 4.0f, 0.0f);

    hkpWorld* world = GetSystemScene<PhysicScene>()->getWorld();
    world->lock();

    //
    // now to make a rigid body out of our height field
    //
    {
        TerrainShape* pTerrainShape = new TerrainShape(ci, pHeightData);
        hkpRigidBodyCinfo bodyInfo;
        bodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
        bodyInfo.m_position = hkVector4(- 2.0f * (xRes - 1.0f), 0.0f, - 2.0f * (zRes - 1.0f));
        bodyInfo.m_shape = pTerrainShape;
        bodyInfo.m_friction = 0.5f;
        m_pBody = new hkpRigidBody(bodyInfo);
        m_pBody->setName(m_entity->getName().c_str());
        m_pBody->setUserData(hkUlong(this));
        world->addEntity(m_pBody);
        m_pBody->removeReference();
        pTerrainShape->removeReference();
    }

    //
    // Now we have finished modifying the world, release our write marker.
    //
    world->unlock();
    return Errors::Success;
}

///
/// @inheritDoc
///
void TerrainPhysicObject::Update(f32 DeltaTime) {

}

///
/// @inheritDoc
///
Error TerrainPhysicObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {

    return Errors::Success;
}
