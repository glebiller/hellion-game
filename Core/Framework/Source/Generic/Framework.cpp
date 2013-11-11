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

#include "Defines.h"
#include "Proto.h"
#include "Universal/UScene.h"
#include "Universal/UObject.h"
#include "Debugger/Debugger.h"
#include "Manager/ChangeManager.h"
#include "Manager/ServiceManager.h"
#include "Manager/TaskManager.h"
#include "Service/DefinitionService.h"
#include "Generic/Scheduler.h"
#include "Generic/Instrumentation.h"
#include "Generic/Framework.h"

/**
 * @inheritDoc
 */
Framework::Framework(void)
    : m_serviceManager(new ServiceManager())
    , m_pScheduler(new Scheduler()) 
    , m_pSceneCCM(new ChangeManager())
    , m_pObjectCCM(new ChangeManager()) {
    m_pScene = new UScene(m_pSceneCCM, m_pObjectCCM);
    m_definitionService = new DefinitionService(m_pScene);
}

/**
 * @inheritDoc
 */
Framework::~Framework(void) {
    delete m_pScene;
    delete m_pScheduler;
    delete m_pSceneCCM;
    delete m_pObjectCCM;
    delete m_definitionService;
    delete m_serviceManager;
}

/**
 * @inheritDoc
 */
Error Framework::Initialize(void) {
    m_definitionService->parseEnvironment();
    
    //
    // Init debugger
    // 
#ifdef DEBUG_BUILD
    //bool debuggerActive = Singletons::EnvironmentManager.Variables().GetAsBool("Framework::DebugWindow", false);
    //Singletons::Debugger.initialize(debuggerActive);
    //Singletons::Debugger.setChangeManagers(m_pSceneCCM, m_pObjectCCM);
#endif
    
    //
    // Register the framework as the system access provider.  The system access provider gives the
    //  ability for systems to set the properties in other systems.
    //
    //Singletons::ServiceManager.RegisterSystemAccessProvider(this);
    
    m_pScheduler->init();

    //
    // Complete the parsing of the GDF and the initial scene.
    //
    m_definitionService->parseSystems();
    m_sNextScene = m_definitionService->getStartupScene();
    m_definitionService->parseScene(m_sNextScene);

    m_pScheduler->SetScene(m_pScene);

    //
    // Initialize resources necessary for parallel change distribution.
    //
    m_pObjectCCM->SetTaskManager(m_pScheduler->getTaskManager());
    m_pSceneCCM->SetTaskManager(m_pScheduler->getTaskManager());

    //
    // Init Scene
    // 
    m_pScene->init();

    return Errors::Success;
}


void Framework::Shutdown(void) {
    //
    // Clean debugger
    //
#ifdef DEBUG_BUILD
    //Singletons::Debugger.clean();
#endif

    //
    // Free resources used for parallel execution by the change manager.
    //
    m_pObjectCCM->ResetTaskManager();
    m_pSceneCCM->ResetTaskManager();
}


Error Framework::Execute(void) {
    ServiceManager::get()->getRuntimeService()->setStatus(RuntimeService::Status::Running);

    while (true) {
        processMessages();
        m_pScheduler->Execute();
        m_pScene->update();
        
        if (IServiceManager::get()->getRuntimeService()->isQuit()) {
            break;
        }
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void Framework::processMessages(void) {
#if defined(MSC_COMPILER)
    MSG Msg;
    while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
#endif
}