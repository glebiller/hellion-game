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

#include "boost/functional/factory.hpp"

#include "BaseTypes.h"
#include "Interface.h"
#include "OISB.h"

#include "System.h"
#include "Scene.h"
#include "Object/Object.h"
#include "Object/ObjectCamera.h"
#include "Object/ObjectGui.h"
#include "Object/ObjectPlayer.h"
#include "Object/MouseObject.h"
#include "Task.h"


extern ManagerInterfaces   g_Managers;


/**
 * @inheritDoc
 */
InputScene::InputScene(ISystem* pSystem) : ISystemScene(pSystem)
    , m_pInputTask(NULL)
    , m_DefaultSchema(NULL) {
    //
    // Fill the object factories
    // 
    m_ObjectFactories["Player"] = boost::factory<InputPlayerObject*>();
    m_ObjectFactories["Camera"] = boost::factory<InputCameraObject*>();
    m_ObjectFactories["Mouse"] = boost::factory<InputMouseObject*>();
    m_ObjectFactories["Gui"] = boost::factory<InputGuiObject*>();

    //
    // Fill the properties default values
    // 
}

/**
 * @inheritDoc
 */
InputScene::~InputScene(void) {
    SAFE_DELETE(m_pInputTask);
}

/**
 * @inheritDoc
 */
Error InputScene::initialize(void) {
    ASSERT(!m_bInitialized);

    m_pInputTask = new InputTask(this);
    ASSERT(m_pInputTask != NULL);
    if (m_pInputTask == NULL) {
        return Errors::Failure;
    }

    m_DefaultSchema = OISB::System::getSingleton().getDefaultActionSchemaAutoCreate();
    // Exit
    m_InputActions.Exit = m_DefaultSchema->createAction<OISB::TriggerAction>("Exit");
    m_InputActions.Exit->bind("Keyboard/ECHAP");
    // Move Forward
    m_InputActions.MoveForward = m_DefaultSchema->createAction<OISB::TriggerAction>("MoveForward");
    m_InputActions.MoveForward->bind("Keyboard/Z");
    // Move Backward
    m_InputActions.MoveBackward = m_DefaultSchema->createAction<OISB::TriggerAction>("MoveBackward");
    m_InputActions.MoveBackward->bind("Keyboard/S");
    // Strafe Left
    m_InputActions.StrafeLeft = m_DefaultSchema->createAction<OISB::TriggerAction>("StrafeLeft");
    m_InputActions.StrafeLeft->bind("Keyboard/A");
    m_InputActions.StrafeLeft->bind("Keyboard/Q", "Mouse/Left Button");
    // Strafe Right
    m_InputActions.StrafeRight = m_DefaultSchema->createAction<OISB::TriggerAction>("StrafeRight");
    m_InputActions.StrafeRight->bind("Keyboard/E");
    m_InputActions.StrafeRight->bind("Keyboard/D", "Mouse/Left Button");
    // Turn Right / Left
    m_InputActions.RotateRightLeft = m_DefaultSchema->createAction<OISB::AnalogAxisAction>("RotateRightLeft");
    m_InputActions.RotateRightLeft->setProperty("UseCircularValues", true);
    m_InputActions.RotateRightLeft->setProperty("MinimumValue", 0.0f);
    m_InputActions.RotateRightLeft->setProperty("MaximumValue", 2.0f * Math::Angle::Pi);
    m_InputActions.RotateRightLeft->setProperty("EmulationSpeed", 2.0f);
    m_InputActions.RotateRightLeft->setProperty("EmulationReturnSpeed", 0.0f);
    m_InputActions.RotateRightLeft->bind("Keyboard/D", "Keyboard/Q");
    // Jump
    m_InputActions.Jump = m_DefaultSchema->createAction<OISB::TriggerAction>("Jump");
    m_InputActions.Jump->bind("Keyboard/ESPACE");

    // Camera Up / Down
    m_InputActions.CameraRotateUpDown = m_DefaultSchema->createAction<OISB::AnalogAxisAction>("CameraRotateUpDown");
    m_InputActions.CameraRotateUpDown->setProperty("UseCircularValues", true);
    m_InputActions.CameraRotateUpDown->setProperty("MinimumValue", 0.0f);
    m_InputActions.CameraRotateUpDown->setProperty("MaximumValue", 2.0f * Math::Angle::Pi);
    m_InputActions.CameraRotateUpDown->setProperty("EmulationSpeed", 2.0f);
    m_InputActions.CameraRotateUpDown->setProperty("EmulationReturnEnabled", false);
    m_InputActions.CameraRotateUpDown->setProperty("Sensitivity", 0.001f);
    m_InputActions.CameraRotateUpDown->bind("Mouse/Y Axis");
    // Camera Right / Left
    m_InputActions.CameraRotateRightLeft = m_DefaultSchema->createAction<OISB::AnalogAxisAction>("CameraRotateRightLeft");
    m_InputActions.CameraRotateRightLeft->setProperty("UseCircularValues", true);
    m_InputActions.CameraRotateRightLeft->setProperty("MinimumValue", 0.0f);
    m_InputActions.CameraRotateRightLeft->setProperty("MaximumValue", 2.0f * Math::Angle::Pi);
    m_InputActions.CameraRotateRightLeft->setProperty("EmulationSpeed", 2.0f);
    m_InputActions.CameraRotateRightLeft->setProperty("EmulationReturnEnabled", false);
    m_InputActions.CameraRotateRightLeft->setProperty("Sensitivity", 0.001f);
    m_InputActions.CameraRotateRightLeft->bind("Mouse/X Axis");

    // Mouse Up / Down
    m_InputActions.MouseUpDown = m_DefaultSchema->createAction<OISB::AnalogAxisAction>("MouseUpDown");
    m_InputActions.MouseUpDown->setProperty("EmulationReturnEnabled", false);
    m_InputActions.MouseUpDown->setProperty("Sensitivity", 1.0f);
    m_InputActions.MouseUpDown->bind("Mouse/Y Axis");
    // Mouse Right / Left
    m_InputActions.MouseRightLeft = m_DefaultSchema->createAction<OISB::AnalogAxisAction>("MouseRightLeft");
    m_InputActions.MouseRightLeft->setProperty("EmulationReturnEnabled", false);
    m_InputActions.MouseRightLeft->setProperty("Sensitivity", 1.0f);
    m_InputActions.MouseRightLeft->bind("Mouse/X Axis");

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void InputScene::Update(f32 DeltaTime) {
    ObjectsList Objects = m_pObjects;

    // Global inputs
    if (m_InputActions.Exit->isActive()) {
        g_Managers.pEnvironment->Runtime().SetStatus(IEnvironment::IRuntime::Status::Quit);
        return;
    }

    //
    // Cycle through all of our objects and apply the changes.
    // Also post our change notifications to the CCM.
    //
    for (ObjectsList::iterator it = Objects.begin(); it != Objects.end(); it++) {
        InputObject* pObject = static_cast<InputObject*>(*it);
        pObject->Update(DeltaTime);
    }
}
