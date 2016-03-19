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


#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <BulletCollision/CollisionShapes/btCapsuleShape.h>

#include "Object/PhysicObject.h"


#include "Scene.h"

/**
 * @inheritDoc
 */
PhysicObject::PhysicObject(ISystemScene& pSystemScene, UObject& entity, const Schema::SystemComponent& component)
    : ISystemObject(&pSystemScene, &entity, component)
    , m_bStatic(false) {
    position_ = const_cast<Schema::PhysicPosition*>(static_cast<const Schema::PhysicPosition*>(component.data()));

    btCollisionShape* playerShape = new btCapsuleShape(1 ,2);
    btDefaultMotionState* fallMotionState =
            new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 2, 0)));
    btScalar mass = 1;
    btVector3 fallInertia(0, 0, 0);
    playerShape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, playerShape, fallInertia);
    rigidBody_ = new btRigidBody(fallRigidBodyCI);
    rigidBody_->setAngularFactor(0);
    GetSystemScene<PhysicScene>()->getDynamicsWorld_()->addRigidBody(rigidBody_);
}

/**
 * @inheritDoc
 */
PhysicObject::~PhysicObject() {

}

/**
 * @inheritDoc
 */
Error PhysicObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    if (ChangeType & System::Changes::Input::Velocity) {
        auto scalar = pSubject->getVelocity()->scalar();
        rigidBody_->setLinearVelocity(btVector3(scalar->x(), scalar->y(), scalar->z()));
        velocity_ = pSubject->getVelocity();
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void PhysicObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);

    //position_->mutate_x(position_->x() + (velocity_->scalar()->x() * DeltaTime));
    //position_->mutate_y(position_->y() + (velocity_->scalar()->y() * DeltaTime));
    //position_->mutate_z(position_->z() + (velocity_->scalar()->z() * DeltaTime));

    btTransform trans;
    rigidBody_->getMotionState()->getWorldTransform(trans);
    position_->mutate_x(trans.getOrigin().getX());
    position_->mutate_y(trans.getOrigin().getY());
    position_->mutate_z(trans.getOrigin().getZ());

    PostChanges(System::Changes::Physic::Position);
}