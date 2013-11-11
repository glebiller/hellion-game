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

#include "Defines.h"
#include "DataTypes.h"
#include "Generic/IttNotify.h"

class Instrumentation;
class TaskManager;

/**
 * Base class containing data common to classes handling different callbacks.
 */
class GenericCallbackData {
public:

    /**
     * Constructor.
     *
     * @param [in,out]  instrumentation If non-null, the instrumentation.
     * @param [in,out]  pParam          If non-null, the parameter.
     * @param   parameter3              The third parameter.
     */
    GenericCallbackData(Instrumentation* instrumentation, void* pParam DECLARE_JOB_AND_TP_EVENT_ARGS(jobType, tpEvent));

protected:
    Instrumentation*    m_instrumentation;
    void*               m_pParam;

#if defined(STATISTICS_BY_JOB_TYPE)
    Proto::SystemType   m_jobType;
#endif

#if defined(USE_THREAD_PROFILER)
    __itt_event         m_tpEvent;
#endif

};