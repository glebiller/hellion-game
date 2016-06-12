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

#include "Object/CameraInputObject.h"

#include <OISMouse.h>
#include <schema/scene_generated.h>
#include <schema/physic_components_generated.h>

#include "Scene.h"

///
/// @inheritDo
///
CameraInputObject::CameraInputObject(ISystemScene& pSystemScene, UObject& entity, const Schema::SystemComponent& component)
    : InputObject(&pSystemScene, &entity, component) {
    auto systemObject = entity.GetExtension(Schema::ComponentType::InputVelocity);
    velocity_ = systemObject->getMutableComponent<Schema::Components::InputVelocity>();

    InputScene* inputScene = GetSystemScene<InputScene>();
    m_rotateUpDownAction = inputScene->getDefaultSchema()->createAction<OISB::AnalogAxisAction>(entity.getName() + "_UpDown");
    m_rotateRightLeftAction = inputScene->getDefaultSchema()->createAction<OISB::AnalogAxisAction>(entity.getName() + "_RightLeft");

    m_rotateUpDownAction->setUseAbsoluteValues(true);
    m_rotateUpDownAction->setAnalogEmulator(nullptr);
    m_rotateUpDownAction->setMinimumValue(0);
    m_rotateUpDownAction->setMaximumValue(1024);
    m_rotateUpDownAction->bind("Mouse/X Axis");
    m_rotateRightLeftAction->bind("Mouse/Y Axis");

    // TODO better
    OIS::Mouse* mouse = OISB::System::getSingleton().getOISMouse();
    mouse->getMouseState().width = 1024;
    mouse->getMouseState().height = 728;
}

///
/// @inheritDoc
///
CameraInputObject::~CameraInputObject() {

}

///
/// @inheritDoc
///
void CameraInputObject::Update(float DeltaTime) {
    velocity_->mutable_angular()->mutate_y(m_rotateRightLeftAction->getRelativeValue());
    //PostChanges(Schema::EntityChange::InputVelocity);
    /*m_rotation.y = m_rotateUpDownAction->getRelativeValue();
    m_rotation.x = m_rotateRightLeftAction->getRelativeValue();
    if (m_rotation != Math::Vector3::Zero) {
        PostChanges(System::Changes::Input::Rotation);
    }*/
}

///
/// @inheritDoc
///
Error CameraInputObject::ChangeOccurred(ISystemObject* systemObject, IObserver::Changes changes) {
    if (changes & Schema::EntityChange::PhysicPosition) {
        /*if (systemObject->getEntity()->getId() == "player") {
            auto subjectPosition = systemObject->getComponent<Schema::Components::PhysicPosition>();
            auto position = getMutableComponent<Schema::Components::PhysicPosition>();
            position->mutate_x(subjectPosition->x());
            position->mutate_y(subjectPosition->y());
            position->mutate_z(subjectPosition->z());
            PostChanges(Schema::EntityChange::PhysicPosition);
        } else {

        }*/
    }
    return Errors::Success;
}
