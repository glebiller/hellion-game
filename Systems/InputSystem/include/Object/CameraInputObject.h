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

#include <OISB.h>
#include <Framework/include/schema/entity_change_generated.h>

#include "Object/Object.h"

class UObject;

/**
 * <c>CameraInputObject</c> Implementation of the ISystemObject interface.
 * This is the Camera object created objects.
 * 
 * @sa  InputObject
 * @sa  IGeometryObject
 */
class CameraInputObject : public InputObject {
public:

    ///
    /// @inheritDoc.
    ///
    /// @param [in,out] pSystemScene    If non-null, the system scene.
    /// @param  pszName                 The name.
    ///
    CameraInputObject(ISystemScene& pSystemScene, UObject& entity, const Schema::SystemComponent& component);

    ///
    /// @inheritDoc.
    ///
    ~CameraInputObject();

    ///
    /// @inheritDoc.
    ///
    void Update(float DeltaTime);

    ///
    /// @inheritDoc.
    ///
    ISystemObject::Changes GetPotentialSystemChanges() {
        return Schema::EntityChange::InputVelocity | Schema::EntityChange::PhysicPosition;
    };

    ///
    /// @inheritDoc.
    ///
    IObserver::Changes GetDesiredSystemChanges() {
        return Schema::EntityChange::PhysicPosition;
    };

    ///
    /// @inheritDoc.
    ///
    Error ChangeOccurred(ISystemObject* systemObject, IObserver::Changes changes);

private:
    Schema::Components::InputVelocity* velocity_;

    OISB::AnalogAxisAction* m_rotateUpDownAction;
    OISB::AnalogAxisAction* m_rotateRightLeftAction;

};

