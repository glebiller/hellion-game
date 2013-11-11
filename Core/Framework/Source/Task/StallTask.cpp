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
#include "Task/StallTask.h"

StallTask::StallTask(TaskManager* pTaskManager, Handle hWaitFor)
    : m_pTaskManager(pTaskManager)
    , m_hWaitFor(hWaitFor) {

};

tbb::task* StallTask::execute() {
    if (m_pTaskManager->IsPrimaryThread()) {
        // we can't stall, so stall somebody else
        m_pTaskManager->AddStallTask();
        // wait for a tenth of a second to encourage somebody else to grab the stall task
        tbb::this_tbb_thread::sleep(tbb::tick_count::interval_t(0.1));
    } else {
        ASSERT(m_hWaitFor != NULL);
#if defined(MSC_COMPILER)
        WaitForSingleObject(m_hWaitFor, INFINITE);
#endif
    }

    return NULL;
};