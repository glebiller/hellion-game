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

#include <tbb/task.h>
#include <mutex>
#include <condition_variable>

#include "DataTypes.h"

class TaskManager;

/**
 * This class is used by <c>TaskManager</c> to stall unused threads.
 *
 * @sa  tbb::task
 */
class StallTask : public tbb::task {
public:

    /**
     * Constructor.
     *
     * @param [in,out]  pTaskManager    If non-null, manager for task.
     * @param   hWaitFor                Handle of the wait for.
     */
    StallTask(TaskManager* pTaskManager, std::shared_ptr<boost::interprocess::named_semaphore> hWaitFor);

    /**
     * Gets the execute.
     *
     * @return  null if it fails, else.
     */
    tbb::task* execute();

protected:
    TaskManager* m_pTaskManager;
    std::shared_ptr<boost::interprocess::named_semaphore> m_hWaitFor;

};