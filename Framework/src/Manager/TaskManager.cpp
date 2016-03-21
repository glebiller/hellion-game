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

#include <boost/interprocess/sync/named_semaphore.hpp>
#include "schema/environment_generated.h"
#include "Defines.h"
#include "DataTypes.h"

#pragma warning (push)
#pragma warning (disable: 4100)
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/partitioner.h>
#pragma warning (pop)

#if defined(MSC_COMPILER)
    #include <Windows.h>
    #include <process.h>
#endif

#include "System/ISystemTask.h"
#include "Manager/TaskManager.h"
#include "Manager/ServiceManager.h"
#include "Generic/Instrumentation.h"
#include "Task/StallTask.h"
#include "Task/SynchronizeTask.h"
#include "Task/GenericCallbackTask.h"
#include "Task/ParallelForBody.h"
#include "Task/PerformanceHint.h"

const char* const   semaphoreName  = "TaskManagerSemaphore";

namespace local {
    typedef GenericCallbackTask<ITaskManager::JobFunction> SystemTask;
    typedef GenericCallbackTask<ITaskManager::JobFunction> JobTask;
    typedef GenericCallbackTask<ITaskManager::JobCompletionFunction> JobCompletionTask;

    static TaskManager* g_pTaskManager = NULL;
} // namespace local

using namespace local;

/**
 * @inhertiDoc
 */
TaskManager::TaskManager()
    : m_instrumentation(new Instrumentation())
    , m_bTimeToQuit(false)
    , m_uMaxNumberOfThreads(0)
    , m_uNumberOfThreads(0)
    , m_uTargetNumberOfThreads(0)
    , m_uPrimaryThreadID(tbb::this_tbb_thread::get_id()) {

}

/**
 * @inhertiDoc
 */
TaskManager::~TaskManager() {
    delete m_instrumentation;
}

/**
 * @inhertiDoc
 */
void TaskManager::AddStallTask() {
    BOOST_ASSERT(m_pStallPoolParent != NULL);
    tbb::task* pStallTask = new(m_pStallPoolParent->allocate_additional_child_of(*m_pStallPoolParent))
    StallTask(this, m_hStallPoolSemaphore);
    BOOST_ASSERT(pStallTask != NULL);
    m_pStallPoolParent->spawn(*pStallTask);
}

/**
 * @inhertiDoc
 */
void TaskManager::SystemTaskCallback(void* pData) {
    ISystemTask* pTask = static_cast<ISystemTask*>(pData);
    pTask->Update(g_pTaskManager->m_fDeltaTime);
}

/**
 * @inhertiDoc
 */
void TaskManager::InitAffinityData(void* mgr) {
    TaskManager* pThis = static_cast<TaskManager*>(mgr);
    tbb::spin_mutex::scoped_lock _lock(pThis->m_spinMutex);
    pThis->m_affinityIDs.push_back(tbb::task::self().affinity());
}

/**
 * @inhertiDoc
 */
