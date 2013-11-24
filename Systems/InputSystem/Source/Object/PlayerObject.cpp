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
#include "Object.h"
#include "Object/PlayerObject.h"
#include "Object/IKeyboardObject.h"
#include "ObjectId/ObjectId.h"

/**
 * @inheritDoc
 */
PlayerInputObject::PlayerInputObject(ISystemScene* pSystemScene, IEntity* entity) 
    : InputObject(pSystemScene, entity)
    , ISceneObject()
    , IGeometryObject() {
    
    m_shotKeyboardButtonData = new KeyboardButtonData();
    InputScene* inputScene = GetSystemScene<InputScene>();
    m_forwardInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_Forward");
    m_backwardInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_Backward");
    m_strafeRightInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_StrafeRight");
    m_strafeLeftInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_StrafeLeft");
    m_turnRightInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_TurnRight");
    m_turnLeftInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_TurnLeft");
    m_jumpInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_Jump");
}

/**
 * @inheritDoc
 */
PlayerInputObject::~PlayerInputObject(void) {
    
}

/**
 * @inheritDoc
 */
Error PlayerInputObject::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_forwardInputAction->bind("Keyboard/HAUT");
    m_backwardInputAction->bind("Keyboard/BAS");
    m_strafeRightInputAction->bind("Keyboard/R");
    m_strafeLeftInputAction->bind("Keyboard/E");
    m_turnRightInputAction->bind("Keyboard/DROITE");
    m_turnLeftInputAction->bind("Keyboard/GAUCHE");
    m_jumpInputAction->bind("Keyboard/ESPACE");

    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error PlayerInputObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    
    if (ChangeType & System::Changes::Physic::Position) {
        m_position = *dynamic_cast<IGeometryObject*>(pSubject)->GetPosition();
    }
    if (ChangeType & System::Changes::Physic::Orientation) {
        m_orientation = *dynamic_cast<IGeometryObject*>(pSubject)->GetOrientation();
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void PlayerInputObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);

    u32 mModified = 0;
    
    if (m_forwardInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        m_velocity.x += m_forwardInputAction->isActive() ? 1 : -1;
    }
    if (m_backwardInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        m_velocity.x += m_backwardInputAction->isActive() ? -1 : 1;
    }
    if (m_strafeRightInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        m_velocity.z += m_strafeRightInputAction->isActive() ? 1 : -1;
    }
    if (m_strafeLeftInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        m_velocity.z += m_strafeLeftInputAction->isActive() ? -1 : 1;
    }
    if (m_turnRightInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Rotation;
        m_rotation.y += m_turnRightInputAction->isActive() ? -1 : 1;
    }
    if (m_turnLeftInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Rotation;
        m_rotation.y += m_turnLeftInputAction->isActive() ? 1 : -1;
    }
    if (m_jumpInputAction->hasChanged()) {
        if (m_jumpInputAction->isActive()) {
            //createShot();
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
void PlayerInputObject::createShot(void) {
    Proto::Object shotProto;
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

    m_createObjectQueue->push_back(shotProto);
    PostChanges(System::Changes::Generic::CreateObject);
}