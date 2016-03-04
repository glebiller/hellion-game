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

#include "DataTypes.h"

class ISystemTask;

/**
 * <c>ITaskManager</c> is an interface class for linking ISystemTasks to a TaskManager.
 */
class ITaskManager {
public:

    /**
     * Defines an alias representing the void*.
     * This type defines the callback used for generic jobs by the <c>ITaskManager</c>.
     */
    typedef void (*JobFunction)(void*);

    /**
     * Defines an alias representing the void*.
     * This type defines the callback used for generic job completions by the <c>ITaskManager</c>.
     * ### return   the number of jobs issued by this function.
     */
    typedef u32(*JobCompletionFunction)(void*);

    /**
     * Defines an alias representing the end.
     */
    typedef void (*ParallelForFunction)(void* param, u32 begin, u32 end);

    /**
     * Callback, called when the non standard per thread.
     * This method triggers a synchronized callback to be called once by each thread used by the
     * <c>ITaskManager</c>. This method which should only be called during initialization and
     * shutdown of the <c>ITaskManager</c>.  This method waits until all callbacks have executed.
     *
     * @param   pfnCallback the function callback to execute.
     * @param   pData       a pointer to data that is passed to the callback.
     */
    virtual void NonStandardPerThreadCallback(JobFunction pfnCallback, void* pData) = 0;

    /**
     * Values that represent JobCountInstructionHints.
     * This type provides hints to the <c>GetRecommendedJobCount</c> method on the type of work
     * about to be submitted to the <c>ITaskManager</c>.
     * @sa  ITaskManager::GetRecommendedJobCount    .
     */
    enum JobCountInstructionHints {
        None, Generic, FP, SIMD_FP, SIMD_INT,
    };

    /**
     * Gets a recommended job count.
     * Call this method to determine the ideal number of tasks to submit to the <c>ITaskManager</c>
     * for maximum performance.
     *
     * @param   Hints   guidance on the type of work done in the jobs about to be submitted.
     * @return  the number of jobs which is optimal for the type of work specified by
     *          <paramref name="Hints"/>
     */
    virtual u32 GetRecommendedJobCount(JobCountInstructionHints Hints = None) = 0;

    /**
     * Sets a number of threads.
     * This method constrains the number of threads used by the <c>ITaskManager</c>.
     *
     * @param   uNumberOfThreads    the limit of the number of threads to use.
     */
    virtual void SetNumberOfThreads(u32 uNumberOfThreads) = 0;

    /**
     * Gets the number of threads.
     * Call this method to get the number of threads in the thread pool which are active for running
     * work.
     *
     * @return  the number of threads being used.
     */
    virtual u32 GetNumberOfThreads() = 0;

    /**
     * Parallel for virtual function.
     *
     * @param [in,out]  pSystemTask If non-null, the system task.
     * @param   pfnJobFunction      The pfn job function.
     * @param [in,out]  pParam      If non-null, the parameter.
     * @param   begin               The begin.
     * @param   end                 The end.
     * @param   minGrain            (optional) the minimum grain.
     */
    virtual void ParallelFor(ISystemTask* pSystemTask, ParallelForFunction pfnJobFunction, void* pParam, u32 begin, u32 end, u32 minGrain = 1) = 0;

};
