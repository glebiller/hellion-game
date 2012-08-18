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


class InputSystem;
class InputTask;
class InputObject;


#include "OISB.h"


class InputScene : public ISystemScene {
        friend class InputSystem;
        friend class InputTask;

    protected:

        /**
         * Constructor.
         *
         * @param [in,out]  pSystem If non-null, the system.
         */
        InputScene(ISystem* pSystem);

        /**
         * Destructor.
         */
        ~InputScene(void);

        /////////////////////////////////
        /// ISystemScene overrides
        /////////////////////////////////

        /**
         * Gets the system type.
         *
         * @return  The system type.
         */
        virtual System::Type GetSystemType(void);

        /**
         * Initializes this InputScene.
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
         * Gets the object types.
         *
         * @return  null if it fails, else the object types.
         */
        virtual const char** GetObjectTypes(void);

        /**
         * Creates an object.
         *
         * @param   pszName The name.
         * @param   pszType The type.
         * @return  The new object.
         */
        virtual ISystemObject* CreateObject(const char* pszName, const char* pszType);

        /**
         * Destroys the object described by pSystemObject.
         *
         * @param [in,out]  pSystemObject   If non-null, the system object.
         * @return  .
         */
        virtual Error DestroyObject(ISystemObject* pSystemObject);

        /**
         * Gets the system task.
         *
         * @return  null if it fails, else the system task.
         */
        virtual ISystemTask* GetSystemTask(void);

        /**
         * Updates the given DeltaTime.
         *
         * @param   DeltaTime   Time of the delta.
         */
        virtual void Update(f32 DeltaTime);

        /////////////////////////////////
        /// ISubject overrides
        /////////////////////////////////

        /**
         * Gets the potential system changes.
         *
         * @return  The potential system changes.
         */
        virtual System::Changes::BitMask GetPotentialSystemChanges(void);

    public:

        struct InputActions {
            OISB::TriggerAction*            Exit;
            OISB::TriggerAction*            MoveForward;
            OISB::TriggerAction*            MoveBackward;
            OISB::TriggerAction*            TurnLeft;
            OISB::TriggerAction*            TurnRight;
            OISB::TriggerAction*            StrafeLeft;
            OISB::TriggerAction*            StrafeRight;
            OISB::TriggerAction*            Jump;

            OISB::AnalogAxisAction*         RotateRightLeft;
            OISB::AnalogAxisAction*         RotateUpsDown;
            
            OISB::AnalogAxisAction*         CameraRotateUpDown;
            OISB::AnalogAxisAction*         CameraRotateRightLeft;
            
            OISB::AnalogAxisAction*         MouseUpDown;
            OISB::AnalogAxisAction*         MouseRightLeft;
        };
        InputActions                        m_InputActions;

    protected:

        InputTask*                      m_pInputTask;
        std::list<InputObject*>         m_Objects;
        OISB::ActionSchema*             m_DefaultSchema;

};
