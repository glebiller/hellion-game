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

#include <boost/system/error_code.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/dll.hpp>
#include <flatbuffers/util.h>

#include "Environment_generated.h"
#include "UniversalScene_generated.h"
#include "Defines.h"
#include "Universal/UScene.h"
#include "Universal/UObject.h"
#include "Manager/ChangeManager.h"
#include "Manager/ServiceManager.h"
#include "Service/DefinitionService.h"
#include "Generic/Scheduler.h"
#include "Generic/Framework.h"

Framework::Framework() :
    m_serviceManager(new ServiceManager()),
    m_pSceneCCM(new ChangeManager()),
    m_pObjectCCM(new ChangeManager()),
    m_definitionService(new DefinitionService()),
    m_pScheduler(new Scheduler()),
    m_pScene(nullptr) {
}

Framework::~Framework() {
    if (m_pScene != nullptr) {
        delete m_pScene;
    }
    delete m_pScheduler;
    delete m_pSceneCCM;
    delete m_pObjectCCM;
    delete m_definitionService;
    delete m_serviceManager;
    delete m_environment;
}

boost::system::errc::errc_t Framework::Initialize() {
    // Load settings
    std::string environmentFile;
    flatbuffers::LoadFile("Environment.bin", true, &environmentFile);
    m_environment = Schema::GetEnvironment(environmentFile.c_str());
    
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
    //IServiceManager::get().RegisterSystemAccessProvider(this);
    
    m_pScheduler->init(m_environment);

    //
    // Complete the parsing of the GDF and the initial scene.
    //
    for (auto system : *m_environment->systems()) {
        boost::filesystem::path sharedLibraryPath(boost::dll::program_location().parent_path());
        sharedLibraryPath /= system->c_str() + boost::dll::shared_library::suffix().string();
        boost::dll::shared_library systemLib(sharedLibraryPath);
        m_systemLibraries.push_back(systemLib);

        std::function<void (IServiceManager*)> fnInitSystemLib =
                systemLib.get<void (IServiceManager*)>("InitializeSystemLib");
        fnInitSystemLib(IServiceManager::get());
        std::function<ISystem* ()> fnCreateSystem =
                systemLib.get<ISystem* ()>("CreateSystem");
        ISystem* iSystem = fnCreateSystem();
        //iSystem->initialize();
        Schema::SystemType systemType = iSystem->GetSystemType();
        ASSERT(m_systems.find(systemType) == m_systems.end());
        m_systems[systemType] = iSystem;
    }

    IServiceManager::get()->getRuntimeService()->setNextScene(m_environment->startupScene()->c_str());

    //
    // Initialize resources necessary for parallel change distribution.
    //
    m_pObjectCCM->SetTaskManager(m_pScheduler->getTaskManager());
    m_pSceneCCM->SetTaskManager(m_pScheduler->getTaskManager());

    setNextScene(m_environment->startupScene()->c_str());

    return boost::system::errc::success;
}

///
/// @inheritDoc.
///
void Framework::Shutdown() {
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

///
/// @inheritDoc.
///
Error Framework::Execute() {
    RuntimeService* runtimeService = IServiceManager::get()->getRuntimeService();
    //while (true) {
        if (runtimeService->isNextScene()) {
            setNextScene(runtimeService->getSceneName());
            runtimeService->setStatus(RuntimeService::Status::Running);
        }

        processMessages();
        m_pScheduler->execute();
        m_pScene->update();
        
        if (runtimeService->isQuit()) {
            //break;
        }
    //}

    return Errors::Success;
}

///
/// @inheritDoc.
///
void Framework::processMessages() {
#if defined(MSC_COMPILER)
    MSG Msg;
    while (PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
#endif
}

///
/// @inheritDoc.
///
void Framework::setNextScene(std::string nextSceneName) {
    m_pScheduler->waitForScenes();
    if (m_pScene != nullptr) {
        delete m_pScene;
    }
    m_pScene = new UScene(m_pSceneCCM, m_pObjectCCM, m_systems);
    /*for (auto it : m_systems) {
        m_pScene->Extend(*it.second);
    }*/

    //m_definitionService->parseScene(m_pScene, nextSceneName);
    m_pScheduler->setScene(m_pScene);
    m_pScene->init();
}