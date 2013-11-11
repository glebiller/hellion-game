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
    m_upInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_Up");
    m_rightInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_R");
    m_downInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_Down");
    m_leftInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_E");
    m_rightRotateInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_Right");
    m_leftRotateInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_Left");
    m_shotInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName() + "_Shot");
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
    
    m_upInputAction->bind("Keyboard/HAUT");
    m_rightInputAction->bind("Keyboard/R");
    m_downInputAction->bind("Keyboard/BAS");
    m_leftInputAction->bind("Keyboard/E");
    m_rightRotateInputAction->bind("Keyboard/DROITE");
    m_leftRotateInputAction->bind("Keyboard/GAUCHE");
    m_shotInputAction->bind("Keyboard/J");

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
    
    if (m_upInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        m_velocity.y += m_upInputAction->isActive() ? -1 : 1;
    }
    if (m_rightInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        m_velocity.x += m_rightInputAction->isActive() ? 1 : -1;
    }
    if (m_downInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        m_velocity.y += m_downInputAction->isActive() ? 1 : -1;
    }
    if (m_leftInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Velocity;
        m_velocity.x += m_leftInputAction->isActive() ? -1 : 1;
    }
    if (m_rightRotateInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Rotation;
        m_rotation.z += m_rightRotateInputAction->isActive() ? -1 : 1;
    }
    if (m_leftRotateInputAction->hasChanged()) {
        mModified |= System::Changes::Input::Rotation;
        m_rotation.z += m_leftRotateInputAction->isActive() ? 1 : -1;
    }
    if (m_shotInputAction->hasChanged()) {
        if (m_shotInputAction->isActive()) {
            createShot();
        }
        mModified |= System::Changes::Input::Action;
        m_shotKeyboardButtonData->down = m_shotInputAction->isActive();
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