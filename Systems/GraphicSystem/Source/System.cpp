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
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <include/cef_app.h>
#include <flatbuffers/util.h>
#include <GraphicSystem_generated.h>
#pragma warning( push, 0 )
#include <Ogre.h>
#include <OgrePlugin.h>
#include <OgreWindowEventUtilities.h>
#pragma warning( pop )

#include "GraphicSystem_generated.h"
#include "Manager/ServiceManager.h"
#include "Listener/CustomLogListener.h"
#include "Defines.h"
#include "System.h"
#include "Scene.h"

#define CEF_ENABLE_SANDBOX 0

extern IServiceManager* g_serviceManager;

/**
 * @inheritDoc
 */
GraphicSystem::GraphicSystem()
    : ISystem()
    , m_pRenderSystem(NULL)
    , m_pRenderWindow(NULL)
    , m_pMaterialManager(NULL) {


    // Load settings
    std::string graphicSystemFile;
    flatbuffers::LoadFile("GraphicSystem.bin", true, &graphicSystemFile);
    definition_ = Schema::Systems::GetGraphicSystem(graphicSystemFile.c_str());

    /*m_propertySetters["ResourceLocation"] = boost::bind(&GraphicSystem::setResourceLocation, this, _1);
    m_propertySetters["WindowName"] = boost::bind(&GraphicSystem::setWindowName, this, _1);
    m_propertySetters["Resolution"] = boost::bind(&GraphicSystem::setResolution, this, _1);
    m_propertySetters["FullScreen"] = boost::bind(&GraphicSystem::setFullScreen, this, _1);
    m_propertySetters["VerticalSync"] = boost::bind(&GraphicSystem::setVerticalSync, this, _1);
    m_propertySetters["AntiAliasing"] = boost::bind(&GraphicSystem::setAntiAliasing, this, _1);*/

    Ogre::LogManager* logManager = new Ogre::LogManager();
    logManager->createLog("", true, false, true);
    logManager->getDefaultLog()->addListener(new CustomLogListener());

    m_pRoot = new Ogre::Root("", "", "");
    m_pResourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();
    m_pOverlaySystem = new Ogre::OverlaySystem();
}

/**
 * @inheritDoc
 */
GraphicSystem::~GraphicSystem() {
    if (m_bInitialized) {
        Ogre::WindowEventUtilities::removeWindowEventListener(m_pRenderWindow, this);

        m_pResourceGroupManager->shutdownAll();

        // Note: it appears that attempting to unload or uninstall the ParticleFX plugin at all with Ogre1.9
        // will cause heap corruption around the guard pages allocated by the NT memory manager.  Luckily it seems
        // like this is not leaking appreciable resources as the app will soon exit.  This should eventually be revisited
        // should a new version of the ParticleFX plugin and/or Ogre become available.
        // m_pRoot->unloadPlugin("Plugin_ParticleFX");
        // m_pRoot->uninstallPlugin("Plugin_ParticleFX");

        CefShutdown();
    }

    m_pRoot->shutdown();

    delete m_pOverlaySystem;
    delete m_pRoot;
}

/**
 * @inheritDoc
 */
