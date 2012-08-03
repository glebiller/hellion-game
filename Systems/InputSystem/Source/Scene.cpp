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


/////////////////////////////////
/// Includes
/////////////////////////////////


#include <BaseTypes.h>
#include <Interface.h>
#include "OISB.h"

#include "System.h"
#include "Scene.h"
#include "Object/Object.h"
#include "Object/ObjectCamera.h"
#include "Object/ObjectGui.h"
#include "Object/ObjectPlayer.h"
#include "Task.h"


/////////////////////////////////
/// Static elements
/////////////////////////////////


extern ManagerInterfaces   g_Managers;


/////////////////////////////////
/// Class implementation
/////////////////////////////////


InputScene::InputScene(
    ISystem* pSystem
)
    : ISystemScene(pSystem)
    , m_pInputTask(NULL)
    , m_DefaultSchema(NULL) {
}

InputScene::~InputScene(
    void
) {
    //
    // Free all the remaining objects.
    //
    for (std::list<InputObject*>::iterator it = m_Objects.begin(); it != m_Objects.end(); it++) {
        delete *it;
    }

    m_Objects.clear();
    //
    // Delete the task object
    //
    SAFE_DELETE(m_pInputTask);
}


System::Type
InputScene::GetSystemType(
    void
) {
    return System::Types::Input;
}


Error
InputScene::Initialize(
    std::vector<Properties::Property> Properties
) {
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
    m_InputActions.CameraRotateUpDown->setProperty("EmulationReturnSpeed", 0.0f);
    m_InputActions.CameraRotateUpDown->setProperty("Sensitivity", 0.001f);
    m_InputActions.CameraRotateUpDown->bind("Mouse/Y Axis");
    // Camera Right / Left
    m_InputActions.CameraRotateRightLeft = m_DefaultSchema->createAction<OISB::AnalogAxisAction>("CameraRotateRightLeft");
    m_InputActions.CameraRotateRightLeft->setProperty("UseCircularValues", true);
    m_InputActions.CameraRotateRightLeft->setProperty("MinimumValue", 0.0f);
    m_InputActions.CameraRotateRightLeft->setProperty("MaximumValue", 2.0f * Math::Angle::Pi);
    m_InputActions.CameraRotateRightLeft->setProperty("EmulationSpeed", 2.0f);
    m_InputActions.CameraRotateRightLeft->setProperty("EmulationReturnSpeed", 0.0f);
    m_InputActions.CameraRotateRightLeft->setProperty("Sensitivity", 0.001f);
    m_InputActions.CameraRotateRightLeft->bind("Mouse/X Axis");


    m_bInitialized = True;
    return Errors::Success;
}


void
InputScene::GetProperties(
    Properties::Array& Properties
) {
    UNREFERENCED_PARAM(Properties);
}


void
InputScene::SetProperties(
    Properties::Array Properties
) {
    ASSERT(m_bInitialized);
}


pcstr*
InputScene::GetObjectTypes(
    void
) {
    return NULL;
}


ISystemObject*
InputScene::CreateObject(
    pcstr pszName,
    pcstr pszType
) {
    ASSERT(m_bInitialized);
    //
    // Create the object and add it to the object list.
    //
    InputObject* pObject = NULL;

    if (strcmp(pszType, "Player") == 0) {
        pObject = new InputPlayerObject(this, pszName);
    } else if (strcmp(pszType, "Camera") == 0) {
        pObject = new InputCameraObject(this, pszName);
    } else {
        pObject = new InputGuiObject(this, pszName);
    }

    ASSERT(pObject != NULL);

    if (pObject != NULL) {
        m_Objects.push_back(pObject);
    }

    return pObject;
}


Error
InputScene::DestroyObject(
    ISystemObject* pSystemObject
) {
    ASSERT(m_bInitialized);
    ASSERT(pSystemObject != NULL);
    //
    // Cast to a InputObject so that the correct destructor will be called.
    //
    InputObject* pObject = reinterpret_cast<InputObject*>(pSystemObject);
    //
    // Remove the object from the list and delete it.
    //
    m_Objects.remove(pObject);
    SAFE_DELETE(pSystemObject);
    return Errors::Success;
}


ISystemTask*
InputScene::GetSystemTask(
    void
) {
    return m_pInputTask;
}


System::Changes::BitMask
InputScene::GetPotentialSystemChanges(
    void
) {
    return System::Changes::Graphics::GUI;
}

void InputScene::Update(
    f32 DeltaTime
) {
    std::list<InputObject*>& Objects = m_Objects;

    // Global inputs
    if (m_InputActions.Exit->isActive()) {
        g_Managers.pEnvironment->Runtime().SetStatus(IEnvironment::IRuntime::Status::Quit);
    }

    //
    // Cycle through all of our objects and apply the changes. Also post our change notifications to the CCM.
    //
    for (std::list<InputObject*>::iterator it = Objects.begin(); it != Objects.end(); it++) {
        InputObject* pObject = *it;

        //--------------------------------------------------------------------------
        // GUI Objects do not need to be modified.
        //
        if (pObject->m_Type == pObject->Type_GUI) {
            continue;
        }

        pObject->Update(DeltaTime);
    }
}