void TaskManager::Init(const Schema::Environment* environment) {
    // Call this from the primary thread before calling any other TaskManager methods.
    g_pTaskManager = this;

    int requestedNumberOfThreads = environment->taskManagerThreads();
    if (requestedNumberOfThreads == 0) {
        // IMPLEMENTATION NOTE
        // The audio thread (which Thread Profiler shows as constantly working)
        // has no negative impact on the performance when it causes apparent
        // oversubscription. Shrinking the pool to avoid this apparent oversubscription
        // results in smaller FPS. So this is probably one of the cases when TP
        // misinterprets the behavior of threads created by some of the Windows
        // subsystems (DirectX is historically its weak place).
        requestedNumberOfThreads = tbb::task_scheduler_init::default_num_threads();
    }

    m_pStallPoolParent = NULL;
    boost::interprocess::named_semaphore::remove(semaphoreName);
    m_hStallPoolSemaphore = new boost::interprocess::named_semaphore(
            boost::interprocess::create_only_t(), semaphoreName, requestedNumberOfThreads
    );
#if defined(USE_THREAD_PROFILER)
    ISettingService* settingService = IServiceManager::get()->getSettingService();
    m_bTPEventsForTasks = settingService->getBool("TaskManager::TPEventsForTasks");
    m_bTPEventsForJobs = settingService->getBool("TaskManager::TPEventsForJobs");
    m_bTPEventsForSynchronize = settingService->getBool("TaskManager::TPEventsForSynchronize");

    if (m_bTPEventsForSynchronize) {
        m_tSynchronizeTPEvent = __itt_event_createA("Synchronize", 11);
    }

    m_tpSystemTaskSpawn = __itt_event_createA("SystemTaskSpawn", 15);
#endif /* USE_THREAD_PROFILER */
    m_uMaxNumberOfThreads = requestedNumberOfThreads;
    m_uTargetNumberOfThreads = requestedNumberOfThreads;
    m_uNumberOfThreads = requestedNumberOfThreads;
    m_pTbbScheduler = new tbb::task_scheduler_init(requestedNumberOfThreads);
    m_pSystemTasksRoot = new(tbb::task::allocate_root()) tbb::empty_task;
    NonStandardPerThreadCallback(InitAffinityData, this);
#ifdef STATISTICS_BY_JOB_TYPE
    m_instrumentation->setActiveThreadCount(requestedNumberOfThreads);
#endif
}

/**
 * @inhertiDoc
 */
void TaskManager::Shutdown() {
    // Call this from the primary thread as the last TaskManager call.
    BOOST_ASSERT(IsPrimaryThread());
    // get the callback thread to exit
    m_bTimeToQuit = true;
    // trigger the release of the stall pool
    boost::interprocess::named_semaphore::remove(semaphoreName);
    m_pSystemTasksRoot->destroy(*m_pSystemTasksRoot);
    delete m_pTbbScheduler;
    delete m_hStallPoolSemaphore;
    // now get rid of all the events
#ifdef USE_THREAD_PROFILER
    m_SupportForSystemTasks.clear();
#endif
}

/**
 * @inhertiDoc
 */
void TaskManager::updatePeriodicData(float deltaTime) {
#ifdef STATISTICS_BY_JOB_TYPE
    m_instrumentation->UpdatePeriodicData(deltaTime);
#endif
}

/**
 * @inhertiDoc
 */
void TaskManager::IssueJobsForSystemTasks(ISystemTask** pTasks, unsigned int uTaskCount, float fDeltaTime) {
    // Call this from the primary thread to schedule system work.
    BOOST_ASSERT(IsPrimaryThread());
    __ITT_EVENT_START(m_tpSystemTaskSpawn, PROFILE_TASKMANAGER);
    BOOST_ASSERT(uTaskCount > 0);
    m_fDeltaTime = fDeltaTime;
    UpdateThreadPoolSize();
    BOOST_ASSERT(m_pSystemTasksRoot != NULL);
    // We'll be adding tasks to the m_pSystemTasksRoot, and we will eventually call wait_for_all on it.
    // Support the eventual wait_for_all by setting reference count to 1 now.
    m_pSystemTasksRoot->set_ref_count(1);
    // now schedule the tasks, based upon their PerformanceHint order
    tbb::task_list tTaskList;
    unsigned int uAffinityCount = (unsigned int)m_affinityIDs.size();

    unsigned int h = 0;
    unsigned int i = 0;
    for (h = 0; h < Task::Task_MAX; h++) {
        for (i = 0; i < uTaskCount; i++) {
            if (pTasks[i]->IsPrimaryThreadOnly()) {
                // put this task on the list of tasks to be run on the primary thread
                // only do this during the first outer loop
                if (h == 0) {
                    m_primaryThreadSystemTaskList.push_back(pTasks[i]);
                }
            } else {
                // see if it's time to dispatch this task
                if (Task::getPerformanceHint(pTasks[i]->GetSystemType()) == (Task::PerformanceHint) h) {
                    // this task can be run on an arbitrary thread -- allocate it
                    SystemTask* pSystemTask = new(tbb::task::allocate_additional_child_of(*m_pSystemTasksRoot))
                    SystemTask(m_instrumentation, SystemTaskCallback, pTasks[i] 
                        PASS_JOB_AND_TP_EVENT_ARGS(pTasks[i]->GetSystemType(), GetSupportForSystemTask(pTasks[i]).m_tpeSystemTask));
                    // affinity will increase the chances that each SystemTask will be assigned
                    // to a unique thread, regardless of PerformanceHint
                    BOOST_ASSERT(pSystemTask != NULL);
                    pSystemTask->set_affinity(m_affinityIDs[i % uAffinityCount]);
                    tTaskList.push_back(*pSystemTask);
                }
            }
        }
    }

    // We only spawn system tasks here. They in their turn will spawn descendant tasks.
    // Waiting for the whole bunch completion happens in WaitForSystemTasks.
    m_pSystemTasksRoot->spawn(tTaskList);
    __ITT_EVENT_END(m_tpSystemTaskSpawn, PROFILE_TASKMANAGER);
}

