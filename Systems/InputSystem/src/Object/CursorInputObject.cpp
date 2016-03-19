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

#include "Object/CursorInputObject.h"

#include <windows.h>
#include <OISMouse.h>

#include "Scene.h"
#include "Generic/UObject.h"

///
/// @inheritDo
///
CursorInputObject::CursorInputObject(ISystemScene* pSystemScene, UObject* entity)
    : InputObject(pSystemScene, entity) {

}

///
/// @inheritDoc
///
CursorInputObject::~CursorInputObject() {

}

///
/// @inheritDoc
///
Error CursorInputObject::initialize() {


    m_mouse = OISB::System::getSingleton().getOISMouse();
    m_mouse->getMouseState().width = 1024;
    m_mouse->getMouseState().height = 728;

    return Errors::Success;
}

///
/// @inheritDoc
///
void CursorInputObject::Update(float DeltaTime) {
    OIS::Mouse* mouse = OISB::System::getSingleton().getOISMouse();
    m_velocity.x = mouse->getMouseState().X.abs;
    m_velocity.y = mouse->getMouseState().Y.abs;
    //if (m_velocity != Math::Vector3::Zero) {
        // TODO change
        //PostChanges(System::Changes::Input::Velocity);
    //}
    
    if (mouse->getMouseState().buttonDown(OIS::MB_Left)) {
        //PostChanges(System::Changes::Input::Action);
    }

}

///
/// @inheritDoc
///
Error CursorInputObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {

    return Errors::Success;
}
