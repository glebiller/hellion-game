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

//
// core includes
//
#include "BaseTypes.h"
#include "Interface.h"

//
// system includes
//
#include "Object/Object.h"


///////////////////////////////////////////////////////////////////////////////
// HavokObject - Constructor
HavokObject::HavokObject(
    ISystemScene* pSystemScene,
    const char* pszName
)
    : ISystemObject(pSystemScene, pszName)
    , m_Position(Math::Vector3::Zero)
    , m_Orientation(Math::Quaternion::Zero)
    , m_Scale(Math::Vector3::One) {
}


///////////////////////////////////////////////////////////////////////////////
// ~HavokObject - Destructor
HavokObject::~HavokObject(
    void
) {
}


///////////////////////////////////////////////////////////////////////////////
// GetSystemType - Returns System type for this Object
SystemProto::Type HavokObject::GetSystemType(void) {
    return SystemProto::Physic;
}


///////////////////////////////////////////////////////////////////////////////
// GetPosition - Returns the position of this Object
const Math::Vector3*
HavokObject::GetPosition(
    void
) {
    return &m_Position;
}


///////////////////////////////////////////////////////////////////////////////
// GetPosition - Returns the orientation of this Object
const Math::Quaternion*
HavokObject::GetOrientation(
    void
) {
    return &m_Orientation;
}


///////////////////////////////////////////////////////////////////////////////
// GetScale - Returns the scale of this Object
const Math::Vector3*
HavokObject::GetScale(
    void
) {
    ASSERT(false);
    return NULL;
}
