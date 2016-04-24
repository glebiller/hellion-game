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

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <OgreImage.h>
#include <OgreColourValue.h>
#include <OgreResourceGroupManager.h>
#include <Scene.h>

///
/// @inheritDoc
///
TerrainPhysicObject::TerrainPhysicObject(ISystemScene& pSystemScene, UObject& entity,
                                         const Schema::SystemComponent& component)
        : PhysicObject(pSystemScene, entity, component) {
    Ogre::Image img;
    img.load("terrain/terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    const int xRes = 257;
    const int zRes = 257;
    float minHeight = 1;
    float maxHeight = 0;
    float* pHeightData = new float[xRes * zRes];
    {
        for (int x = 0; x < xRes; x++) {
            for (int z = 0; z < zRes; z++) {
                float height = img.getColourAt((size_t) z, (size_t) x, 0).r;
                pHeightData[x * zRes + z] = height;
                if (minHeight > height) {
                    minHeight = height;
                }
                if (maxHeight < height) {
                    maxHeight = height;
                }
            }
        }
    }
    //
    // Create the shape
    //
    btHeightfieldTerrainShape* heightfieldShape = new btHeightfieldTerrainShape(xRes, zRes, pHeightData,
                                                                                100, minHeight, maxHeight,
                                                                                1, PHY_FLOAT, false);
    btVector3 localScaling(4, 100, 4);
    heightfieldShape->setLocalScaling(localScaling);

    rigidBody_ = new btRigidBody(0, new btDefaultMotionState(), heightfieldShape);
    rigidBody_->setFriction(0.8f);
    rigidBody_->setHitFraction(0.8f);
    rigidBody_->setRestitution(0.6f);
    rigidBody_->getWorldTransform().setOrigin(btVector3(0, 50, 0));
    rigidBody_->getWorldTransform().setRotation(btQuaternion::getIdentity());
    rigidBody_->setCollisionFlags(rigidBody_->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);

    GetSystemScene<PhysicScene>()->getDynamicsWorld_()->addRigidBody(rigidBody_);
}

///
/// @inheritDoc
///
TerrainPhysicObject::~TerrainPhysicObject() {
}

///
/// @inheritDoc
///
void TerrainPhysicObject::Update(float DeltaTime) {

}

///
/// @inheritDoc
///
Error TerrainPhysicObject::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {

    return Errors::Success;
}
