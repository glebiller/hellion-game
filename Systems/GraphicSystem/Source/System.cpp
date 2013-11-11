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
#include <berkelium/Berkelium.hpp>

#pragma warning( push, 0 )
#include <Ogre.h>
#include <OgrePlugin.h>
#include <OgreWindowEventUtilities.h>
#pragma warning( pop )

#include "Manager/ServiceManager.h"
#include "Listener/CustomLogListener.h"
#include "Defines.h"
#include "System.h"
#include "Scene.h"

extern IServiceManager* g_serviceManager;

/**
 * @inheritDoc
 */
GraphicSystem::GraphicSystem(void) 
    : ISystem()
    , m_pRenderSystem(NULL)
    , m_pRenderWindow(NULL)
    , m_pMaterialManager(NULL) {
    m_SceneFactory = boost::factory<GraphicScene*>();

    m_propertySetters["ResourceLocation"] = boost::bind(&GraphicSystem::setResourceLocation, this, _1);
    m_propertySetters["WindowName"] = boost::bind(&GraphicSystem::setWindowName, this, _1);
    m_propertySetters["Resolution"] = boost::bind(&GraphicSystem::setResolution, this, _1);
    m_propertySetters["FullScreen"] = boost::bind(&GraphicSystem::setFullScreen, this, _1);
    m_propertySetters["VerticalSync"] = boost::bind(&GraphicSystem::setVerticalSync, this, _1);
    m_propertySetters["AntiAliasing"] = boost::bind(&GraphicSystem::setAntiAliasing, this, _1);

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
GraphicSystem::~GraphicSystem(void) {
    if (m_bInitialized) {
        Ogre::WindowEventUtilities::removeWindowEventListener(m_pRenderWindow, this);

        m_pResourceGroupManager->shutdownAll();

        // Note: it appears that attempting to unload or uninstall the ParticleFX plugin at all with Ogre1.9
        // will cause heap corruption around the guard pages allocated by the NT memory manager.  Luckily it seems
        // like this is not leaking appreciable resources as the app will soon exit.  This should eventually be revisited
        // should a new version of the ParticleFX plugin and/or Ogre become available.
        // m_pRoot->unloadPlugin("Plugin_ParticleFX");
        // m_pRoot->uninstallPlugin("Plugin_ParticleFX");

        Berkelium::destroy();
    }

    m_pRoot->shutdown();

    delete m_pOverlaySystem;
    delete m_pRoot;
}

/**
 * @inheritDoc
 */
Error GraphicSystem::initialize(void) {
   ASSERT(!m_bInitialized);

    //
    // Intialize the render system and render window.
    //
#ifdef DEBUG_BUILD
    m_pRoot->loadPlugin("RenderSystem_Direct3D9_d");
#else
    m_pRoot->loadPlugin("RenderSystem_Direct3D9");
#endif

    Ogre::RenderSystemList pRenderList;
    pRenderList = m_pRoot->getAvailableRenderers();
    m_pRenderSystem = pRenderList.front();
    m_pRoot->setRenderSystem(m_pRenderSystem);
    m_pRoot->initialise(false, "Shoot Em Up Project");

    // Install the particle fx plugin
#ifdef DEBUG_BUILD
    m_pRoot->loadPlugin("Plugin_ParticleFX_d");
#else
    m_pRoot->loadPlugin("Plugin_ParticleFX");
#endif

    // Note: createRenderWindow() is now called directly so that a render winow is created.  The old calling steps
    // yielded a render system with no render window until after plugins load which causes assertions for CreateParticleSystem
    // which requires a render window at the time the billboard renderer loads.
    m_pRenderWindow = m_pRoot->createRenderWindow(
        m_RenderWindowDescription.name,
        m_RenderWindowDescription.width,
        m_RenderWindowDescription.height,
        m_RenderWindowDescription.useFullScreen,
        &m_RenderWindowDescription.miscParams
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
    m_pResourceGroupManager->addResourceLocation("Assets/Media/Graphic", "FileSystem", "Default", true);
    m_pResourceGroupManager->initialiseResourceGroup("Default");
    m_pResourceGroupManager->loadResourceGroup("Default");
    
    Berkelium::init(Berkelium::FileString::empty());
    
    g_serviceManager->getLogService()->log(LOGOG_LEVEL_INFO, "System initialized");
    m_bInitialized = true;
    return Errors::Success;
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
void GraphicSystem::setResourceLocation(Proto::RepeatedString* values) {
    ASSERT(!m_bInitialized);
    auto value = values->begin();

    const std::string name = *value;
    const std::string locationType = *(++value);
    const std::string resourceGroup = *(++value);
    bool recursive = boost::lexical_cast<bool>(*(++value));
    
    /*m_pResourceGroupManager->addResourceLocation(name, locationType, resourceGroup, recursive);
    m_pResourceGroupManager->initialiseResourceGroup(resourceGroup);
    m_pResourceGroupManager->loadResourceGroup(resourceGroup);*/
}

/**
 * @inheritDoc
 */
void GraphicSystem::setWindowName(Proto::RepeatedString* values) {
    ASSERT(!m_bInitialized);
    auto value = values->begin();
    m_RenderWindowDescription.name = *value;
}

/**
 * @inheritDoc
 */
void GraphicSystem::setResolution(Proto::RepeatedString* values) {
    auto value = values->begin();

    u32 width  = boost::lexical_cast<u32>(*value);
    u32 height = boost::lexical_cast<u32>(*(++value));

    if (m_bInitialized) {
        m_pRenderWindow->resize(width, height);
    } else {
        m_RenderWindowDescription.width = width;
        m_RenderWindowDescription.height = height;
    }
}

/**
 * @inheritDoc
 */
void GraphicSystem::setFullScreen(Proto::RepeatedString* values) {
    ASSERT(!m_bInitialized);
    auto value = values->begin();

    m_RenderWindowDescription.useFullScreen = boost::lexical_cast<bool>(*value);
}

/**
 * @inheritDoc
 */
void GraphicSystem::setVerticalSync(Proto::RepeatedString* values) {
    ASSERT(!m_bInitialized);
    auto value = values->begin();

    m_RenderWindowDescription.miscParams["verticalSync"] = boost::lexical_cast<bool>(*value);
}

/**
 * @inheritDoc
 */
void GraphicSystem::setAntiAliasing(Proto::RepeatedString* values) {
    ASSERT(!m_bInitialized);
    auto value = values->begin();

    m_RenderWindowDescription.miscParams["FSAA"] = *(value++);
    m_RenderWindowDescription.miscParams["FSAAQuality"] = *value;
}