/**
 * @inhertiDoc
 */
void TaskManager::NonStandardPerThreadCallback(JobFunction pfnCallback, void* pData) {
    // This method triggers a synchronized callback to be called once by each thread used
    // by the TaskManager.  This method waits until all callbacks have executed.
    // only one at a time here
    tbb::spin_mutex::scoped_lock _lock(m_tSynchronizedCallbackMutex);
    __ITT_EVENT_START(m_tSynchronizeTPEvent, PROFILE_TASKMANAGER);
    int uNumberOfThreads = m_uNumberOfThreads;

    if (uNumberOfThreads != m_uMaxNumberOfThreads) {
        m_uTargetNumberOfThreads = m_uMaxNumberOfThreads;
        UpdateThreadPoolSize();
    }

    SynchronizeTask::PrepareCallback(pfnCallback, pData, m_uMaxNumberOfThreads);
    tbb::task* pBroadcastParent = new(tbb::task::allocate_root()) tbb::empty_task;
    BOOST_ASSERT(pBroadcastParent != NULL);
    // we have one reference for each thread, plus one for the wait_for_all below
    pBroadcastParent->set_ref_count(m_uMaxNumberOfThreads + 1);
    tbb::task_list tList;

    for (int i = 0; i < m_uMaxNumberOfThreads; i++) {
        // Add a SynchronizedTasks for each thread in the TBB pool (workers + this master)
        tbb::task* pNewTask = new(pBroadcastParent->allocate_child()) SynchronizeTask;
        BOOST_ASSERT(pNewTask != NULL);
        tList.push_back(*pNewTask);
    }

    // get the synchronize tasks running
    pBroadcastParent->spawn_and_wait_for_all(tList);
    pBroadcastParent->destroy(*pBroadcastParent);

    if (uNumberOfThreads != m_uMaxNumberOfThreads) {
        m_uTargetNumberOfThreads = uNumberOfThreads;
        UpdateThreadPoolSize();
    }

    __ITT_EVENT_END(m_tSynchronizeTPEvent, PROFILE_TASKMANAGER);
} 

/**
 * @inhertiDoc
 */
unsigned int TaskManager::GetRecommendedJobCount(ITaskManager::JobCountInstructionHints Hints) {
    // Call this method to determine the ideal number of tasks to submit to the TaskManager
    // for maximum performance.
    //
    // Ignoring hints for now and just returning the number of available threads.
    //
    return m_uNumberOfThreads;
}

/**
 * @inhertiDoc
 */
