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

#include <boost/thread/thread.hpp>
#include <boost/timer/timer.hpp>

#include "Manager/IServiceManager.h"
#include "Manager/TaskManager.h"
#include "Service/SettingService.h"
#include "Service/RuntimeService.h"
#include "Universal/UScene.h"
#include "Generic/Scheduler.h"
#include "Debugger/Debugger.h"

// Set the timer to 120Hz
const boost::timer::nanosecond_type Scheduler::sm_defaultClockFrequency =  boost::timer::nanosecond_type(1000000000LL / 120);

/**
 * @inheritDoc
 */
Scheduler::Scheduler()
    : m_runtimeService(IServiceManager::get()->getRuntimeService()) {
    SettingService* settingService = IServiceManager::get()->getSettingService();
    m_benchmarkingEnabled = settingService->getBool("Scheduler::Benchmarking");
    m_pTaskManager = new TaskManager(settingService->getInt("TaskManager::Threads"));
    IServiceManager::get()->setTaskManager(m_pTaskManager);
    m_executionTimer.start();
}

/**
 * @inheritDoc
 */
Scheduler::~Scheduler(void) {
    m_pTaskManager->Shutdown();
    delete m_pTaskManager;
}

/**
 * Initialises this object.
 */
void Scheduler::init(void) {
    m_pTaskManager->Init();
}

/**
 * @inheritDoc
 */
void Scheduler::SetScene(const UScene* pScene) {
    //
    // Setup the Debugger
    // 
#ifdef DEBUG_BUILD
    //Singletons::Debugger.setScene(pScene);
#endif

    //
    // Wait for any executing scenes to finish and clear out the list.
    //
    ISystemTask* aScenesToWaitFor[Proto::SystemType_MAX];
    u32 cScenesToWaitFor = 0;

    for (auto it = m_SceneExecs.begin(); it != m_SceneExecs.end(); it++) {
        ASSERT(cScenesToWaitFor < Proto::SystemType_MAX);
        aScenesToWaitFor[ cScenesToWaitFor++ ] = it->second->GetSystemTask<ISystemTask>();
    }

    m_SceneExecs.clear();

    if (cScenesToWaitFor > 0) {
        m_pTaskManager->WaitForSystemTasks(aScenesToWaitFor, cScenesToWaitFor);
    }

    //
    // Copy over all the system scenes.
    //
    const UScene::SystemScenes& SystemScenes = pScene->GetSystemScenes();
    for (auto it = SystemScenes.begin(); it != SystemScenes.end(); it++) {
        if (it->second->GetSystemTask<ISystemTask>() != NULL) {
            m_SceneExecs[ it->first ] = it->second;
        }
    }

    //
    // Re-create the timer as a scene load may have taken a long time.
    //
    m_executionTimer.start();
}

/**
 * @inheritDoc
 */
void Scheduler::Execute(void) {
    //
    // Get the delta time; seconds since last Execute call.
    //
    if (!m_benchmarkingEnabled) {
        boost::chrono::nanoseconds waitFor = boost::chrono::nanoseconds(sm_defaultClockFrequency - m_executionTimer.elapsed().wall);
        if (waitFor.count() >= 0) {
            boost::this_thread::sleep_for(waitFor);
        }
    }
    f32 deltaTime = boost::chrono::duration<f32>(boost::chrono::nanoseconds(m_executionTimer.elapsed().wall)).count();
    m_executionTimer.start();

    //
    // Update instrumentation for this frame.
    // If we do this here, there's no thread sync to worry about since we're single-threaded here.
    //
    m_pTaskManager->updatePeriodicData(deltaTime);
    
#ifdef DEBUG_BUILD
    //
    // Update the debugger
    // 
    //Singletons::Debugger.update(DeltaTime);
#endif

    //
    // Check if the execution is paused, and set delta time to 0 if so.
    //
    if (m_runtimeService->isPaused()) {
        deltaTime = 0.0f;
    }

    //
    // Schedule the scenes that are ready for execution.
    //
    ISystemTask* aScenesToExecute[Proto::SystemType_MAX];
    u32 cScenesToExecute = 0;

    for (auto it = m_SceneExecs.begin(); it != m_SceneExecs.end(); it++) {
        ASSERT(cScenesToExecute < Proto::SystemType_MAX);
        aScenesToExecute[cScenesToExecute++] = it->second->GetSystemTask<ISystemTask>();
    }

    m_pTaskManager->IssueJobsForSystemTasks(aScenesToExecute, cScenesToExecute, deltaTime);

    //
    // Wait for the scenes that will be completing execution in this frame.
    //
    m_pTaskManager->WaitForSystemTasks(aScenesToExecute, cScenesToExecute);
}
