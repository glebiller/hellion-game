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

#include <windows.h>
#include <OISMouse.h>

#include "Scene.h"
#include "Generic/UObject.h"

///
/// @inheritDo
///
CameraInputObject::CameraInputObject(ISystemScene* pSystemScene, UObject* entity)
    : InputObject(pSystemScene, entity) {
    InputScene* inputScene = GetSystemScene<InputScene>();
    m_rotateUpDownAction = inputScene->getDefaultSchema()->createAction<OISB::AnalogAxisAction>(entity->getName() + "_UpDown");
    m_rotateRightLeftAction = inputScene->getDefaultSchema()->createAction<OISB::AnalogAxisAction>(entity->getName() + "_RightLeft");
}

///
/// @inheritDoc
///
CameraInputObject::~CameraInputObject() {

}

///
/// @inheritDoc
///
Error CameraInputObject::initialize() {


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

    return Errors::Success;
}

///
/// @inheritDoc
///
void CameraInputObject::Update(float DeltaTime) {
    m_rotation.y = m_rotateUpDownAction->getRelativeValue();
    m_rotation.x = m_rotateRightLeftAction->getRelativeValue();
    if (m_rotation != Math::Vector3::Zero) {
        PostChanges(System::Changes::Input::Rotation);
    }

    OIS::Mouse* mouse = OISB::System::getSingleton().getOISMouse();
    int x = mouse->getMouseState().X.abs;
}

///
/// @inheritDoc
///
Error CameraInputObject::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {

    return Errors::Success;
}
