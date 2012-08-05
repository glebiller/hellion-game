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


/////////////////////////////////
/// Includes
/////////////////////////////////


#include "BaseTypes.h"
#include "Interface.h"

#include "Scene.h"
#include "Object/Object.h"
#include "Object/ObjectPlayer.h"


/////////////////////////////////
/// Static elements
/////////////////////////////////


#define PINPUTSCENE         reinterpret_cast<InputScene*>(m_pSystemScene)

const char* InputPlayerObject::sm_kapszCommonPropertyNames[] = {
    "None"
};

const Properties::Property InputPlayerObject::sm_kaCommonDefaultProperties[] = {
    Properties::Property(sm_kapszCommonPropertyNames[ Property_None ],
    Properties::Values::Boolean,
    Properties::Flags::Valid, false),
};


/////////////////////////////////
/// Class implementation
/////////////////////////////////


InputPlayerObject::InputPlayerObject(
    ISystemScene* pSystemScene,
    const char* pszName
)
    : InputObject(pSystemScene, pszName)
    , m_Velocity(Math::Vector3::Zero) {
    ASSERT(Property_Count == sizeof sm_kapszCommonPropertyNames / sizeof sm_kapszCommonPropertyNames[ 0 ]);
    ASSERT(Property_Count == sizeof sm_kaCommonDefaultProperties / sizeof sm_kaCommonDefaultProperties[ 0 ]);
}

///////////////////////////////////////////////////////////////////////////////
InputPlayerObject::~InputPlayerObject(
    void
) {
}

///////////////////////////////////////////////////////////////////////////////
Error
InputPlayerObject::Initialize(
    std::vector<Properties::Property> Properties
) {
    ASSERT(!m_bInitialized);

    //
    // Read in the properties.
    //
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();
        }
    }

    //
    // Set this as initialized.
    //
    m_bInitialized = true;
    //
    // Set the properties for this object.
    //
    SetProperties(Properties);
    return Errors::Success;
}

///////////////////////////////////////////////////////////////////////////////
void
InputPlayerObject::GetProperties(
    Properties::Array& Properties
) {
    //
    // Get the index of our first item.
    //
    i32 iProperty = static_cast<i32>(Properties.size());
    //
    // Add the common properties.
    //
    Properties.reserve(Properties.size() + Property_Count);

    for (i32 i = 0; i < Property_Count; i++) {
        Properties.push_back(sm_kaCommonDefaultProperties[ i ]);
    }
}

///////////////////////////////////////////////////////////////////////////////
void
InputPlayerObject::SetProperties(
    Properties::Array Properties
) {
    ASSERT(m_bInitialized);
}

///////////////////////////////////////////////////////////////////////////////
System::Types::BitMask
InputPlayerObject::GetDesiredSystemChanges(
    void
) {
    return System::Changes::Geometry::Orientation;
}

///////////////////////////////////////////////////////////////////////////////
void
InputPlayerObject::Update(
    f32 DeltaTime
) {
    u32 mModified = 0;
    InputScene* pScene = static_cast<InputScene*>(m_pSystemScene);

    if (pScene->m_InputActions.MoveForward->hasChanged()) {
        m_Velocity.z = pScene->m_InputActions.MoveForward->isActive() ? 1.0f : 0.0f;
        mModified |= System::Changes::Input::Velocity;
    }

    if (pScene->m_InputActions.MoveBackward->hasChanged()) {
        m_Velocity.z = pScene->m_InputActions.MoveBackward->isActive() ? -1.0f : 0.0f;
        mModified |= System::Changes::Input::Velocity;
    }

    if (pScene->m_InputActions.StrafeLeft->hasChanged()) {
        m_Velocity.x = pScene->m_InputActions.StrafeLeft->isActive() ? 1.0f : 0.0f;
        mModified |= System::Changes::Input::Velocity;
    }

    if (pScene->m_InputActions.StrafeRight->hasChanged()) {
        m_Velocity.x = pScene->m_InputActions.StrafeRight->isActive() ? -1.0f : 0.0f;
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

///////////////////////////////////////////////////////////////////////////////
Error
InputPlayerObject::ChangeOccurred(
    ISubject* pSubject,
    System::Changes::BitMask ChangeType
) {
    ASSERT(m_bInitialized);
    return Errors::Success;
}

///////////////////////////////////////////////////////////////////////////////
System::Changes::BitMask
InputPlayerObject::GetPotentialSystemChanges(
    void
) {
    return System::Changes::Input::Velocity | System::Changes::Geometry::Orientation;
}

///////////////////////////////////////////////////////////////////////////////
const Math::Vector3*
InputPlayerObject::GetVelocity(
    void
) {
    return &m_Velocity;
}

///////////////////////////////////////////////////////////////////////////////
const Math::Vector3*
InputPlayerObject::GetPosition(
    void
) {
    ASSERT(false);
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
const Math::Quaternion*
InputPlayerObject::GetOrientation(
    void
) {
    return &m_Orientation;
}


///////////////////////////////////////////////////////////////////////////////
const Math::Vector3*
InputPlayerObject::GetScale(
    void
) {
    ASSERT(false);
    return NULL;
}




