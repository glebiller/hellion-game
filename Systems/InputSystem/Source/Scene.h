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

#include "OISB.h"
#include "System/ISystemScene.h"
#include "System/ISystem.h"
#include "System.h"
#include "Task.h"

class InputSystem;
class InputObject;


/**
 * Implementation of the ISystemScene interface. See Interfaces\System.h for a definition of the
 * class and its functions.
 * 
 * @sa  ISystemScene
 */
class InputScene : public ISystemScene {

    public:

        /**
         * @inheritDoc
         */
        InputScene(ISystem* pSystem);

        /**
         * @inheritDoc
         */
        ~InputScene(void);

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
            return System::Changes::Input::Velocity;
        };

        /**
         * @inheritDoc
         */
        System::Changes::BitMask GetDesiredSystemChanges(void) {
            return System::Changes::None;
        };
        
        /**
         * @inheritDoc
         */
        System::Type GetSystemType(void) {
            return System::Types::Input;
        };
        
        /**
         * @inheritDoc
         */
        ISystemTask* GetSystemTask(void) {
            return m_pInputTask;
        };

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
        OISB::ActionSchema*             m_DefaultSchema;

};