Error GraphicSystem::initialize() {
   ASSERT(!m_bInitialized);

    //
    // Intialize the render system and render window.
    //
#ifdef DEBUG_BUILD
    m_pRoot->loadPlugin("RenderSystem_Direct3D9_d");
#else
    m_pRoot->loadPlugin("RenderSystem_GL");
#endif

    Ogre::RenderSystemList pRenderList;
    pRenderList = m_pRoot->getAvailableRenderers();
    m_pRenderSystem = pRenderList.front();
    m_pRoot->setRenderSystem(m_pRenderSystem);
    m_pRoot->initialise(false, "Hellion Game");

    // Install the particle fx plugin
#ifdef DEBUG_BUILD
    m_pRoot->loadPlugin("Plugin_ParticleFX_d");
#else
    m_pRoot->loadPlugin("Plugin_ParticleFX");
#endif

    std::string name = definition_->windowName()->c_str();
    Ogre::NameValuePairList miscParams;
    miscParams["verticalSync"] = definition_->verticalSync();
    miscParams["FSAA"] = definition_->antiAliasing();
    miscParams["FSAAQuality"] = definition_->antiAliasingQuality();
    m_pRenderWindow = m_pRoot->createRenderWindow(
            name,
            definition_->resolution()->width(),
            definition_->resolution()->height(),
            definition_->fullscreen(),
            &miscParams
    );
    ASSERT(m_pRenderWindow != NULL);
    m_pRenderWindow->setDeactivateOnFocusChange(false);


    // Save the window handle & render window
    size_t hWnd;
    m_pRenderWindow->getCustomAttribute("WINDOW", &hWnd);
    g_serviceManager->getWindowService()->setHandle(hWnd);
    g_serviceManager->getWindowService()->setRenderWindow(m_pRenderWindow);

    // listen to the RenderWindow
    Ogre::WindowEventUtilities::addWindowEventListener(m_pRenderWindow, this);

    // Load materials
    // TODO make a way to use the property setters to set this
    m_pResourceGroupManager->addResourceLocation("Assets/Media/Graphic", "FileSystem", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    m_pResourceGroupManager->initialiseResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    m_pResourceGroupManager->loadResourceGroup(Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    
    CefMainArgs args;
    CefSettings settings;
    settings.no_sandbox = true;
    //settings.single_process = true;
    settings.command_line_args_disabled = true;
    //settings.multi_threaded_message_loop = true;
    CefString(&settings.log_file).FromASCII("logs/Interface.log");
    CefString(&settings.resources_dir_path).FromASCII("Assets/Browser");
    CefString(&settings.locales_dir_path).FromASCII("Assets/Browser/locales");

    bool result = CefInitialize(args, settings, nullptr, NULL);
    ASSERT(result);

    //g_serviceManager->getLogService()->log(LOGOG_LEVEL_INFO, "System initialized");
    m_bInitialized = true;
    return Errors::Success;
}

ISystemScene* GraphicSystem::createScene() {
    m_pSystemScene = new GraphicScene(this);
    return m_pSystemScene;
}

/**
 * @inheritDoc
 */
void GraphicSystem::windowClosed(Ogre::RenderWindow* pRenderWindow) {
    ASSERT(pRenderWindow == m_pRenderWindow);
    g_serviceManager->getRuntimeService()->setStatus(RuntimeService::Status::Quit);
}
/**
 * @inheritDoc
 */
void GraphicSystem::setResourceLocation(Schema::Systems::ResourceLocation* values) {
    ASSERT(!m_bInitialized);
    /*
    const std::string name = *value;
    const std::string locationType = *(++value);
    const std::string resourceGroup = *(++value);
    bool recursive = boost::lexical_cast<bool>(*(++value));
    
    m_pResourceGroupManager->addResourceLocation(name, locationType, resourceGroup, recursive);
    m_pResourceGroupManager->initialiseResourceGroup(resourceGroup);
    m_pResourceGroupManager->loadResourceGroup(resourceGroup);*/
}

/**
 * @inheritDoc
 */
void GraphicSystem::setWindowName(std::string* values) {
    ASSERT(!m_bInitialized);
    auto value = values->begin();
    m_RenderWindowDescription.name = *value;
}

/**
 * @inheritDoc
 */
void GraphicSystem::setResolution(Schema::vector2* values) {
    if (m_bInitialized) {
        m_pRenderWindow->resize(values->x(), values->y());
    } else {
        m_RenderWindowDescription.width = values->x();
        m_RenderWindowDescription.height = values->y();
    }
}

/**
 * @inheritDoc
 */
void GraphicSystem::setFullScreen(bool values) {
    ASSERT(!m_bInitialized);
    m_RenderWindowDescription.useFullScreen = values;
}

/**
 * @inheritDoc
 */
void GraphicSystem::setVerticalSync(bool values) {
    ASSERT(!m_bInitialized);
    m_RenderWindowDescription.miscParams["verticalSync"] = values;
}

/**
 * @inheritDoc
 */
void GraphicSystem::setAntiAliasing(Schema::vector2* values) {
    ASSERT(!m_bInitialized);
    m_RenderWindowDescription.miscParams["FSAA"] = values->x();
    m_RenderWindowDescription.miscParams["FSAAQuality"] = values->y();
}

void GraphicSystem::setProperties(const flatbuffers::Vector<flatbuffers::Offset<Schema::Property>>* properties) {

}

flatbuffers::Vector<flatbuffers::Offset<Schema::Property>>* GraphicSystem::getProperties() {
    return nullptr;
}