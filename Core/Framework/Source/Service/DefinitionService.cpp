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
#include <Windows.h>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "Proto.h"
#include "Universal/UScene.h"
#include "Universal/UObject.h"
#include "System/ISystemTask.h"
#include "Manager/ServiceManager.h"
#include "Service/SettingService.h"
#include "Generic/SystemInterface.h"
#include "Service/DefinitionService.h"
#include "Environment_generated.h"

/**
 * @inheritDoc
 */
DefinitionService::DefinitionService() {
    Error result = loadProto("Application.adf.bin", &m_gdProto);
    ASSERT(result == Errors::Success);
}

/**
 * @inheritDoc
 */
DefinitionService::~DefinitionService(void) {
    //
    // Iterate through all the loaded libraries.
    //
    for (auto it = m_systemLibs.begin(); it != m_systemLibs.end(); it++) {
        HMODULE hLib = reinterpret_cast<HMODULE>(it->hLib);
        //
        // Get the system destruction function.
        //
        DestroySystemFunction fnDestroySystem = reinterpret_cast<DestroySystemFunction>(GetProcAddress(hLib, "DestroySystem"));
        if (fnDestroySystem != NULL) {
            fnDestroySystem(it->pSystem);
        }

        FreeLibrary(hLib);
    }

    m_systemLibs.clear();
}

/**
 * @inheritDoc
 */
void DefinitionService::parseEnvironment(void) {
    ASSERT(m_gdProto.IsInitialized());
    SettingService* settingService = IServiceManager::get()->getSettingService();
    for (auto property : m_gdProto.properties()) {
        settingService->add(property);
    }
}

/**
 * @inheritDoc
 */
void DefinitionService::parseSystems(void) {
    ASSERT(m_gdProto.systems_size() > 0);
    for (auto system : m_gdProto.systems()) {
        loadSystemLibrary(system.type(), &m_pSystem);
        ASSERT(m_pSystem != NULL);

        // Get the default properties from system, then Initialize it
        m_pSystem->setProperties(system.properties());
        m_pSystem->initialize();
        ASSERTMSG1(system.type() == m_pSystem->GetSystemType(),
                   "Parser identified an incorrect system type. It should be %s.", Proto::SystemType_Name(m_pSystem->GetSystemType()));
    }
}

/**
 * @inheritDoc
 */
void DefinitionService::parseScene(UScene* scene, std::string sceneName) {
    const auto& scenes = m_gdProto.scenes();
    auto sceneIt = std::find(scenes.begin(), scenes.end(), sceneName);
    if (sceneIt == scenes.end()) {
        return;
    }
    
    SystemService* systemService = IServiceManager::get()->getSystemService();
    
    //
    // Create the initial scene for each system.
    //
    for (auto it : systemService->get()) {
        scene->Extend(it.second);
    }

    //
    // Parse the SDF file
    //
    Proto::Scene sceneProto;
    Error result = loadProto(*sceneIt + ".sdf.bin", &sceneProto);
    ASSERT(result == Errors::Success);

    //
    // Initialize the scene templates.
    //
    scene->addTemplates(&sceneProto.templates());

    //
    // Initialize the System scenes.
    //
    for (auto system : sceneProto.systems()) {
        m_pSystem = systemService->get(system.type());
        ASSERTMSG1(m_pSystem != NULL, "Parser was unable to get system %s.", Proto::SystemType_Name(system.type()));

        if (m_pSystem != NULL) {
            auto it = scene->GetSystemScenes().find(m_pSystem->GetSystemType());
            ASSERTMSG1(it != scene->GetSystemScenes().end(), "Parser was unable to find a scene for system %s.", Proto::SystemType_Name(system.type()));
            m_pSystemScene = it->second;
            ASSERT(m_pSystemScene != NULL);
            // Initialize system scene properties
            m_pSystemScene->setProperties(system.properties());
            m_pSystemScene->initialize();
            m_pSystemScene->GetSystemTask<ISystemTask>()->initialize();
        }
    }

    //
    // Initialize the scene objects.
    //
    for (auto object : sceneProto.objects()) {
        scene->createObject(&object);
    }

    //
    // Refresh all scenes
    // 
    for (auto scene : scene->GetSystemScenes()) {
        scene.second->GlobalSceneStatusChanged(ISystemScene::PostLoadingObjects);
    }

    //
    // Initialize the links.
    //
    for (auto link : sceneProto.links()) {
        UObject* pSubject = scene->FindObject(link.subject().c_str());
        UObject* pObserver = scene->FindObject(link.observer().c_str());

        //
        // Get the extension for the object.
        //
        ISystemObject* pSystemObserver = pSystemObserver = pObserver->GetExtension(link.observersystemtype());

        //
        // Call the scene to register the links.
        //
        if (link.subjectsystemtype() != Proto::SystemType::Null) {
            ISystemObject* pSystemSubject = pSystemSubject = pSubject->GetExtension(link.subjectsystemtype());
            scene->CreateObjectLink(pSystemSubject, pSystemObserver);
        } else {
            scene->CreateObjectLink(pSubject, pSystemObserver);
        }
    }
}

std::string DefinitionService::getStartupScene(void) {
    return m_gdProto.startupscene();
}

/**
 * @inheritDoc
 */
Error DefinitionService::loadProto(std::string file, google::protobuf::Message* proto) {
    boost::filesystem::path filePath(boost::filesystem::current_path() / file);
    if (!boost::filesystem::exists(filePath)) {
        return Errors::Failure;
    }
    
    // Read file
    std::fstream input(filePath.c_str(), std::ios::in | std::ios::binary);

    // Build proto
    proto->Clear();
    proto->ParseFromIstream(&input);

    // TEST
    //boost::filesystem::path filePatha(boost::filesystem::current_path() / "Environment.bin");
    //std::fstream inputa(filePatha.c_str(), std::ios::in | std::ios::binary);

    //auto environment = Schema::GetEnvironment(&inputa);
    //auto propp = environment->properties()->Get(0);

    //printf("%s", propp);

    // TEST
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error DefinitionService::loadSystemLibrary(Proto::SystemType type,  ISystem** ppSystem) {
    Error Err = Errors::Failure;

    std::string libraryName = Proto::SystemType_Name(type) + "System";
    HMODULE hLib = LoadLibraryA(libraryName.c_str());
    if (hLib == NULL) {
        ASSERTMSG1(false, "Failed to load system %s", libraryName);
        return Err;
    }
    
    InitializeSystemLibFunction fnInitSystemLib = reinterpret_cast<InitializeSystemLibFunction>(GetProcAddress(hLib, "InitializeSystemLib"));
    if (fnInitSystemLib != NULL) {
        fnInitSystemLib(IServiceManager::get());
    }
    
    CreateSystemFunction fnCreateSystem = reinterpret_cast<CreateSystemFunction>(GetProcAddress(hLib, "CreateSystem"));
    if (fnCreateSystem == NULL) {
        return Err;
    }
    
    ISystem* pSystem = fnCreateSystem();
    if (pSystem == NULL) {
        return Err;
    }

    SystemService* systemService = IServiceManager::get()->getSystemService();
    Proto::SystemType systemType = pSystem->GetSystemType();
    ISystem* pCurrSystem = systemService->get(systemType);
    if (pCurrSystem != NULL) {
        return Err;
    }

    systemService->add(pSystem);
    
    SystemLib systemLib = {
        reinterpret_cast<Handle>(hLib),
        pSystem
    };
    m_systemLibs.push_back(systemLib);

    *ppSystem = pSystem;
    return Errors::Success;
}

