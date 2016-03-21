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

#include <boost/assert.hpp>
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
    Instrumentation();

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
     * @param   deltaTime   float - Elapsed wall-clock time since the last call to this function.
     */
    void UpdatePeriodicData(float deltaTime);

    /**
     * Get the number of available CPUs in the system, including physical and logical CPUs.
     *
     * @return  int - Number of available CPUs in the system.
     */
    int getCPUCount() {
        return m_CPUCount;
    };

    /**
     * Get the most recently-measured frame rate (in Frames Per Second).  This value is
     * automatically refreshed.  It is averaged over the last few frames (defined by the update
     * interval in this class).
     *
     * @return  float - Frame rate, in frames per second.
     *
     * ### sa   Instrumentation::m_secondsPerUpdate .
     */
    float getCurrentFPS() {
        return m_currentFPS;
    };

    /**
     * Get the number of CPU performance counters that we're using.  There will be one for each
     * available CPU, and one more for the total.
     *
     * @return  int - Number of CPU performance counters, which will be returned by the
     *          getCPUCounters call.
     */
    int getNumCounters() {
        return m_numCounters;
    };

    /**
     * Get the most recently measured CPU counters.  This value is automatically refreshed.
     *
     * @param   CPUPercent  double* - Array, filled by this call, of CPU counters showing percent CPU
     *                      load. Must be big enough to hold all counters, see
     *                      Instrumentation::getNumCounters().
     */
    void getCPUCounters(double* CPUPercent) {
        BOOST_ASSERT(CPUPercent != NULL);

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
     * @param   activeThreadCount   int - Number of active threads the app should use now.
     */
    void setActiveThreadCount(int activeThreadCount) {
        m_activeThreadCount = activeThreadCount;
    }

    /**
     * Get the number of threads that we're currently using in this application.
     *
     * @return  int - Current active thread count.
     */
    int getActiveThreadCount() {
        return m_activeThreadCount;
    }

    /**
     * Called when some job has finished.  Keep track of how much time this job has spent in this
     * frame. There may be many jobs of one type passed in during a single frame; their results will
     * be appended.
     *
     * @param   jobType         unsigned int - The type of the job that has just completed; a member of
     * 							Proto::SystemType.
     * @param   jobCounterTicks i64 - The number of clock ticks, from _RDTSC, that this job used
     *                          during this frame.
     */
    void CaptureJobCounterTicks(Schema::SystemType jobType, long long jobCounterTicks) {
        unsigned int jobIndex = System::Types::GetIndex(jobType);
#if defined(_MSC_VER)
        if (jobIndex < Proto::SystemType) {
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
#endif
    }

    /**
     * Get the max number of job types possible in the system, so the caller can allocate the right
     * sized array.
     *
     * @return  int - Max number of job types.
     */
    int getJobCount() {
        return (int) Schema::SystemType::MAX ;
    }

    /**
     * Get the ratios of job work done in this most recent frame.
     *
     * @param   jobRatios   float* - Array that this function should fill with the ratios of time spent
     *                      in each workload on this frame. Must be the right length; call
     *                      getJobCount.
     */
    void getJobRatios(float* jobRatios) {
        int systemCount = static_cast<unsigned int> (Schema::SystemType::MAX);
        for (int i = 0; i < systemCount; i++) {
            jobRatios[i] = m_pLastFrameRatio[i];
        }
    }

private:
    float         m_currentFPS;
    int         m_CPUCount;
    int         m_numCounters;
    double*        m_CPUPercentCounters;
    int         m_activeThreadCount;

    /**
     * The seconds per update.
     * Update interval - this is how often this object will refresh its data from its sources.
     */
    static const float    m_secondsPerUpdate;

    float         m_secondsSinceLastUpdate;
    int         m_framesSinceLastUpdate;

    long long         m_LastUpdateTick;

    long long*        m_pAccumulatingFrameTicks;
    float*        m_pLastFrameRatio;
    std::vector<void*> m_vecProcessorCounters;

    // Index of the performance object called "Processor" in English.
    // From registry, in HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\PerfLib\<your language ID>.
    static const int    m_processorObjectIndex = 238;

    // Take a guess at how long the name could be in all languages of the "Processor" counter object.
    static const int    m_processorObjectNameMaxSize = 128;
};
