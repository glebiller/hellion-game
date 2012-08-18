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

#include "Object/Object.h"
#include "Object/IGeometryObject.h"
#include "Object/IMoveObject.h"

class InputObject;
class InputSystem;
class InputScene;
class InputTask;

/**
 * <c>InputCameraObject</c> Implementation of the ISystemObject interface. This is the Camera
 * object created objects.
 * 
 * @sa  InputObject
 * @sa  IGeometryObject
 * @sa  IMoveObject
 */
class InputCameraObject : public InputObject, public IGeometryObject, public IMoveObject {

        friend InputSystem;
        friend InputScene;
        friend InputTask;

    protected:

        /**
         * Constructor.
         *
         * @param [in,out]  pSystemScene    If non-null, the system scene.
         * @param   pszName                 The name.
         */
        InputCameraObject(ISystemScene* pSystemScene, const char* pszName);

        /**
         * Destructor.
         */
        ~InputCameraObject(void);

        /////////////////////////////////
        /// ISystemObject overrides
        /////////////////////////////////

        /**
         * Initializes this InputCameraObject.
         *
         * @param   Properties  The properties.
         * @return  .
         */
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /**
         * Gets the properties.
         *
         * @param [in,out]  Properties  The properties.
         */
        virtual void GetProperties(Properties::Array& Properties);

        /**
         * Sets the properties.
         *
         * @param   Properties  The properties.
         */
        virtual void SetProperties(Properties::Array Properties);

        /**
         * Gets the desired system changes.
         *
         * @return  The desired system changes.
         */
        virtual System::Types::BitMask GetDesiredSystemChanges(void) {
            return System::Changes::Geometry::Orientation;
        }

        /**
         * Updates the given DeltaTime.
         *
         * @param   DeltaTime   Time of the delta.
         */
        virtual void Update(f32 DeltaTime);

        /////////////////////////////////
        /// IObserver overrides
        /////////////////////////////////

        /**
         * Change occurred.
         *
         * @param [in,out]  pSubject    If non-null, the subject.
         * @param   ChangeType          Type of the change.
         * @return  .
         */
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        /**
         * Gets the potential system changes.
         *
         * @return  The potential system changes.
         */
        virtual System::Changes::BitMask GetPotentialSystemChanges(void){
            return System::Changes::Geometry::Orientation;
        }

        /////////////////////////////////
        /// IGeometryObject overrides
        /////////////////////////////////

        /**
         * Gets the position.
         * Implementation of the <c>IGeometryObject::GetPosition</c> function.
         *
         * @return  Math::Vector3* - Returns the position for this object.
         *
         * @sa   IGeometryObject::GetPosition    .
         */
        virtual const Math::Vector3* GetPosition(void) {
            ASSERT(false);
            return NULL;
        }

        /**
         * Gets the orientation.
         * Implementation of the <c>IGeometryObject::GetOrientation</c> function.
         *
         * @return  Math::Quaternion* - Returns the orientation quaternion for this object.
         *
         * @sa   IGeometryObject::GetOrientation .
         */
        virtual const Math::Quaternion* GetOrientation(void) {
            return &m_Orientation;
        }

        /**
         * Gets the scale.
         * Implementation of the <c>IGeometryObject::GetScale</c> function.
         *
         * @return  Math::Vector3* - Returns the scale for this object.
         *
         * @sa   IGeometryObject::GetScale   .
         */
        virtual const Math::Vector3* GetScale(void) {
            ASSERT(false);
            return NULL;
        }

        /////////////////////////////////
        /// IMoveObject overrides
        /////////////////////////////////

        /**
         * Gets the velocity.
         *
         * @return  null if it fails, else the velocity.
         */
        virtual const Math::Vector3* GetVelocity(void) {
            return &m_Velocity;
        }

    public:

        enum CommonPropertyTypes {
            Property_None, Property_Count
        };
        static const char*                        sm_kapszCommonPropertyNames[];
        static const Properties::Property   sm_kaCommonDefaultProperties[];

    private:

        Math::Vector3                       m_Velocity;
        Math::Quaternion                    m_Orientation;

};

