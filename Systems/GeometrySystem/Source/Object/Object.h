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

#include "Proto.h"
#include "System.h"
#include "System/ISystemObject.h"

class GeometrySystem;
class GeometryScene;

class GeometryObject : public ISystemObject, public IGeometryObject {

    public:

        /**
         * @inheritDoc
         */
        GeometryObject(ISystemScene* pSystemScene);

        /**
         * @inheritDoc
         */
        ~GeometryObject(void);

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
        System::Type GetSystemType(void) {
            return System::Types::Gui;
        }

        /**
         * @inheritDoc
         */
        System::Changes::BitMask GetPotentialSystemChanges(void) {
            return System::Changes::Geometry::All;
        };

        /**
         * @inheritDoc
         */
        System::Types::BitMask GetDesiredSystemChanges(void) {
            return System::Changes::Physics::Position | System::Changes::Geometry::All
        };

        /**
         * @inheritDoc
         */
        Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        /**
         * @inheritDoc
         */
        const Math::Vector3* GetPosition(void) {
            return &m_Position;
        }

        /**
         * @inheritDoc
         */
        const Math::Quaternion* GetOrientation(void) {
            return &m_Orientation;
        }

        /**
         * @inheritDoc
         */
        const Math::Vector3* GetScale(void) {
            return &m_Scale;
        }

    protected:

        void setPosition(ProtoStringList values);

        void setOrientation(ProtoStringList values);

        void setScale(ProtoStringList values);

    private:

        Math::Vector3                   m_Position;
        Math::Quaternion                m_Orientation;
        Math::Vector3                   m_Scale;

};
