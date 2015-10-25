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
#include <cef_browser.h>

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

class MouseCallback : public OIS::MouseListener {

    virtual ~MouseCallback() {}
    virtual bool mouseMoved( const OIS::MouseEvent &arg) {
        Handle browserHostPtr = g_serviceManager->getWindowService()->getBrowser();
        if (browserHostPtr != nullptr) {
            CefRefPtr<CefBrowser> browser = *static_cast<CefRefPtr<CefBrowser>*>(browserHostPtr);
            CefMouseEvent mouseEvent;
            mouseEvent.x = arg.state.X.abs;
            mouseEvent.y = arg.state.Y.abs;
            browser->GetHost()->SendMouseMoveEvent(mouseEvent, false);
        }
        return true;
    }
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
        Handle browserHostPtr = g_serviceManager->getWindowService()->getBrowser();
        if (browserHostPtr != nullptr) {
            CefRefPtr<CefBrowser> browser = *static_cast<CefRefPtr<CefBrowser>*>(browserHostPtr);
            CefMouseEvent mouseEvent;
            mouseEvent.x = arg.state.X.abs;
            mouseEvent.y = arg.state.Y.abs;
            browser->GetHost()->SendMouseClickEvent(mouseEvent, CefBrowserHost::MouseButtonType::MBT_LEFT, false, 1);
        }
        return true;
    }
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id) {
        Handle browserHostPtr = g_serviceManager->getWindowService()->getBrowser();
        if (browserHostPtr != nullptr) {
            CefRefPtr<CefBrowser> browser = *static_cast<CefRefPtr<CefBrowser>*>(browserHostPtr);
            CefMouseEvent mouseEvent;
            mouseEvent.x = arg.state.X.abs;
            mouseEvent.y = arg.state.Y.abs;
            browser->GetHost()->SendMouseClickEvent(mouseEvent, CefBrowserHost::MouseButtonType::MBT_LEFT, true, 1);
        }
        return true;
    }
};
class KeyCallback : public OIS::KeyListener {

    virtual ~KeyCallback() {}
    virtual bool keyPressed(const OIS::KeyEvent &arg) {
        if (arg.text == 0) {
            return true;
        }

        const OIS::Keyboard* device = static_cast<const OIS::Keyboard*>(arg.device);
        int modifiers = 0;
        if (device->isModifierDown(OIS::Keyboard::Modifier::Shift)) {
            modifiers |= EVENTFLAG_SHIFT_DOWN;
        }

        Handle browserHostPtr = g_serviceManager->getWindowService()->getBrowser();
        if (browserHostPtr != nullptr) {
            CefRefPtr<CefBrowser> browser = *static_cast<CefRefPtr<CefBrowser>*>(browserHostPtr);
            CefKeyEvent keyEvent;
            keyEvent.is_system_key = false;
            keyEvent.modifiers = modifiers;
            keyEvent.windows_key_code = arg.text;
            keyEvent.type = KEYEVENT_RAWKEYDOWN;
            browser->GetHost()->SendKeyEvent(keyEvent);
            keyEvent.type = KEYEVENT_CHAR;
            browser->GetHost()->SendKeyEvent(keyEvent);
        }
        return true;
    }

    virtual bool keyReleased(const OIS::KeyEvent &arg) {
        if (arg.text == 0) {
            return true;
        }

        Handle browserHostPtr = g_serviceManager->getWindowService()->getBrowser();
        if (browserHostPtr != nullptr) {
            CefRefPtr<CefBrowser> browser = *static_cast<CefRefPtr<CefBrowser>*>(browserHostPtr);
            CefKeyEvent keyEvent;
            keyEvent.is_system_key = false;
            keyEvent.windows_key_code = arg.text;
            keyEvent.type = KEYEVENT_KEYUP;
            browser->GetHost()->SendKeyEvent(keyEvent);
        }
        return true;
    }
};

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
    OISB::System::getSingleton().getOISMouse()->setEventCallback(new MouseCallback());
    OISB::System::getSingleton().getOISKeyboard()->setEventCallback(new KeyCallback());

    g_serviceManager->getLogService()->log(LOGOG_LEVEL_INFO, "System initialized");
    m_bInitialized = true;
    return Errors::Success;
}