void TaskManager::ParallelFor(ISystemTask* pSystemTask, ParallelForFunction pfnJobFunction, void* pParam, unsigned int begin, unsigned int end, unsigned int minGrainSize) {
#if defined(STATISTICS_BY_JOB_TYPE)
    // ??? How often does this fail over to NULL?
    Schema::SystemType jobType = pSystemTask ? pSystemTask->GetSystemType() : Schema::SystemType::MIN;
#endif
#if defined(USE_THREAD_PROFILER)
    __itt_event tpEvent = GetSupportForSystemTask(pSystemTask).m_tpeSystemTaskJob;
#endif
    ParallelForBody body(m_instrumentation, pfnJobFunction, pParam
                         PASS_JOB_AND_TP_EVENT_ARGS(jobType, tpEvent));

    if (m_uNumberOfThreads != 1) {
        tbb::parallel_for(tbb::blocked_range<unsigned int>(begin, end, minGrainSize), body, tbb::auto_partitioner());
        //tbb::parallel_for( tbb::blocked_range<unsigned int>( begin, end, minGrainSize ), body, tbb::simple_partitioner() );
    } else {
        body(tbb::blocked_range<unsigned int>(begin, end, minGrainSize));
    }
}

/**
 * @inhertiDoc
 */
void TaskManager::WaitForSystemTasks(ISystemTask** pTasks, unsigned int uTaskCount) {
    // Call this from the primary thread to wait until specified tasks spawned with IssueJobsForSystemTasks
    // and all of their subtasks are complete.
    BOOST_ASSERT(IsPrimaryThread());
    BOOST_ASSERT(uTaskCount > 0);
    BOOST_ASSERT(uTaskCount <= System::Types::MAX);
    // if we are waiting for any of the tasks dedicated to the primary thread,
    // then execute them now
    // if we issued primary thread tasks that we're not waiting for this time,
    // then save them for the next time
#if defined(MSC_COMPILER)
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
#endif
    for (auto systemTask : m_primaryThreadSystemTaskList) {
        // see if we are waiting for this task
        if (std::find(pTasks, pTasks + uTaskCount, systemTask)) {
            // we are, so execute it now on the primary thread
            __ITT_EVENT_START(GetSupportForSystemTask(systemTask).m_tpeSystemTask, PROFILE_TASKMANAGER);
            systemTask->Update(m_fDeltaTime);
            __ITT_EVENT_END(GetSupportForSystemTask(systemTask).m_tpeSystemTask, PROFILE_TASKMANAGER);
        } else {
            // save it for next time
            m_tmpTaskList.push_back(systemTask);
        }
    }
#if defined(MSC_COMPILER)
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
#endif
    m_primaryThreadSystemTaskList.clear();
    m_primaryThreadSystemTaskList.swap(m_tmpTaskList);

    // contribute to the parallel calculation, and when it completes, we're done
    if (m_pSystemTasksRoot->ref_count() == 0) {
        return;
    }

    m_pSystemTasksRoot->wait_for_all();
}

/**
 * @inhertiDoc
 */
void TaskManager::SetNumberOfThreads(unsigned int uNumberOfThreads) {
    // This method constrains the number of threads used by the TaskManager.
    unsigned int uTargetNumberOfThreads = uNumberOfThreads;

    if (uNumberOfThreads > m_uMaxNumberOfThreads || uTargetNumberOfThreads == 0) {
        uTargetNumberOfThreads = m_uMaxNumberOfThreads;
    }

    m_uTargetNumberOfThreads = uTargetNumberOfThreads;
}


#if defined(USE_THREAD_PROFILER)

#define InitTpEventsForSystem(base_name, base_len)                                                                  \
    if( m_bTPEventsForTasks ) {                                                                                     \
        support.m_tpeSystemTask = __itt_event_createA( base_name, base_len );                                       \
    }                                                                                                               \
    if( m_bTPEventsForJobs ) {                                                                                      \
        support.m_tpeSystemTaskJob = __itt_event_createA( base_name"Job", base_len + 3 );                           \
        support.m_tpeSystemTaskJobCompletion = __itt_event_createA( base_name"JobCompletion", base_len + 13 );      \
    }

/**
 * @inhertiDoc
 */
