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
#include "Object/PlayerObject.h"
#include "Object/IKeyboardObject.h"

/**
 * @inheritDoc
 */
PlayerInputObject::PlayerInputObject(ISystemScene& pSystemScene, UObject& entity, const Schema::SystemComponent& component)
    : InputObject(&pSystemScene, &entity, component) {
    velocity_ = const_cast<Schema::InputVelocity*>(static_cast<const Schema::InputVelocity*>(component.data()));
    m_shotKeyboardButtonData = new KeyboardButtonData();
    InputScene* inputScene = GetSystemScene<InputScene>();
    m_forwardInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity.getName() + "_Forward");
    m_backwardInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity.getName() + "_Backward");
    m_strafeRightInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity.getName() + "_StrafeRight");
    m_strafeLeftInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity.getName() + "_StrafeLeft");
    m_turnRightInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity.getName() + "_TurnRight");
    m_turnLeftInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity.getName() + "_TurnLeft");
    m_jumpInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity.getName() + "_Jump");

    m_forwardInputAction->bind("Keyboard/w");
    m_backwardInputAction->bind("Keyboard/s");
    m_strafeRightInputAction->bind("Keyboard/d");
    m_strafeLeftInputAction->bind("Keyboard/a");
    m_turnRightInputAction->bind("Keyboard/e");
    m_turnLeftInputAction->bind("Keyboard/q");
    m_jumpInputAction->bind("Keyboard/Space");
}

/**
 * @inheritDoc
 */
PlayerInputObject::~PlayerInputObject() {
    
}

/**
 * @inheritDoc
 */
Error PlayerInputObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {

    
    /*if (ChangeType & System::Changes::Physic::Position) {
        m_position = *dynamic_cast<IGeometryObject*>(pSubject)->GetPosition();
    }
    if (ChangeType & System::Changes::Physic::Orientation) {
        m_orientation = *dynamic_cast<IGeometryObject*>(pSubject)->GetOrientation();
    }*/

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void PlayerInputObject::Update(f32 DeltaTime) {


    u32 mModified = 0;

    if (m_forwardInputAction->hasChanged() || m_backwardInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        int newValue = m_backwardInputAction->isActive() || m_forwardInputAction->isActive();
        if (!m_forwardInputAction->isActive()) {
            newValue = -newValue;
        }
        velocity_->mutable_scalar()->mutate_x(newValue);
    }
    if (m_strafeRightInputAction->hasChanged() || m_strafeLeftInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        int newValue = 0;
        if (m_strafeRightInputAction->isActive()) {
            newValue = 1;
        } else if (m_strafeLeftInputAction->isActive()) {
            newValue = -1;
        } else {
            newValue = 0;
        }
        velocity_->mutable_scalar()->mutate_z(newValue);
    }
    if (m_turnRightInputAction->hasChanged()) {
        //mModified |= System::Changes::Input::Rotation;
        //int yModifier = m_turnRightInputAction->isActive() ? -1 : 1;
        //velocity_->mutable_scalar()->mutate_y(velocity_->scalar()->y() + yModifier);
    }
    if (m_turnLeftInputAction->hasChanged()) {
        //mModified |= System::Changes::Input::Rotation;
        //int yModifier = m_turnLeftInputAction->isActive() ? 1 : -1;
        //velocity_->mutable_scalar()->mutate_y(velocity_->scalar()->y() + yModifier);
    }
    if (m_jumpInputAction->hasChanged()) {
        if (m_jumpInputAction->isActive()) {
            createShot();
        }
        //mModified |= System::Changes::Input::Action;
        //m_shotKeyboardButtonData->down = m_jumpInputAction->isActive();
    }
    
    if (mModified != 0) {
        PostChanges(mModified);
    }
}

/**
 * @inheritDoc
 */
void PlayerInputObject::createShot() {
    /*Proto::Object shotProto;
    // TODO use boost for object ID
    shotProto.set_id(ObjectId::gen().str());
    shotProto.set_name("Shot");
    shotProto.set_template_("ShotTemplate");

    auto physicSystemObject = shotProto.add_systemobjects();
    physicSystemObject->set_systemtype(Proto::SystemType::Physic);
    physicSystemObject->set_type("Movable");

    auto positionProperty = physicSystemObject->add_properties();
    positionProperty->set_name("Position");
    getVector3(&m_position, positionProperty->mutable_value());
    auto orientationProperty = physicSystemObject->add_properties();
    orientationProperty->set_name("Orientation");
    getQuaternion(&m_orientation, orientationProperty->mutable_value());

    auto networkSystemObject = shotProto.add_systemobjects();
    networkSystemObject->set_systemtype(Proto::SystemType::Network);
    networkSystemObject->set_type("Replicable");

    m_createObjectQueue->push_back(shotProto);*/
    //PostChanges(System::Changes::Generic::CreateObject);

}