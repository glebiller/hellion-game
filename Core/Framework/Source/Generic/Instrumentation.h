// Copyright © 2008-2009 Intel Corporation
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

#include "System/Types.h"

/**
* The Instrumentation class collects and records performance stats about the various parts of
* the system.
* It is hooked in to the framework and is updated from within the scheduler. It caches
* performance stats until they are needed for display.
* This class implements the service Instrumentation interface, and is a Singleton.
*/
class Instrumentation {
public:

    /**
     * Basic constructor.
     */
    Instrumentation(void);

    /**
     * Basic destructor.
     */
    ~Instrumentation();

    /**
     * Updates the periodic data described by deltaTime.
     * This function drives the instrumentation; it's called regularly from the scheduler in the
     * framework, to let the instrumentation know that another frame has elapsed.  Whenever it is
     * called, this function will make sure that the cached stats used by the instrumentation are up
     * to date; if they've gotten too old, it will refresh those stats.
     *
     * @param   deltaTime   f32 - Elapsed wall-clock time since the last call to this function.
     */
    void UpdatePeriodicData(f32 deltaTime);

    /**
     * Get the number of available CPUs in the system, including physical and logical CPUs.
     *
     * @return  i32 - Number of available CPUs in the system.
     */
    i32 getCPUCount() {
        return m_CPUCount;
    };

    /**
     * Get the most recently-measured frame rate (in Frames Per Second).  This value is
     * automatically refreshed.  It is averaged over the last few frames (defined by the update
     * interval in this class).
     *
     * @return  f32 - Frame rate, in frames per second.
     *
     * ### sa   Instrumentation::m_secondsPerUpdate .
     */
    f32 getCurrentFPS() {
        return m_currentFPS;
    };

    /**
     * Get the number of CPU performance counters that we're using.  There will be one for each
     * available CPU, and one more for the total.
     *
     * @return  i32 - Number of CPU performance counters, which will be returned by the
     *          getCPUCounters call.
     */
    i32 getNumCounters() {
        return m_numCounters;
    };

    /**
     * Get the most recently measured CPU counters.  This value is automatically refreshed.
     *
     * @param   CPUPercent  f64* - Array, filled by this call, of CPU counters showing percent CPU
     *                      load. Must be big enough to hold all counters, see
     *                      Instrumentation::getNumCounters().
     */
    void getCPUCounters(f64* CPUPercent) {
        ASSERT(CPUPercent != NULL);

        if (CPUPercent != NULL) {
            for (int i = 0; i < m_numCounters; i++) {
                CPUPercent[i] = m_CPUPercentCounters[i];
            }
        }

        return;
    };

    /**
     * Set the number of threads the application will now run.
     *
     * @param   activeThreadCount   i32 - Number of active threads the app should use now.
     */
    void setActiveThreadCount(i32 activeThreadCount) {
        m_activeThreadCount = activeThreadCount;
    }

    /**
     * Get the number of threads that we're currently using in this application.
     *
     * @return  i32 - Current active thread count.
     */
    i32 getActiveThreadCount() {
        return m_activeThreadCount;
    }

    /**
     * Called when some job has finished.  Keep track of how much time this job has spent in this
     * frame. There may be many jobs of one type passed in during a single frame; their results will
     * be appended.
     *
     * @param   jobType         u32 - The type of the job that has just completed; a member of
     * 							Proto::SystemType.
     * @param   jobCounterTicks i64 - The number of clock ticks, from _RDTSC, that this job used
     *                          during this frame.
     */
    void CaptureJobCounterTicks(Proto::SystemType jobType, i64 jobCounterTicks) {
        u32 jobIndex = System::Types::GetIndex(jobType);
        if (jobIndex < Proto::SystemType_MAX) {
            //******************************
            // GDC - LAB 4 - Activity 3
            //
            // This accumulator seems to introduce a data race condition.
            // Hint: Maybe an atomic update?
            // Hint: Perhaps ::InterlockedExchangeAdd()?
            // Can get compiler warnings about loss of precision; only really using low 32 bits of i64 value.
#pragma warning ( push )
#pragma warning ( disable : 4244 )
            m_pAccumulatingFrameTicks[jobIndex] += (long)jobCounterTicks;
#pragma warning ( pop )
            //
            //******************************
        }
    }

    /**
     * Get the max number of job types possible in the system, so the caller can allocate the right
     * sized array.
     *
     * @return  i32 - Max number of job types.
     */
    i32 getJobCount() {
        return (i32) Proto::SystemType_MAX;
    }

    /**
     * Get the ratios of job work done in this most recent frame.
     *
     * @param   jobRatios   f32* - Array that this function should fill with the ratios of time spent
     *                      in each workload on this frame. Must be the right length; call
     *                      getJobCount.
     */
    void getJobRatios(f32* jobRatios) {
        for (int i = Proto::SystemType_MAX; i < Proto::SystemType_MAX; i++) {
            jobRatios[i] = m_pLastFrameRatio[i];
        }
    }

private:
    f32         m_currentFPS;
    i32         m_CPUCount;
    i32         m_numCounters;
    f64*        m_CPUPercentCounters;
    i32         m_activeThreadCount;

    /**
     * The seconds per update.
     * Update interval - this is how often this object will refresh its data from its sources.
     */
    static const f32    m_secondsPerUpdate;

    f32         m_secondsSinceLastUpdate;
    i32         m_framesSinceLastUpdate;

    i64         m_LastUpdateTick;

    i64*        m_pAccumulatingFrameTicks;
    f32*        m_pLastFrameRatio;
    std::vector<void*> m_vecProcessorCounters;

    // Index of the performance object called "Processor" in English.
    // From registry, in HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\PerfLib\<your language ID>.
    static const i32    m_processorObjectIndex = 238;

    // Take a guess at how long the name could be in all languages of the "Processor" counter object.
    static const i32    m_processorObjectNameMaxSize = 128;
};
