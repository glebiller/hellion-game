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

#include <btBulletCollisionCommon.h>
#include <schema/entity_change_generated.h>
#include <schema/physic_components_generated.h>

#include "System/ISystemScene.h"
#include "System/ISystemObject.h"

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
    PhysicObject(ISystemScene &pSystemScene, UObject &entity, const Schema::SystemComponent& component);

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
    ISystemObject::Changes GetPotentialSystemChanges() override {
        return Schema::EntityChange::PhysicPosition;
    };

    /**
     * @inheritDoc
     */
    IObserver::Changes GetDesiredSystemChanges() override {
        return 0;
    };

    /**
     * @inheritDoc
     */
    Error ChangeOccurred(ISystemObject* systemObject, IObserver::Changes changes) override;

    /**
     * @inheritDoc
     */
    void Update(float DeltaTime) override;

protected:
    Schema::Components::PhysicPosition* position_;
    bool m_bStatic;

};
