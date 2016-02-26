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

#pragma once

#include "Manager/IServiceManager.h"
#include "Manager/TaskManager.h"
#include "Service/RuntimeService.h"
#include "Service/SettingService.h"
#include "Service/SystemService.h"
#include "Service/WindowService.h"

/**
 * Managerial class for providing services to the systems.
 *
 * @sa  IServiceManager
 */
class ServiceManager : public IServiceManager {
public:
    /**
     * Constructor.
     */
    ServiceManager();

    /**
     * Destructor.
     */
    ~ServiceManager();

    /**
     * @inheritDoc
     */
    ITaskManager* getTaskManager() {
        return m_taskManager;
    };
    
    /**
     * @inheritDoc
     */
    void setTaskManager(ITaskManager* taskManager) {
        m_taskManager = taskManager;
    };

    /**
     * @inheritDoc
     */
    RuntimeService* getRuntimeService() {
        return m_runtimeService;
    };
    
    /**
     * @inheritDoc
     */
    SettingService* getSettingService() {
        return m_settingService;
    };
    
    /**
     * @inheritDoc
     */
    SystemService* getSystemService() {
        return m_systemService;
    };
    
    /**
     * @inheritDoc
     */
    WindowService* getWindowService() {
        return m_windowService;
    };

private:
    ITaskManager*               m_taskManager;
    RuntimeService*             m_runtimeService;
    SettingService*             m_settingService;
    SystemService*              m_systemService;
    WindowService*              m_windowService;

};

