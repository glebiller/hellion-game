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

#include "boost/functional/factory.hpp"
#include "OIS.h"

#include "BaseTypes.h"
#include "Interface.h"
#include "OISB.h"

#include "System.h"
#include "Scene.h"


extern ManagerInterfaces    g_Managers;


/**
 * @inheritDoc
 */
InputSystem::InputSystem(void) : ISystem() {
    new OISB::System();

    //
    // Init the Scene factory
    //
    m_SceneFactory = boost::factory<InputScene*>();

    //
    // Fill the properties default values
    //
    
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
    size_t hWnd = g_Managers.pPlatform->Window().GetHandle();
    ASSERTMSG(hWnd != 0, "Window handle should not be null !");
    OIS::InputManager* ois = OIS::InputManager::createInputSystem(hWnd);
    OISB::System::getSingleton().initialize(ois);
    return Errors::Success;
}
