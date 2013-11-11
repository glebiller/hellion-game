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

#include "Manager/ITaskManager.h"

class LogService;
class RuntimeService;
class SettingService;
class SystemService;
class WindowService;

/**
 * Interface class for providing services to the systems.
 */
class IServiceManager {
public:
    /**
     * Default constructor.
     */
    IServiceManager() {
        sm_instance = this;
    };

    /**
     * Destructor.
     */
    virtual ~IServiceManager() {};

    /**
     * Gets task manager.
     *
     * @return  null if it fails, else the task manager.
     */
    // TODO Wrap it inside a service
    virtual ITaskManager* getTaskManager() = 0;

    /**
     * Sets task manager.
     *
     * @param [in,out]  taskManager If non-null, manager for task.
     */
    virtual void setTaskManager(ITaskManager* taskManager) = 0;

    /**
     * Gets runtime service.
     *
     * @return  null if it fails, else the runtime service.
     */
    virtual LogService* getLogService() = 0;

    /**
     * Gets system service.
     *
     * @return  null if it fails, else the system service.
     */
    virtual RuntimeService* getRuntimeService() = 0;

    /**
     * Gets system service.
     *
     * @return  null if it fails, else the system service.
     */
    virtual SettingService* getSettingService() = 0;

    /**
     * Gets system service.
     *
     * @return  null if it fails, else the system service.
     */
    virtual SystemService* getSystemService() = 0;

    /**
     * Gets window service.
     *
     * @return  null if it fails, else the window service.
     */
    virtual WindowService* getWindowService() = 0;

    /**
     * Gets the get.
     *
     * @return  null if it fails, else.
     */
    static IServiceManager* get() {
        return sm_instance;
    }

private:
    static IServiceManager*     sm_instance;

};
