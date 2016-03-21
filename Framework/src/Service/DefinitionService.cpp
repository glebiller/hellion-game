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

#include <Defines.h>

#if defined(MSC_COMPILER)
#include <Windows.h>
#endif

#include <fstream>
#include <boost/dll.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "schema/environment_generated.h"
#include "Universal/UScene.h"
#include "Universal/UObject.h"
#include "System/ISystemTask.h"
#include "Manager/ServiceManager.h"
#include "Service/SettingService.h"
#include "Service/DefinitionService.h"
#include "schema/environment_generated.h"

/**
 * @inheritDoc
 */
DefinitionService::DefinitionService() {
    std::string applicationFile;
    //Error result = loadProto("Application.adf.bin", &applicationFile);
    //BOOST_ASSERT(result == Errors::Success);
    //m_gdProto = nullptr; // TODO
}

/**
 * @inheritDoc
 */
DefinitionService::~DefinitionService() {
    //
    // Iterate through all the loaded libraries.
    //
    for (auto it = m_systemLibs.begin(); it != m_systemLibs.end(); it++) {
#if defined(MSC_COMPILER)
        HMODULE hLib = reinterpret_cast<HMODULE>(it->hLib);
        //
        // Get the system destruction function.
        //
        DestroySystemFunction fnDestroySystem = reinterpret_cast<DestroySystemFunction>(GetProcAddress(hLib, "DestroySystem"));
        if (fnDestroySystem != NULL) {
            fnDestroySystem(it->pSystem);
        }

        FreeLibrary(hLib);
#endif
    }

    m_systemLibs.clear();
}

/**
 * @inheritDoc
 */
void DefinitionService::parseEnvironment() {
    // TODO save environment
    //SettingService* settingService = IServiceManager::get()->getSettingService();
    //settingService->initialize(nullptr);
}

/**
 * @inheritDoc
 */
void DefinitionService::parseSystems() {
    BOOST_ASSERT(m_gdProto->systems()->size() > 0);
    for (auto system : *m_gdProto->systems()) {
        loadSystemLibrary(system->c_str());
        BOOST_ASSERT(m_pSystem != NULL);

        // Get the default properties from system, then Initialize it
        // TODO
        //m_pSystem->setProperties(system->properties());
        //m_pSystem->initialize();
        /*ASSERTMSG1(system.type() == m_pSystem->GetSystemType(),
                   "Parser identified an incorrect system type. It should be %s.", Proto::SystemType_Name(m_pSystem->GetSystemType()));
                   */
    }
}

/**
 * @inheritDoc
 */
void DefinitionService::parseScene(UScene* scene, std::string sceneName) {
    //const auto scenes = m_gdProto->scenes();
    /*auto sceneIt = std::find(scenes->begin(), scenes->end(), sceneName.c_str());
    if (sceneIt == scenes.end()) {
        return;
    }
    */
    SystemService* systemService = IServiceManager::get()->getSystemService();
    
    //
    // Create the initial scene for each system.
    //
    for (auto it : systemService->get()) {
        scene->Extend(*it.second, nullptr);
    }

    //
    // Parse the SDF file
    //
    std::string sceneFile;
    Error result = loadProto(sceneName + ".sdf.bin", &sceneFile);
    // TODO
    /*const Schema::Scene* sceneProto = Schema::CreateScene(sceneFile.c_str()) nullptr;
    BOOST_ASSERT(result == Errors::Success);

    //
    // Initialize the scene templates.
    //
    scene->addTemplates(sceneProto->templates());

    //
    // Initialize the System scenes.
    //
    for (auto system : *sceneProto->systems()) {
        m_pSystem = systemService->get(system->type());
        ASSERTMSG1(m_pSystem != NULL, "Parser was unable to get system %s.", Proto::SystemType_Name(system.type()));

        if (m_pSystem != NULL) {
            auto it = scene->GetSystemScenes().find(m_pSystem->GetSystemType());
            ASSERTMSG1(it != scene->GetSystemScenes().end(), "Parser was unable to find a scene for system %s.", Proto::SystemType_Name(system.type()));
            m_pSystemScene = it->second;
            BOOST_ASSERT(m_pSystemScene != NULL);
            // Initialize system scene properties
            m_pSystemScene->setProperties(system->properties());
            m_pSystemScene->initialize();
            m_pSystemScene->GetSystemTask<ISystemTask>()->initialize();
        }
    }

    //
    // Initialize the scene objects.
    //
    for (auto object : *sceneProto->objects()) {
        scene->createObject(object);
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
    for (auto link : *sceneProto->links()) {
        UObject* pSubject = scene->FindObject(link->subject()->name()->c_str());
        UObject* pObserver = scene->FindObject(link->observer()->name()->c_str());

        //
        // Get the extension for the object.
        //
        ISystemObject* pSystemObserver = pSystemObserver = pObserver->GetExtension(link->observer()->systemType());

        //
        // Call the scene to register the links.
        //
        if (link->subject()->systemType() != Proto::SystemType::Undefined) {
            ISystemObject* pSystemSubject = pSystemSubject = pSubject->GetExtension(link->subject()->systemType());
            scene->CreateObjectLink(pSystemSubject, pSystemObserver);
        } else {
            scene->CreateObjectLink(pSubject, pSystemObserver);
        }
    }*/
}

std::string DefinitionService::getStartupScene() {
    return m_gdProto->startupScene()->c_str();
}

/**
 * @inheritDoc
 */
Error DefinitionService::loadProto(std::string file, std::string* proto) {
    boost::filesystem::path filePath(boost::filesystem::current_path() / file);
    if (!boost::filesystem::exists(filePath)) {
        return Errors::Failure;
    }
    
    // Read file
    std::fstream input(filePath.c_str(), std::ios::in | std::ios::binary);
    if (!input.is_open()) {
        return Errors::Failure;
    }
    *proto = std::string(std::istreambuf_iterator<char>(input), std::istreambuf_iterator<char>());
    if (input.bad()) {
        return Errors::Failure;
    }

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
Error DefinitionService::loadSystemLibrary(const std::string type) {
    Error Err = Errors::Failure;

    boost::filesystem::path shared_library_path(".");               // argv[1] contains path to directory with our plugin library
    shared_library_path /= type;
    boost::function<void(IServiceManager*)> fnInitSystemLib = boost::dll::import<void (IServiceManager*)>(
            shared_library_path, "InitializeSystemLib"
    );

    if (fnInitSystemLib != NULL) {
        fnInitSystemLib(IServiceManager::get());
    }

#if defined(MSVC_COMPILER)

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

#endif
    return Errors::Success;
}

