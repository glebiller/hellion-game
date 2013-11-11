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

#include "DataTypes.h"

class TaskManager;

/**
 * This class is used by <c>TaskManager</c> to manage synchronized callbacks.
 *
 * @sa  tbb::task
 * @sa  TaskManager::NonStandardPerThreadCallback   .
 */
class SynchronizeTask: public tbb::task {
public:

    /**
     * Default constructor.
     */
    SynchronizeTask();

    /**
     * Gets the execute.
     *
     * @return  null if it fails, else.
     */
    tbb::task* execute();

    /**
     * Callback, called when the prepare.
     *
     * @param   fFunc           The function.
     * @param [in,out]  pParam  If non-null, the parameter.
     * @param   uCount          Number of.
     */
    static void PrepareCallback(ITaskManager::JobFunction fFunc, void* pParam, u32 uCount) {
        m_fCallback = fFunc;
        m_pCallbackParam = pParam;
        m_lCallbacksCount = uCount;
#if defined(MSC_COMPILER)
        ResetEvent(m_hAllCallbacksInvokedEvent);
#endif
    }

public:
    static Handle m_hAllCallbacksInvokedEvent;
    static ITaskManager::JobFunction m_fCallback;
    static void* m_pCallbackParam;
    static volatile long m_lCallbacksCount;

};