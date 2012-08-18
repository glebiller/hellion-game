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


#include <queue>
#include <list>
#include <string>

#include "Debug.h"
#include "Interface.h"

#include "Object/Object.h"


/////////////////////////////////
/// Static elements
/////////////////////////////////


const char* InputObject::sm_kapszTypeNames[] = {
    "Controlled", "Overlay",
    NULL
};

const char* InputObject::sm_kapszPropertyNames[] = {
    "FKey", "Orientation"
};

const Properties::Property InputObject::sm_kaDefaultProperties[] = {
    Properties::Property(sm_kapszPropertyNames[ Property_FKey ],
    VALUE1(Properties::Values::Int32),
    Properties::Flags::Valid,
    NULL, NULL, NULL, NULL,
    0),

    Properties::Property(sm_kapszPropertyNames[ Property_Orientation ],
    VALUE1x3(Properties::Values::Float32),
    Properties::Flags::Valid,
    NULL, NULL, NULL, NULL,
    0)
};


/////////////////////////////////
/// Class Implementation
/////////////////////////////////


InputObject::InputObject(ISystemScene* pSystemScene, const char* pszName)
    : ISystemObject(pSystemScene, NULL) {
    ASSERT(Property_Count == sizeof sm_kapszPropertyNames / sizeof sm_kapszPropertyNames[ 0 ]);
    ASSERT(Property_Count == sizeof sm_kaDefaultProperties / sizeof sm_kaDefaultProperties[ 0 ]);
    m_sName = pszName;
}


InputObject::~InputObject(void) {
}


System::Type InputObject::GetSystemType(void) {
    return System::Types::Input;
}


Error InputObject::Initialize(std::vector<Properties::Property> Properties) {
    //
    // Set this set as initialized.
    //
    m_bInitialized = true;
    SetProperties(Properties);
    return Errors::Success;
}


void InputObject::GetProperties(Properties::Array& Properties) {
    //
    // Get the index of our first item.
    //
    i32 iProperty = static_cast<i32>(Properties.size());
    //
    // Add all the properties.
    //
    Properties.reserve(Properties.size() + Property_Count);

    for (i32 i = 0; i < Property_Count; i++) {
        Properties.push_back(sm_kaDefaultProperties[ i ]);
    }
}


void InputObject::SetProperties(Properties::Array Properties) {
    ASSERT(m_bInitialized);

    //
    // Read in the properties.
    //
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();
            ASSERT(false);
            //
            // Set this property to invalid since it's already been read.
            //
            it->ClearFlag(Properties::Flags::Valid);
        }
    }
}


System::Types::BitMask InputObject::GetDesiredSystemChanges(void) {
    return System::Changes::None;
}


Error InputObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    return Errors::Success;
}

