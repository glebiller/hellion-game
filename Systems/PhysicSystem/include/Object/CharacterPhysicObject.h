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

#pragma once

#include <System/ISystemObject.h>
#include <schema/entity_change_generated.h>
#include <LinearMath/btVector3.h>
#include <LinearMath/btTransform.h>
#include <BulletDynamics/Dynamics/btRigidBody.h>

///
/// <c>CharacterPhysicObject</c> Implementation of the ISystemObject interface. This is the
/// Character object created objects using Havok Character Proxy.
///
/// @sa PhysicObject
///
class CharacterPhysicObject : public ISystemObject {
public:
    CharacterPhysicObject(ISystemScene& pSystemScene, UObject& entity, const Schema::SystemComponent& component);

    ~CharacterPhysicObject();

    ISystemObject::Changes GetPotentialSystemChanges() override {
        return Schema::EntityChange::PhysicPosition;
    };

    IObserver::Changes GetDesiredSystemChanges() override {
        return Schema::EntityChange::InputVelocity | Schema::EntityChange::PhysicPosition;
    };

    Error ChangeOccurred(ISystemObject* systemObject, IObserver::Changes changes) override;

    void Update(float DeltaTime) override;

private:
    Schema::Components::PhysicPosition* position_;
    btRigidBody* rigidBody_;
    btTransform transform_;
    btVector3 velocity_;

    float m_Radius;

};
