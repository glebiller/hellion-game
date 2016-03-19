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

#include <System/Types.h>

#include "System/ISystemScene.h"
#include "System/ISystemObject.h"
#include "Object/IGeometryObject.h"

class btRigidBody;

///
/// <c>HavokObject</c> Implementation of the ISystemObject interface. This is the base object
/// created by the HavokPhysics Scene.
///
/// @sa ISystemObject
/// @sa IGeometryObject
///
class PhysicObject : public ISystemObject {
public:
    ///
    /// @inheritDoc.
    ///
    PhysicObject(ISystemScene& pSystemScene, UObject& entity, const Schema::SystemComponent& component);

    ///
    /// @inheritDoc.
    ///
    virtual ~PhysicObject();

    ///
    /// Query if this object is static.
    ///
    /// @return true if static, false if not.
    ///
    inline bool isStatic() {
        return m_bStatic;
    };

    /**
     * @inheritDoc
     */
    System::Changes::BitMask GetPotentialSystemChanges() override {
        return System::Changes::Physic::Position | System::Changes::Physic::Orientation;
    };

    /**
     * @inheritDoc
     */
    System::Types::BitMask GetDesiredSystemChanges() override {
        return System::Changes::Input::Velocity | System::Changes::Input::Rotation;
    };

    /**
     * @inheritDoc
     */
    Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) override;

    /**
     * @inheritDoc
     */
    void Update(f32 DeltaTime) override;

    Schema::PhysicPosition* getPosition() override {
        return position_;
    }

protected:
    Schema::PhysicPosition* position_;
    const Schema::InputVelocity* velocity_;
    btRigidBody* rigidBody_;
    bool                m_bStatic;

};
