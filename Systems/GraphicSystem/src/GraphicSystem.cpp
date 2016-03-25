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

#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/lexical_cast.hpp>
#include <flatbuffers/util.h>
#include <schema/graphic_system_generated.h>

#pragma warning( push, 0 )

#include <Ogre.h>
#include <OgrePlugin.h>
#include <OgreWindowEventUtilities.h>

#pragma warning( pop )

#include "GraphicSystemSystem.h"
#include "Scene.h"

/**
 * @inheritDoc
 */
GraphicSystem::GraphicSystem(Framework* framework)
        : ISystem()
        , m_pRenderWindow(nullptr)
        , m_pMaterialManager(nullptr),
          framework_(framework) {
    flatbuffers::LoadFile("GraphicSystem.bin", true, &definitionData_);
    definition_ = Schema::Systems::GetGraphicSystem(definitionData_.c_str());

    Ogre::LogManager* logManager = new Ogre::LogManager();
    logManager->createLog("", true, false, true);
    logManager->getDefaultLog()->addListener(this);

    m_pRoot = new Ogre::Root("", "", "");
    m_pOverlaySystem = new Ogre::OverlaySystem();
    m_pResourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();

    // Load plugins
    for (auto plugin : *definition_->plugins()) {
        m_pRoot->loadPlugin(plugin->c_str());
    }

    Ogre::RenderSystemList pRenderList = m_pRoot->getAvailableRenderers();
    Ogre::RenderSystem* renderSystem = pRenderList.front();
    m_pRoot->setRenderSystem(renderSystem);
    m_pRoot->initialise(false, "Hellion Game");

    // Misc Params
    Ogre::NameValuePairList miscParams;
    for (auto param : *definition_->miscParams()) {
        miscParams[param->name()->c_str()] = param->value()->c_str();
    }
    m_pRenderWindow = m_pRoot->createRenderWindow(
            definition_->windowName()->c_str(),
            definition_->resolution()->width(),
            definition_->resolution()->height(),
            definition_->fullscreen(),
            &miscParams
    );
    BOOST_ASSERT(m_pRenderWindow != NULL);
    m_pRenderWindow->setDeactivateOnFocusChange(false);
    Ogre::WindowEventUtilities::addWindowEventListener(m_pRenderWindow, this);

    // Save the window handle
    std::size_t hWnd;
    m_pRenderWindow->getCustomAttribute("WINDOW", &hWnd);
    framework->setWindowHandle(hWnd);

    // Load materials
    // TODO resources locations in system config
    m_pResourceGroupManager->addResourceLocation("Assets/Media/Graphic", "FileSystem",
                                                 Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    m_pResourceGroupManager->initialiseResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    m_pResourceGroupManager->loadResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

    BOOST_LOG(logger_) << "Graphic System created";
}

/**
 * @inheritDoc
 */
GraphicSystem::~GraphicSystem() {
    Ogre::WindowEventUtilities::removeWindowEventListener(m_pRenderWindow, this);

    m_pResourceGroupManager->shutdownAll();

    // Note: it appears that attempting to unload or uninstall the ParticleFX plugin at all with Ogre1.9
    // will cause heap corruption around the guard pages allocated by the NT memory manager.  Luckily it seems
    // like this is not leaking appreciable resources as the app will soon exit.  This should eventually be revisited
    // should a new version of the ParticleFX plugin and/or Ogre become available.
    // m_pRoot->unloadPlugin("Plugin_ParticleFX");
    // m_pRoot->uninstallPlugin("Plugin_ParticleFX");

    m_pRoot->shutdown();

    delete m_pOverlaySystem;
    delete m_pRoot;
}

ISystemScene* GraphicSystem::createScene(const Schema::SystemScene* systemScene) {
    m_pSystemScene = new GraphicScene(this, systemScene);
    return m_pSystemScene;
}

void GraphicSystem::messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug,
                                      const Ogre::String& logName, bool& skipThisMessage) {
    if (skipThisMessage) {
        return;
    }

    switch (lml) {
        case Ogre::LogMessageLevel::LML_TRIVIAL:
            BOOST_LOG(logger_) << message;
            break;
        case Ogre::LogMessageLevel::LML_NORMAL:
            BOOST_LOG(logger_) << message;
            break;
        case Ogre::LogMessageLevel::LML_CRITICAL:
            BOOST_LOG(logger_) << message;
            break;
    }
};

void GraphicSystem::windowClosed(Ogre::RenderWindow* pRenderWindow) {
    framework_->setRunning(false);
}
