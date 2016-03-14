﻿// Copyright � 2008-2009 Intel Corporation
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

#include "System.h"
#include "Object/IGeometryObject.h"
#include "Object/ISceneObject.h"
#include "Object/Object.h"

class ISystemScene;
struct KeyboardButtonData;

/**
 * Implementation of the IGraphicsObject interface. See Interfaces\Graphics.h and Interfaces\
 * System.h for a definition of the class and its functions.
 *
 * @sa  ISystemObject
 */
class PlayerInputObject : public InputObject, public ISceneObject, public IGeometryObject {
public:

    /**
     * @inheritDoc
     */
    PlayerInputObject(ISystemScene& pSystemScene, UObject& entity, const Schema::SystemComponent& component);

    /**
     * @inheritDoc
     */
    ~PlayerInputObject();

    /**
     * @inheritDoc
     */
    Error initialize();

    /**
     * @inheritDoc
     */
    System::Changes::BitMask GetPotentialSystemChanges() {
        return System::Changes::Generic::CreateObject | System::Changes::Input::Action 
             | System::Changes::Input::Velocity | System::Changes::Input::Rotation;
    };

    /**
     * @inheritDoc
     */
    System::Types::BitMask GetDesiredSystemChanges() {
        return System::Changes::Physic::Position | System::Changes::Physic::Orientation;
    };

    /**
     * @inheritDoc
     */
    Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

    /**
     * @inheritDoc
     */
    void Update(f32 DeltaTime);

    /**
     * @inheritDoc
     */
    inline const KeyboardButtonData* getKeyboardButtonData() {
        return m_shotKeyboardButtonData;
    }

    /**
     * Create a new shot and queue it.
     */
    void createShot();


    Schema::InputVelocity* getVelocity() override {
        return velocity_;
    }

private:
    Schema::InputVelocity* velocity_;

    OISB::TriggerAction*    m_forwardInputAction;
    OISB::TriggerAction*    m_backwardInputAction;
    OISB::TriggerAction*    m_strafeRightInputAction;
    OISB::TriggerAction*    m_strafeLeftInputAction;
    OISB::TriggerAction*    m_turnRightInputAction;
    OISB::TriggerAction*    m_turnLeftInputAction;
    OISB::TriggerAction*    m_jumpInputAction;

    KeyboardButtonData*     m_shotKeyboardButtonData;

};
