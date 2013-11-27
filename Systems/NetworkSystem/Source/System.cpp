// Copyright © 2008-2009 Intel Corporation
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

#include "Defines.h"
#include "Manager/ServiceManager.h"
#include "System.h"
#include "Scene.h"
#include "Object/Object.h"

extern IServiceManager* g_serviceManager;

/**
 * @inheritDoc
 */
NetworkSystem::NetworkSystem(void) : ISystem() {
    m_SceneFactory = boost::factory<NetworkScene*>();
    m_networkService = new NetworkService(static_cast<NetworkSystem*>(this));
}

/**
 * @inheritDoc
 */
NetworkSystem::~NetworkSystem(void) {
    boost::checked_delete(m_networkService);
}

/**
 * @inheritDoc
 */
Error NetworkSystem::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_networkService->connect("localhost", "26000");
    
    g_serviceManager->getLogService()->log(LOGOG_LEVEL_INFO, "System initialized");
    m_bInitialized = true;
    return Errors::Success;
}