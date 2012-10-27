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

#include "OISB.h"

#include "BaseTypes.h"
#include "Interface.h"

#include "Scene.h"
#include "Object/Object.h"
#include "Object/MouseObject.h"


#define PINPUTSCENE         reinterpret_cast<InputScene*>(m_pSystemScene)

/**
 * @inheritDoc
 */
InputMouseObject::InputMouseObject(ISystemScene* pSystemScene, const char* pszName) : InputObject(pSystemScene, pszName)
    , m_Modified(0)
    , m_MousePosition(Math::Vector3::Zero) {

}

/**
 * @inheritDoc
 */
InputMouseObject::~InputMouseObject(void) {

}

/**
 * @inheritDoc
 */
Error InputMouseObject::initialize() {
    ASSERT(!m_bInitialized);

    OISB::System::getSingleton().getOISMouse()->setEventCallback(this);

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void InputMouseObject::Update(f32 DeltaTime) {
    if (m_Modified != 0) {
        PostChanges(m_Modified);
        m_Modified = 0;
    }
}

/**
 * @inheritDoc
 */
Error InputMouseObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    return Errors::Success;
}

/**
 * @inheritDoc
 */
bool InputMouseObject::mouseMoved(const OIS::MouseEvent &arg) {
    m_MousePosition.x = arg.state.X.rel * 1.0f;
    m_MousePosition.y = arg.state.Y.rel * 1.0f;
    m_MousePosition.z = arg.state.Z.rel / 120.0f;
    m_Modified |= System::Changes::Input::Mouse;
    return true;
}

/**
 * @inheritDoc
 */
bool InputMouseObject::mousePressed(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    switch (id) {
    case OIS::MouseButtonID::MB_Left:
        m_MouseButtonData.id = IMouseObject::MouseButtonID::LEFT;
        break;
    case OIS::MouseButtonID::MB_Right:
        m_MouseButtonData.id = IMouseObject::MouseButtonID::RIGHT;
        break;
    case OIS::MouseButtonID::MB_Middle:
        m_MouseButtonData.id = IMouseObject::MouseButtonID::MIDDLE;
        break;
    }
    m_MouseButtonData.state = IMouseObject::MouseButtonState::PRESSED;
    m_Modified |= System::Changes::Input::Mouse;
    return true;
}

/**
 * @inheritDoc
 */
bool InputMouseObject::mouseReleased(const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
    switch (id) {
    case OIS::MouseButtonID::MB_Left:
        m_MouseButtonData.id = IMouseObject::MouseButtonID::LEFT;
        break;
    case OIS::MouseButtonID::MB_Right:
        m_MouseButtonData.id = IMouseObject::MouseButtonID::RIGHT;
        break;
    case OIS::MouseButtonID::MB_Middle:
        m_MouseButtonData.id = IMouseObject::MouseButtonID::MIDDLE;
        break;
    }
    m_MouseButtonData.state = IMouseObject::MouseButtonState::RELEASED;
    m_Modified |= System::Changes::Input::Mouse;
    return true;
}
