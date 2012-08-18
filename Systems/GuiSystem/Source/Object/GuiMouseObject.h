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

#include "Object/GuiObject.h"
#include "Property.h"
#include "System.h"

class ISystemScene;
class ISubject;

/**
 * Implementation of the IGraphicsObject interface. See Interfaces\Graphics.h and Interfaces\
 * System.h for a definition of the class and its functions.
 * 
 * @sa  GuiObject
 */
class GuiMouseObject : public GuiObject {

    public:

        /**
         * Constructor.
         *
         * @param [in,out]  pSystemScene    If non-null, the system scene.
         * @param   pszName                 The name.
         */
        GuiMouseObject(ISystemScene* pSystemScene, const char* pszName);

        /**
         * Destructor.
         */
        ~GuiMouseObject(void);

        /////////////////////////////////
        /// ISystemObject overrides
        /////////////////////////////////

        /**
         * Initializes this GuiObject.
         * Implementation of the <c>ISystem::Initialize</c> function.
         *
         * @param   Properties  Initializes the object with the properties specified by
         *                      <paramref name="Properties"/>.
         * @return  Error.
         *
         * @sa   ISystem::Initialize .
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
         * Implementation of the <c>IGeometryObject::GetDesiredSystemChanges</c> function.
         *
         * @return  System::Types::BitMask - System changes desired by the object.
         *
         * @sa   ISystemObject::GetSystemType    .
         */
        virtual System::Types::BitMask GetDesiredSystemChanges(void) {
            return System::Changes::Input::Velocity;
        }
        
        /**
         * Updates the given DeltaTime.
         * Implementation of the <c>ISystemTask::Update</c> method.
         *
         * @param   DeltaTime   Elapsed time since the last frame.
         *
         * @sa   ISystemTask::Update .
         */
        virtual void Update(f32 DeltaTime);
        
        /////////////////////////////////
        /// IObserver overrides
        /////////////////////////////////

        /**
         * Change occurred.
         * Implementation of the <c>IObserver::ChangeOccurred</c> function.
         *
         * @param   pSubject    Subject of this notification.
         * @param   ChangeType  Type of notification for this object.
         * @return  Error.
         *
         * @sa   IObserver::ChangeOccurred   .
         */
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        /**
         * Gets the potential system changes.
         * Implementation of the <c>ISubject::GetPotentialSystemChanges</c> function.
         *
         * @return  System::Changes::BitMask - Returns systems changes possible for this object.
         *
         * @sa   ISubject::GetPotentialSystemChanges .
         */
        virtual System::Changes::BitMask GetPotentialSystemChanges(void) {
            return System::Changes::None;
        }


    private:


};

