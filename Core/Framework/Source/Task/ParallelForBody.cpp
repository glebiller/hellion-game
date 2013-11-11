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
#include <tbb/blocked_range.h>

#include "Assert.h"
#include "Defines.h"
#include "DataTypes.h"
#include "Proto/Common.pb.h"
#include "Generic/IttNotify.h"
#include "Generic/Instrumentation.h"
#include "Manager/TaskManager.h"
#include "Task/ParallelForBody.h"

ParallelForBody::ParallelForBody(Instrumentation* instrumentation, ITaskManager::ParallelForFunction pfn, void* pParam DECLARE_JOB_AND_TP_EVENT_ARGS(jobType, tpEvent))
        : GenericCallbackData(instrumentation, pParam PASS_JOB_AND_TP_EVENT_ARGS(jobType, tpEvent))
        , m_pfnCallback(pfn) {
};

void ParallelForBody::operator() (const tbb::blocked_range<u32>& r) const {
    JOB_TASK_STARTED(m_jobType, m_tpEvent);
    m_pfnCallback(m_pParam, r.begin(), r.end());
    JOB_TASK_FINISHED(m_jobType, m_tpEvent);
}
