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
#include <boost/timer/timer.hpp>
#include <schema/system_type_generated.h>

#include "Generic/Instrumentation.h"
#include "Manager/TaskManager.h"
#include "Task/ParallelForBody.h"

ParallelForBody::ParallelForBody(Instrumentation* instrumentation, TaskManager::ParallelForFunction pfn, void* pParam, Schema::SystemType jobType)
        : GenericCallbackData(instrumentation, pParam, jobType)
        , m_pfnCallback(pfn) {
};

void ParallelForBody::operator() (const tbb::blocked_range<unsigned int>& r) const {
    boost::timer::cpu_timer counter;
    m_pfnCallback(m_pParam, r.begin(), r.end());
    m_instrumentation->CaptureJobCounterTicks(m_jobType, counter.elapsed().wall);
}
