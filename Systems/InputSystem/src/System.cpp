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
#include <OIS.h>
#include <OISB.h>
#include <Generic/Framework.h>

#include "System.h"
#include "Scene.h"

/**
 * @inheritDoc
 */
InputSystem::InputSystem(Framework* framework)
        : ISystem(), framework_(framework) {
    new OISB::System();

    std:
    size_t hWnd = framework->getWindowHandle();
    BOOST_ASSERT_MSG(hWnd != 0, "Window handle should not be null !");

    OIS::ParamList paramList;
    paramList.insert(std::make_pair(std::string("WINDOW"), boost::lexical_cast<std::string>(hWnd)));

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
#elif defined OIS_APPLE_PLATFORM

#endif

    OIS::InputManager* inputManager = OIS::InputManager::createInputSystem(paramList);
    OISB::System::getSingleton().initialize(inputManager);
}

/**
 * @inheritDoc
 */
InputSystem::~InputSystem() {
    delete OISB::System::getSingletonPtr();
}

ISystemScene* InputSystem::createScene(const Schema::SystemScene* systemScene) {
    m_pSystemScene = new InputScene(this, systemScene);
    return m_pSystemScene;
}

