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
#include "Object/Object.h"
#include "Object/IGeometryObject.h"
#include "Object/IMoveObject.h"

class InputObject;
class InputSystem;
class InputScene;
class InputTask;

/**
 * <c>InputMouseObject</c> Implementation of the ISystemObject interface.
 * This is the Mouse object created objects.
 * 
 * @sa  InputObject
 * @sa  IMoveObject
 */
class InputPlayerObject : public InputObject, public IGeometryObject, public IMoveObject {

    public:

        /**
         * @inheritDoc
         */
        InputPlayerObject(ISystemScene* pSystemScene, const char* pszName);

        /**
         * @inheritDoc
         */
        ~InputPlayerObject(void);
        
        /**
         * @inheritDoc
         */
        Error initialize(void);
        
        /**
         * @inheritDoc
         */
        void Update(f32 DeltaTime);
        
        /**
         * @inheritDoc
         */
        System::Changes::BitMask GetPotentialSystemChanges(void) {
            return System::Changes::Input::Velocity | System::Changes::Geometry::Orientation;
        };

        /**
         * @inheritDoc
         */
        System::Types::BitMask GetDesiredSystemChanges(void) {
            return System::Changes::Geometry::Orientation;
        };

        /**
         * @inheritDoc
         */
        Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        /**
         * @inheritDoc
         */
        virtual const Math::Vector3* GetPosition(void) {
            ASSERT(false);
            return NULL;
        }

        /**
         * @inheritDoc
         */
        virtual const Math::Quaternion* GetOrientation(void) {
            return &m_Orientation;
        }

        /**
         * @inheritDoc
         */
        virtual const Math::Vector3* GetScale(void) {
            ASSERT(false);
            return NULL;
        }

        /**
         * @inheritDoc
         */
        virtual const Math::Vector3* GetVelocity(void) {
            return &m_Velocity;
        }

    private:

        Math::Vector3                       m_Velocity;
        Math::Quaternion                    m_Orientation;

};

