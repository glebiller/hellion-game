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


#include <BaseTypes.h>
#include <Interface.h>

#include "Scene.h"
#include "Object/Object.h"
#include "Object/ObjectGui.h"


/////////////////////////////////
/// Static elements
/////////////////////////////////


#define PINPUTSCENE         reinterpret_cast<InputScene*>(m_pSystemScene)

pcstr InputGuiObject::sm_kapszCommonPropertyNames[] = {
    "None"
};

const Properties::Property InputGuiObject::sm_kaCommonDefaultProperties[] = {
    Properties::Property(sm_kapszCommonPropertyNames[ Property_None ],
    Properties::Values::Boolean,
    Properties::Flags::Valid, False),
};


/////////////////////////////////
/// Class Implementation
/////////////////////////////////


InputGuiObject::InputGuiObject(
    ISystemScene* pSystemScene,
    pcstr pszName
)
    : InputObject(pSystemScene, pszName) {
    ASSERT(Property_Count == sizeof sm_kapszCommonPropertyNames / sizeof sm_kapszCommonPropertyNames[ 0 ]);
    ASSERT(Property_Count == sizeof sm_kaCommonDefaultProperties / sizeof sm_kaCommonDefaultProperties[ 0 ]);
}


InputGuiObject::~InputGuiObject(
    void
) {
}


Error
InputGuiObject::Initialize(
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
    m_bInitialized = True;
    //
    // Set the properties for this object.
    //
    SetProperties(Properties);
    return Errors::Success;
}


void
InputGuiObject::GetProperties(
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


void
InputGuiObject::SetProperties(
    Properties::Array Properties
) {
    ASSERT(m_bInitialized);
}


System::Types::BitMask
InputGuiObject::GetDesiredSystemChanges(
    void
) {
    return System::Changes::Geometry::Orientation;
}


void
InputGuiObject::Update(
    f32 DeltaTime
) {
    UNREFERENCED_PARAM(DeltaTime);
}


Error
InputGuiObject::ChangeOccurred(
    ISubject* pSubject,
    System::Changes::BitMask ChangeType
) {
    ASSERT(m_bInitialized);
    return Errors::Success;
}


System::Changes::BitMask
InputGuiObject::GetPotentialSystemChanges(
    void
) {
    return System::Changes::None;
}


