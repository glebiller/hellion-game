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

#include "System.h"
#include "Object/PhysicObject.h"
#include "Object/IMoveObject.h"

class ISystemScene;

/**
* Implementation of the IGraphicsObject interface. See Interfaces\Graphics.h and Interfaces\
* System.h for a definition of the class and its functions.
*
* @sa  ISystemObject
*/
class MovablePhysicObject : public PhysicObject, public IMoveObject {
public:

    /**
     * @inheritDoc
     */
    MovablePhysicObject(ISystemScene* pSystemScene, UObject* entity);

    /**
     * @inheritDoc
     */
    virtual ~MovablePhysicObject();

    /**
     * @inheritDoc
     */
    Error initialize();

    /**
     * @inheritDoc
     */
    System::Changes::BitMask GetPotentialSystemChanges() {
        return System::Changes::Physic::Position | System::Changes::Physic::Orientation;
    };

    /**
     * @inheritDoc
     */
    System::Types::BitMask GetDesiredSystemChanges() {
        return System::Changes::Input::Velocity | System::Changes::Input::Rotation;
    };

    /**
     * @inheritDoc
     */
    Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

    /**
     * @inheritDoc
     */
    void Update(float DeltaTime);

private:
    bool    m_constraint_position;
    float     m_velocity_multiplier;
    float     m_rotation_multiplier;

};
