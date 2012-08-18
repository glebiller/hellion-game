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

#include "System/ISystemObject.h"
#include "System.h"

class GuiSystem;
class GuiScene;

///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   Implementation of the IGraphicsObject interface.
///   See Interfaces\Graphics.h and Interfaces\System.h for a definition of the
///   class and its functions.
/// </summary>
///////////////////////////////////////////////////////////////////////////////
class GuiObject : public ISystemObject {

        friend GuiScene;

    public:

        enum Types {
            Type_Generic
        };

        /**
         * Gets the type.
         * Returns the type of graphics object.
         *
         * @return  GuiObject::Types - Type object.
         */
        Types GetType(void) {
            return m_Type;
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

    protected:

        /**
         * Constructor.
         *
         * @param [in,out]  pSystemScene    If non-null, the system scene.
         * @param   pszName                 The name.
         */
        GuiObject(ISystemScene* pSystemScene, const char* pszName);

        /**
         * Destructor.
         */
        ~GuiObject(void);

        /**
         * Gets the system type.
         * Implementation of the <c>ISystemObject::GetSystemType</c> function. Lets this object know
         * when a registered aspects of interest has changed (this function will be called when other
         * systems make changes this object should know about).
         *
         * @return  System::Type - Type of this system.
         *
         * @sa   ISystemObject::GetSystemType    .
         */
        virtual System::Type GetSystemType(void) {
            return System::Types::Gui;
        }

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
        virtual System::Changes::BitMask GetPotentialSystemChanges(void){
            return System::Changes::None;
        }


    protected:
        
        static const char*                  sm_kapszTypeNames[];
        static const char*                  sm_kapszPropertyNames[];
        static const Properties::Property   sm_kaDefaultProperties[];
        
        const char*                         m_pszName;
        Types                               m_Type;

};

