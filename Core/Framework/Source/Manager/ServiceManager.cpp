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

#include "Manager/ServiceManager.h"
#include "Manager/IServiceManager.h"
#include "Manager/TaskManager.h"
#include "Service/LogService.h"
#include "Service/RuntimeService.h"
#include "Service/SettingService.h"
#include "Service/SystemService.h"
#include "Service/WindowService.h"

/**
 * @inheritDoc
 */
ServiceManager::ServiceManager(void)
    : IServiceManager()
    , m_logService(new LogService())
    , m_runtimeService(new RuntimeService())
    , m_settingService(new SettingService())
    , m_systemService(new SystemService())
    , m_windowService(new WindowService()) {
}

/**
 * @inheritDoc
 */
ServiceManager::~ServiceManager(void) {
    delete m_logService;
    delete m_runtimeService;
    delete m_settingService;
    delete m_systemService;
    delete m_windowService;
}

IServiceManager* ServiceManager::sm_instance = nullptr;
