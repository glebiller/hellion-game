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

#include <boost/functional/factory.hpp>
#include <OISB.h>
#include <schema/input_components_generated.h>

#include "Manager/ServiceManager.h"
#include "System.h"
#include "Scene.h"
#include "Task.h"
#include "Object/Object.h"
#include "Object/CameraInputObject.h"
#include "Object/ConnectInputObject.h"
#include "Object/CursorInputObject.h"
#include "Object/PassiveInputObject.h"
#include "Object/PlayerObject.h"

extern IServiceManager* g_serviceManager;

/**
 * @inheritDoc
 */
InputScene::InputScene(ISystem* pSystem, const Schema::SystemScene* systemScene)
    : ISystemScene(pSystem, systemScene) {
    //m_TaskFactory = boost::factory<InputTask*>();

    /*m_ObjectFactories["Camera"] = boost::factory<CameraInputObject*>();
    m_ObjectFactories["Connect"] = boost::factory<ConnectInputObject*>();
    m_ObjectFactories["Cursor"] = boost::factory<CursorInputObject*>();
    m_ObjectFactories["Passive"] = boost::factory<PassiveInputObject*>();*/
    m_ObjectFactories[Schema::ComponentType::InputVelocity] = boost::factory<PlayerInputObject*>();
    
    m_defaultSchema = OISB::System::getSingleton().getDefaultActionSchemaAutoCreate();
    m_quitInputAction = m_defaultSchema->createAction<OISB::TriggerAction>("Exit");
    m_quitInputAction->bind("Keyboard/Escape");
}

/**
 * @inheritDoc
 */
InputScene::~InputScene() {
    OISB::System::getSingleton().destroyActionSchema(m_defaultSchema);
}

/**
 * @inheritDoc
 */
void InputScene::Update(float DeltaTime) {
    if (m_quitInputAction->isActive()) {
        g_serviceManager->getRuntimeService()->setStatus(RuntimeService::Status::Quit);
        return;
    }

    for (auto object : m_pObjects) {
        InputObject* pObject = static_cast<InputObject*>(object.second);
        pObject->Update(DeltaTime);
    }
}

void InputScene::createTask() {
    m_pSystemTask = new InputTask(this);
}


