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
#include <boost/timer/timer.hpp>

#include "DataTypes.h"
#include "Task/GenericCallbackData.h"

class TaskManager;

/**
 * This class is used by <c>TaskManager</c> to execute a generic callback.
 *
 * @sa  tbb::task
 * @sa  GenericCallbackData
 */
template<class Fptr> 
class GenericCallbackTask : public tbb::task, public GenericCallbackData {
public:

    /**
     * Constructor.
     *
     * @param [in,out]  instrumentation         If non-null, the instrumentation.
     * @param   fFunc                           The function.
     * @param [in,out]  pParam                  If non-null, the parameter.
     * @param   DECLARE_TP_EVENT_ARG(tpEvent)   The declare TP event argument (tp event)
     */
    GenericCallbackTask(Instrumentation* instrumentation, Fptr fFunc, void* pParam, Schema::SystemType jobType)
        : GenericCallbackData(instrumentation, pParam, jobType)
        , callbackFunction_(fFunc) {
    };

    /**
     * Gets the execute.
     *
     * @return  null if it fails, else.
     */
    virtual tbb::task* execute() {
        BOOST_ASSERT(callbackFunction_ != NULL);
        boost::timer::cpu_timer counter;
        callbackFunction_(m_pParam);
        m_instrumentation->CaptureJobCounterTicks(m_jobType, counter.elapsed().wall);
        return NULL;
    };

protected:
    Fptr                callbackFunction_;

};