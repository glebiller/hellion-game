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

#include "schema/environment_generated.h"
#include "Manager/TaskManager.h"
#include "Universal/UScene.h"
#include "Generic/Scheduler.h"

// Set the timer to 120Hz
const boost::timer::nanosecond_type Scheduler::sm_defaultClockFrequency =  boost::timer::nanosecond_type(1000000000LL / 120);

Scheduler::Scheduler(TaskManager* taskManager)
      // TODO move new TaskManager responsibility to Framework class
      : m_pTaskManager(taskManager) {
}

Scheduler::~Scheduler() {
    m_pTaskManager->Shutdown();
    delete m_pTaskManager;
}

void Scheduler::init(const Schema::Environment* environment) {
    m_benchmarkingEnabled = environment->schedulerBenchmarking();
    m_pTaskManager->Init(environment);
    m_executionTimer.start();
}

///
/// @inheritDoc
///
void Scheduler::setScene(const UScene* pScene) {
#ifdef DEBUG_BUILD
    //Singletons::Debugger.setScene(pScene);
#endif

    const UScene::SystemScenes& SystemScenes = pScene->GetSystemScenes();
    for (auto it = SystemScenes.begin(); it != SystemScenes.end(); it++) {
        if (it->second->GetSystemTask<ISystemTask>() != NULL) {
            m_SceneExecs[ it->first ] = it->second;
        }
    }

    m_executionTimer.start();
}

///
/// @inheritDoc
///
void Scheduler::execute() {
    //
    // Get the delta time; seconds since last Execute call.
    //
    if (!m_benchmarkingEnabled) {
        boost::chrono::nanoseconds waitFor = boost::chrono::nanoseconds(sm_defaultClockFrequency - m_executionTimer.elapsed().wall);
        if (waitFor.count() >= 0) {
            boost::this_thread::sleep_for(waitFor);
        }
    }
    float deltaTime = boost::chrono::duration<float>(boost::chrono::nanoseconds(m_executionTimer.elapsed().wall)).count();
    m_executionTimer.start();

    //
    // Update instrumentation for this frame.
    // If we do this here, there's no thread sync to worry about since we're single-threaded here.
    //
    //m_pTaskManager->updatePeriodicData(deltaTime);
    
#ifdef DEBUG_BUILD
    //Singletons::Debugger.update(DeltaTime);
#endif

    // TODO
    /*if (m_runtimeService->isPaused()) {
        deltaTime = 0.0f;
    }*/

    //
    // Schedule the scenes that are ready for execution.
    //
    unsigned int cScenesToExecute = 0;
    ISystemTask* aScenesToExecute[m_SceneExecs.size()];
    for (auto it = m_SceneExecs.begin(); it != m_SceneExecs.end(); it++) {
        aScenesToExecute[cScenesToExecute++] = it->second->GetSystemTask<ISystemTask>();
    }

    m_pTaskManager->IssueJobsForSystemTasks(aScenesToExecute, cScenesToExecute, deltaTime);
    m_pTaskManager->WaitForSystemTasks(aScenesToExecute, cScenesToExecute);
}

///
/// @inheritDoc
///
void Scheduler::waitForScenes() {
    ISystemTask* aScenesToWaitFor[m_SceneExecs.size()];
    unsigned int cScenesToWaitFor = 0;

    for (auto it = m_SceneExecs.begin(); it != m_SceneExecs.end(); it++) {
        aScenesToWaitFor[ cScenesToWaitFor++ ] = it->second->GetSystemTask<ISystemTask>();
    }

    m_SceneExecs.clear();

    if (cScenesToWaitFor > 0) {
        m_pTaskManager->WaitForSystemTasks(aScenesToWaitFor, cScenesToWaitFor);
    }
}

