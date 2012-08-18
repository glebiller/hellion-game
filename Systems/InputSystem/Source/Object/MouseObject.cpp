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
#include "Object/MouseObject.h"


/////////////////////////////////
/// Static elements
/////////////////////////////////


#define PINPUTSCENE         reinterpret_cast<InputScene*>(m_pSystemScene)

const char* InputMouseObject::sm_kapszCommonPropertyNames[] = {
    "None"
};

const Properties::Property InputMouseObject::sm_kaCommonDefaultProperties[] = {
    Properties::Property(sm_kapszCommonPropertyNames[ Property_None ],
    Properties::Values::Boolean,
    Properties::Flags::Valid, false),
};


/////////////////////////////////
/// Class implementation
/////////////////////////////////


InputMouseObject::InputMouseObject(
    ISystemScene* pSystemScene,
    const char* pszName
)
    : InputObject(pSystemScene, pszName)
    , m_Velocity(Math::Vector3::Zero) {
    ASSERT(Property_Count == sizeof sm_kapszCommonPropertyNames / sizeof sm_kapszCommonPropertyNames[ 0 ]);
    ASSERT(Property_Count == sizeof sm_kaCommonDefaultProperties / sizeof sm_kaCommonDefaultProperties[ 0 ]);
}

///////////////////////////////////////////////////////////////////////////////
InputMouseObject::~InputMouseObject(
    void
) {
}

///////////////////////////////////////////////////////////////////////////////
Error
InputMouseObject::Initialize(
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
InputMouseObject::GetProperties(
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
void InputMouseObject::SetProperties(Properties::Array Properties) {
    ASSERT(m_bInitialized);
}


///////////////////////////////////////////////////////////////////////////////
void InputMouseObject::Update(f32 DeltaTime) {
    u32 mModified = 0;
    InputScene* pScene = static_cast<InputScene*>(m_pSystemScene);
    
    //if (pScene->m_InputActions.MouseRightLeft->hasChanged()) {
        m_Velocity.x = pScene->m_InputActions.MouseRightLeft->getRelativeValue();
        m_Velocity.y = pScene->m_InputActions.MouseUpDown->getRelativeValue();
        mModified |= System::Changes::Input::Velocity;
        PostChanges(mModified);
    //}
}

///////////////////////////////////////////////////////////////////////////////
Error InputMouseObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    return Errors::Success;
}



