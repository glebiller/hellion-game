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

#include <tbb/task.h>

#include "Assert.h"
#include "DataTypes.h"
#include "Manager/TaskManager.h"
#include "Task/SynchronizeTask.h"

Handle SynchronizeTask::m_hAllCallbacksInvokedEvent = NULL;
ITaskManager::JobFunction SynchronizeTask::m_fCallback = NULL;
void* SynchronizeTask::m_pCallbackParam = NULL;
volatile long SynchronizeTask::m_lCallbacksCount = 0;

SynchronizeTask::SynchronizeTask() {
};

tbb::task* SynchronizeTask::execute() {
    ASSERT(m_fCallback != NULL);
    ASSERT(m_hAllCallbacksInvokedEvent != NULL);
    m_fCallback(m_pCallbackParam);

#if defined(MSC_COMPILER)
    if (InterlockedDecrement(&m_lCallbacksCount) == 0) {
        // set all of the SynchronizeTasks free
        SetEvent(m_hAllCallbacksInvokedEvent);
    } else {
        // wait for somebody else to finish up
        WaitForSingleObject(m_hAllCallbacksInvokedEvent, INFINITE);
    }
#endif

    return NULL;
};
