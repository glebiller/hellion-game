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

#pragma once

#include "Object/PhysicObject.h"

class ISystemScene;
class hkpCharacterProxy;
class hkpCharacterContext;
class IgnorePhantomOverlapListener;

///
/// <c>CharacterPhysicObject</c> Implementation of the ISystemObject interface. This is the
/// Character object created objects using Havok Character Proxy.
///
/// @sa PhysicObject
///
class CharacterPhysicObject : public PhysicObject {
public:
    CharacterPhysicObject(ISystemScene* pSystemScene, UObject* entity);

    ~CharacterPhysicObject();
    
    ///
    /// @inheritDoc.
    ///
    Error initialize();

    ///
    /// @inheritDoc.
    ///
    System::Changes::BitMask GetPotentialSystemChanges() {
        return System::Changes::Physic::Position | System::Changes::Physic::Orientation;
    };
    
    ///
    /// @inheritDoc.
    ///
    System::Types::BitMask GetDesiredSystemChanges() {
        return System::Changes::Input::Velocity | System::Changes::Input::Rotation;
    };
    
    ///
    /// @inheritDoc.
    ///
    Error ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType);
    
    ///
    /// @inheritDoc.
    ///
    void Update(float DeltaTime);

private:
    hkpCharacterProxy*              m_CharacterProxy;
    hkpCharacterContext*            m_characterContext;

    float                             m_Radius;

};
