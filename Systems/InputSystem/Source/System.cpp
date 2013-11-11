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
#include <OIS.h>
#include <OISB.h>

#include "Manager/ServiceManager.h"
#include "System.h"
#include "Scene.h"

extern IServiceManager* g_serviceManager;

/**
 * @inheritDoc
 */
InputSystem::InputSystem(void) 
    : ISystem() {
    new OISB::System();
    m_SceneFactory = boost::factory<InputScene*>();
}

/**
 * @inheritDoc
 */
InputSystem::~InputSystem(void) {
    delete OISB::System::getSingletonPtr();
}

/**
 * @inheritDoc
 */
Error InputSystem::initialize(void) {
    ASSERT(!m_bInitialized);
    
    size_t hWnd = g_serviceManager->getWindowService()->getHandle();
    ASSERTMSG(hWnd != 0, "Window handle should not be null !");

    OIS::ParamList paramList;
    std::ostringstream windowHndStr;
    windowHndStr << hWnd;
    paramList.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    #if defined OIS_WIN32_PLATFORM
        paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_FOREGROUND")));
        paramList.insert(std::make_pair(std::string("w32_mouse"), std::string("DISCL_NONEXCLUSIVE")));
        paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_FOREGROUND")));
        paramList.insert(std::make_pair(std::string("w32_keyboard"), std::string("DISCL_NONEXCLUSIVE")));
    #elif defined OIS_LINUX_PLATFORM
        paramList.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
        paramList.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
        paramList.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
        paramList.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
    #endif

    OIS::InputManager* inputManager = OIS::InputManager::createInputSystem(paramList);
    OISB::System::getSingleton().initialize(inputManager);
    
    g_serviceManager->getLogService()->log(LOGOG_LEVEL_INFO, "System initialized");
    m_bInitialized = true;
    return Errors::Success;
}
