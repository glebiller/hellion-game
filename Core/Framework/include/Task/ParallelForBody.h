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
#include <tbb/blocked_range.h>

#include "Defines.h"
#include "DataTypes.h"
#include "Generic/IttNotify.h"
#include "Manager/ITaskManager.h"
#include "Task/GenericCallbackData.h"

class Instrumentation;
class TaskManager;

/**
 * Parallel for body.
 *
 * @sa  GenericCallbackData
 */
class ParallelForBody : public GenericCallbackData {
public:

    /**
     * Constructor.
     *
     * @param [in,out]  instrumentation                                 If non-null, the
     *                                                                  instrumentation.
     * @param   pfn                                                     The pfn.
     * @param [in,out]  DECLARE_JOB_AND_TP_EVENT_ARGS(jobType,tpEvent)  If non-null, the declare job
     *                                                                  and TP event arguments (job
     *                                                                  type,tp event)
     */
    ParallelForBody(Instrumentation* instrumentation, ITaskManager::ParallelForFunction pfn, void* pParam DECLARE_JOB_AND_TP_EVENT_ARGS(jobType, tpEvent));

    /**
     *  casting operator.
     */
    void operator() (const tbb::blocked_range<u32>& r) const;

private:
    ITaskManager::ParallelForFunction m_pfnCallback;

};