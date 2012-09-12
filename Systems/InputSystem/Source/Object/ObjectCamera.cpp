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
#include "Object/ObjectCamera.h"

#define PINPUTSCENE         reinterpret_cast<InputScene*>(m_pSystemScene)

/**
 * @inheritDoc
 */
InputCameraObject::InputCameraObject(ISystemScene* pSystemScene, const char* pszName) : InputObject(pSystemScene, pszName)
    , m_Velocity(Math::Vector3::Zero) {

}

/**
 * @inheritDoc
 */
InputCameraObject::~InputCameraObject(void) {

}

/**
 * @inheritDoc
 */
Error InputCameraObject::initialize() {
    ASSERT(!m_bInitialized);
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void InputCameraObject::Update(f32 DeltaTime) {
    u32 mModified = 0;
    InputScene* pScene = static_cast<InputScene*>(m_pSystemScene);
    
    m_Orientation.y = pScene->m_InputActions.CameraRotateUpDown->getAbsoluteValue();
    m_Orientation.x = pScene->m_InputActions.CameraRotateRightLeft->getAbsoluteValue();
    mModified |= System::Changes::Geometry::Orientation;

    PostChanges(mModified);
}

/**
 * @inheritDoc
 */
Error InputCameraObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    return Errors::Success;
}
