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

// TODO better
#define int_p_NULL NULL

#include <btBulletDynamicsCommon.h>
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
#include <Scene.h>

///
/// @inheritDoc
///
TerrainPhysicObject::TerrainPhysicObject(ISystemScene& pSystemScene, UObject& entity,
                                         const Schema::SystemComponent& component)
        : PhysicObject(pSystemScene, entity, component) {
    boost::gil::rgb8_image_t image;
    boost::gil::png_read_and_convert_image("Assets/Media/Graphic/terrain/terrain2.png", image);

    const int xRes = 257;
    const int zRes = 257;
    unsigned char* pHeightData = new unsigned char[xRes * zRes];
    {
        for (int x = 0; x < xRes; x++) {
            for (int z = 0; z < zRes; z++) {
                boost::gil::rgb8_pixel_t pixel = boost::gil::const_view(image)(x, z);
                float height = pixel[0];
                pHeightData[x * zRes + z] = (unsigned char) (height * 255);
            }
        }
    }
    //
    // Create the shape
    //
    btHeightfieldTerrainShape* heightfieldShape =
            new btHeightfieldTerrainShape(xRes, zRes,
                                          pHeightData,
                                          1,
                                          0, 100.0f,
                                          1, PHY_INTEGER, false);

    rigidBody_ = new btRigidBody(0, new btDefaultMotionState(), heightfieldShape);
    rigidBody_->setFriction(0.8f);
    rigidBody_->setHitFraction(0.8f);
    rigidBody_->setRestitution(0.6f);
    rigidBody_->getWorldTransform().setOrigin(btVector3(0, 0, 0));
    rigidBody_->getWorldTransform().setRotation(btQuaternion());
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
