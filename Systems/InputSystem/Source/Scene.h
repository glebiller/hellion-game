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

        InputScene(ISystem* pSystem);
        ~InputScene(void);

        /////////////////////////////////
        /// ISystemScene overrides
        /////////////////////////////////

        virtual System::Type GetSystemType(void);

        virtual Error Initialize(std::vector<Properties::Property> Properties);

        virtual void GetProperties(Properties::Array& Properties);

        virtual void SetProperties(Properties::Array Properties);

        virtual pcstr* GetObjectTypes(void);

        virtual ISystemObject* CreateObject(pcstr pszName, pcstr pszType);

        virtual Error DestroyObject(ISystemObject* pSystemObject);

        virtual ISystemTask* GetSystemTask(void);

        virtual void Update(f32 DeltaTime);

        /////////////////////////////////
        /// ISubject overrides
        /////////////////////////////////

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
        };
        InputActions                        m_InputActions;

    protected:

        InputTask*                      m_pInputTask;
        std::list<InputObject*>         m_Objects;
        OISB::ActionSchema*             m_DefaultSchema;

};
