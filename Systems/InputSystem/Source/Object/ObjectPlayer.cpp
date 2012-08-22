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

#include "BaseTypes.h"
#include "Interface.h"

#include "Scene.h"
#include "Object/Object.h"
#include "Object/ObjectPlayer.h"

#define PINPUTSCENE         reinterpret_cast<InputScene*>(m_pSystemScene)


/**
 * @inheritDoc
 */
InputPlayerObject::InputPlayerObject(ISystemScene* pSystemScene, const char* pszName) : InputObject(pSystemScene, pszName)
    , m_Velocity(Math::Vector3::Zero) {

}

/**
 * @inheritDoc
 */
InputPlayerObject::~InputPlayerObject(void) {

}

/**
 * @inheritDoc
 */
Error InputPlayerObject::initialize() {
    ASSERT(!m_bInitialized);
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void InputPlayerObject::Update(f32 DeltaTime) {
    u32 mModified = 0;
    InputScene* pScene = static_cast<InputScene*>(m_pSystemScene);

    if (pScene->m_InputActions.MoveForward->hasChanged() || pScene->m_InputActions.MoveBackward->hasChanged()) {
        m_Velocity.z = 0;
        if (pScene->m_InputActions.MoveForward->isActive()) {
            m_Velocity.z += 1.0;
        }
        if (pScene->m_InputActions.MoveBackward->isActive()) {
            m_Velocity.z -= 1.0;
        }
        mModified |= System::Changes::Input::Velocity;
    }
    
    if (pScene->m_InputActions.StrafeLeft->hasChanged() || pScene->m_InputActions.StrafeRight->hasChanged()) {
        m_Velocity.x = 0;
        if (pScene->m_InputActions.StrafeLeft->isActive()) {
            m_Velocity.x += 1.0;
        }
        if (pScene->m_InputActions.StrafeRight->isActive()) {
            m_Velocity.x -= 1.0;
        }
        mModified |= System::Changes::Input::Velocity;
    }

    if (pScene->m_InputActions.Jump->hasChanged()) {
        m_Velocity.y = pScene->m_InputActions.Jump->isActive() ? 1.0f : 0.0f;
        mModified |= System::Changes::Input::Velocity;
    }

    m_Orientation.Set(Math::Vector3::UnitY, pScene->m_InputActions.RotateRightLeft->getAbsoluteValue());
    mModified |= System::Changes::Geometry::Orientation;

    PostChanges(mModified);
}

/**
 * @inheritDoc
 */
Error InputPlayerObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    return Errors::Success;
}
