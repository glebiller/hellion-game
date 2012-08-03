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

class InputObject;
class InputSystem;
class InputScene;
class InputTask;

///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   <c>InputCameraObject</c> Implementation of the ISystemObject interface.
///   This is the Camera object created objects.
/// </summary>
///////////////////////////////////////////////////////////////////////////////
class InputCameraObject : public InputObject, public IGeometryObject, public IMoveObject {
        friend InputSystem;
        friend InputScene;
        friend InputTask;

    protected:

        InputCameraObject(ISystemScene* pSystemScene, pcstr pszName);
        ~InputCameraObject(void);

        /////////////////////////////////
        /// ISystemObject overrides
        /////////////////////////////////

        virtual Error Initialize(std::vector<Properties::Property> Properties);

        virtual void GetProperties(Properties::Array& Properties);

        virtual void SetProperties(Properties::Array Properties);

        virtual System::Types::BitMask GetDesiredSystemChanges(void);

        virtual void Update(f32 DeltaTime);

        /////////////////////////////////
        /// IObserver overrides
        /////////////////////////////////

        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        virtual System::Changes::BitMask GetPotentialSystemChanges(void);

        /////////////////////////////////
        /// IGeometryObject overrides
        /////////////////////////////////

        /// <summary cref="HavokObject::GetPosition">
        ///   Implementation of the <c>IGeometryObject::GetPosition</c> function.
        /// </summary>
        /// <returns>Math::Vector3* - Returns the position for this object.</returns>
        /// <seealso cref="IGeometryObject::GetPosition"/>
        virtual const Math::Vector3* GetPosition(void);

        /// <summary cref="HavokObject::GetOrientation">
        ///   Implementation of the <c>IGeometryObject::GetOrientation</c> function.
        /// </summary>
        /// <returns>Math::Quaternion* - Returns the orientation quaternion for this object.</returns>
        /// <seealso cref="IGeometryObject::GetOrientation"/>
        virtual const Math::Quaternion* GetOrientation(void);

        /// <summary cref="HavokObject::GetScale">
        ///   Implementation of the <c>IGeometryObject::GetScale</c> function.
        /// </summary>
        /// <returns>Math::Vector3* - Returns the scale for this object.</returns>
        /// <seealso cref="IGeometryObject::GetScale"/>
        virtual const Math::Vector3* GetScale(void);

        /////////////////////////////////
        /// IMoveObject overrides
        /////////////////////////////////

        virtual const Math::Vector3* GetVelocity(void);

    public:

        enum CommonPropertyTypes {
            Property_None, Property_Count
        };
        static pcstr                        sm_kapszCommonPropertyNames[];
        static const Properties::Property   sm_kaCommonDefaultProperties[];

    private:

        Math::Vector3                       m_Velocity;
        Math::Quaternion                    m_Orientation;

};