TaskManager::SystemTaskSupport& TaskManager::GetSupportForSystemTask(ISystemTask* pTask) {
    if (!pTask) {
        static SystemTaskSupport emptySupport;
        return emptySupport;
    }

    SystemTaskSupport& support = m_SupportForSystemTasks[pTask];

    if (!support.m_bInitialized) {
        // figure out the most appropriate Thread Profiler event to map to this System
        support.m_bInitialized = true;

        switch (pTask->GetSystemScene()->GetSystem()->GetSystemType()) {
            case (System::Types::Geometry):
                InitTpEventsForSystem("Geometry", 8);
                break;

            case (System::Types::Graphics):
                InitTpEventsForSystem("Graphics", 8);
                break;

            case (System::Types::PhysicsCollision):
                InitTpEventsForSystem("Physics", 7);
                break;

            case (System::Types::Audio):
                InitTpEventsForSystem("Audio", 5);
                break;

            case (Proto::SystemType::Input):
                InitTpEventsForSystem("Input", 5);
                break;

            case (System::Types::AI):
                InitTpEventsForSystem("AI", 2);
                break;

            case (System::Types::Animation):
                InitTpEventsForSystem("Animation", 9);
                break;

            case (System::Types::Scripting):
                InitTpEventsForSystem("Scripting", 9);
                break;

                // NOTE: these next cases mimic the inline call to System::Types::MakeCustom
            case (1 << (16 + 0)):
                InitTpEventsForSystem("ProceduralTrees", 15);
                break;

            case (1 << (16 + 2)):
                InitTpEventsForSystem("ProceduralFire", 14);
                break;

            default:
                break;
        }
    }

    return support;
}

#endif /* USE_THREAD_PROFILER */

/**
 * @inhertiDoc
 */
bool TaskManager::IsPrimaryThread() {
    return (tbb::this_tbb_thread::get_id() == m_uPrimaryThreadID);
}

/**
 * @inhertiDoc
 */
bool TaskManager::IsTBBThread() {
    // This method is used to determine if the calling thread is an Intel Threading Building Blocks thread.
#ifdef DEBUG_BUILD
    // If called not from TBB thread task::self() will assert itself
    return &tbb::task::self() != NULL;
#else
    return true;
#endif
}

/**
 * @inhertiDoc
 */
void TaskManager::UpdateThreadPoolSize() {
    // This method is called within IssueJobsForSystemTasks to update the size of TBB thread pool
    // if necessary.  The thread pool is resized by a call to SetNumberOfThreads.

    // this is accomplished by creating some tasks which do not complete until signaled
    // see if we should adjust the WaitTasks in the stall pool
    if (m_uTargetNumberOfThreads != m_uNumberOfThreads) {
        unsigned int uNumThreadsToWait = (m_uMaxNumberOfThreads - m_uTargetNumberOfThreads);
        unsigned int uNumThreadsToFree = (m_uMaxNumberOfThreads - m_uNumberOfThreads);

        // free up all the threads
        if (m_pStallPoolParent) {
#if defined(MSC_COMPILER)
            // TODO
            ReleaseSemaphore(m_hStallPoolSemaphore, uNumThreadsToFree, NULL);
#endif
            // Make sure there are no stall tasks from the previous cycle lingering
            // out there and competing for the semaphore
            m_pStallPoolParent->wait_for_all();
        } else {
            // make a new stall parent
            m_pStallPoolParent = new(tbb::task::allocate_root()) tbb::empty_task;
        }

        BOOST_ASSERT(m_pStallPoolParent != NULL);
        m_pStallPoolParent->set_ref_count(uNumThreadsToWait + 1);
        tbb::task_list tList;

        for (unsigned int i = 0; i < uNumThreadsToWait; i++) {
            tbb::task* pStallTask = new(m_pStallPoolParent->allocate_child()) StallTask(this, m_hStallPoolSemaphore);
            BOOST_ASSERT(pStallTask != NULL);
            tList.push_back(*pStallTask);
        }

        m_pStallPoolParent->spawn(tList);
        m_uNumberOfThreads = m_uTargetNumberOfThreads;
#ifdef STATISTICS_BY_JOB_TYPE
        m_instrumentation->setActiveThreadCount(m_uTargetNumberOfThreads);
#endif
    }
}
