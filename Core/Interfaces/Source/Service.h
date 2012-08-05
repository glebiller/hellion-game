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

//
// forward declarations
//
class ISystemObject;

#include <MathUtils.h>

#include "Service/CollisionAPI.h"
#include "Property.h"
#include "SystemTypes.h"

/**
 * Interface class for providing services to the systems.
 */
class IService {
    public:

        /**
         * Interface class for providing access to other systems' ISystem, ISystemScene, and
         *  ISystemObject.
         */
        class ISystemAccess {
            public:

                /**
                 * Gets a system by its name.
                 *
                 * @param   pszSystemName   The name of the system to get.
                 * @return  A handle to the system or null if the system doesn't exist.
                 */
                virtual Handle GetSystem(const char* pszSystemName) = 0;

                /**
                 * Gets a system by its type.
                 *
                 * @param   Type    The type of the system to get.
                 * @return  A handle to the system or null if the system doesn't exist.
                 */
                virtual Handle GetSystem(System::Type Type) = 0;

                /**
                 * Gets a system scene by its system name.
                 *
                 * @param   pszSystemName   The name of the system scene to get.
                 * @return  A handle to the system scene or null if it doesn't exist.
                 */
                virtual Handle GetScene(const char* pszSystemName) = 0;

                /**
                 * Gets a system scene by its system type.
                 *
                 * @param   Type    The type of the system scene to get.
                 * @return  A handle to the system scene or null if it doesn't exist.
                 */
                virtual Handle GetScene(System::Type Type) = 0;

                /**
                 * Gets a system object by its system name.
                 *
                 * @param   pszSystemName   The name of the system object to get.
                 * @param   pszName         The name of the object.
                 * @return  A handle to the system object or null if it doesn't exist.
                 */
                virtual Handle GetSystemObject(const char* pszSystemName, const char* pszName) = 0;

                /**
                 * Gets a system object by its system type and name.
                 *
                 * @param   Type    The type of the system object to get.
                 * @param   pszName Name of the object.
                 * @return  A handle to the system object or null if it doesn't exist.
                 */
                virtual Handle GetSystemObject(System::Type Type, const char* pszName) = 0;

                /**
                 * Gets a single system property.
                 *
                 * @param   hSystem     The handle of the system.
                 * @param   Property    The property to store the data in.  The name of the property must be set
                 *                      prior to entering this function.
                 */
                virtual void GetSystemProperty(Handle hSystem, InOut Properties::Property& Property) = 0;

                /**
                 * Sets a single system property.
                 *
                 * @param   hSystem     The handle of the system.
                 * @param   Property    The property to containing the property data.  This must be properly
                 *                      filled in prior to calling this function.
                 */
                virtual void SetSystemProperty(Handle hSystem, In Properties::Property& Property) = 0;

                /**
                 * Gets a single system scene property.
                 *
                 * @param   hScene      The handle of the system scene.
                 * @param   Property    The property to store the data in.  The name of the property must be set
                 *                      prior to entering this function.
                 */
                virtual void GetSceneProperty(Handle hScene, InOut Properties::Property& Property) = 0;

                /**
                 * Sets a single system scene property.
                 *
                 * @param   hScene      The handle of the system scene.
                 * @param   Property    The property to containing the property data.  This must be properly
                 *                      filled in prior to calling this function.
                 */
                virtual void SetSceneProperty(Handle hScene, In Properties::Property& Property) = 0;

                /**
                 * Gets a single object property.
                 *
                 * @param   hObject     The handle of the system object.
                 * @param   Property    The property to store the data in.  The name of the property must be set
                 *                      prior to entering this function.
                 */
                virtual void GetObjectProperty(Handle hObject, InOut Properties::Property& Property) = 0;

                /**
                 * Sets a single object property.
                 *
                 * @param   hObject     The handle of the system object.
                 * @param   Property    The property to containing the property data.  This must be properly
                 *                      filled in prior to calling this function.
                 */
                virtual void SetObjectProperty(Handle hObject, In Properties::Property& Property) = 0;
        };

        /**
         * Gets a reference to the ISystemAccess class.
         *
         * @return  A reference to the ICollision class.
         */
        virtual ISystemAccess& SystemAccess(void) = 0;

        /**
         * Used by a system/framework to register itself as a provider for ISystemAccess.
         *
         * @param   pSystemAccess   A pointer to the provider to register.
         */
        virtual void RegisterSystemAccessProvider(ISystemAccess* pSystemAccess) = 0;

        /**
         * Used by a system/framework to de-register itself as a provider for ISystemAccess.
         *
         * @param   pSystemAccess   A pointer to the provider to de-register.
         */
        virtual void UnregisterSystemAccessProvider(ISystemAccess* pSystemAccess) = 0;

        /**
         * Interface class for providing collision tests to the systems.
         */
        class ICollision {
            public:

                /**
                 * Requests a collision test.
                 *
                 * @param   Request Collision request.
                 * @return  A handle used to get the results.
                 */
                virtual Coll::Handle Test(const Coll::Request& Request) = 0;

                /**
                 * Requests a collision line test.
                 *
                 * @param   Position0   Vector position to start test.
                 * @param   Position1   Vector position to end test.
                 * @param   Request     Collision request.
                 * @return  A handle used to get the results.
                 */
                virtual Coll::Handle LineTest(const Math::Vector3& Position0, const Math::Vector3& Position1, Coll::Request& Request) = 0;

                /**
                 * Gets results for the given handle.
                 *
                 * @param   Handle  Collision test handle.
                 * @param   Result  Pointer to structure to be filled with results.
                 * @return  Returns true if test has finished.
                 */
                virtual bool Finalize(Coll::Handle Handle, Coll::Result* Result) = 0;

        };

        /**
         * Gets a reference to the ICollision class.
         *
         * @return  A reference to the ICollision class.
         */
        virtual ICollision& Collision(void) = 0;

        /**
         * Used by a system/framework to register itself as a provider for ICollision.
         *
         * @param   pCollision  A pointer to the provider to register.
         */
        virtual void RegisterCollisionProvider(ICollision* pCollision) = 0;

        /**
         * Used by a system/framework to de-register itself as a provider for ICollision.
         *
         * @param   pCollision  A pointer to the provider to de-register.
         */
        virtual void UnregisterCollisionProvider(ICollision* pCollision) = 0;

        /**
         * Instrumentation interface, to let various pieces of code touch the instrumentation.
         */
        class IInstrumentation {
            public:

                /**
                 * Updates the periodic data described by deltaTime.
                 * This function drives the instrumentation; it's called regularly from the scheduler in the
                 * framework, to let the instrumentation know that another frame has elapsed.  Whenever it is
                 * called, this function will make sure that the cached stats used by the instrumentation are up
                 * to date; if they've gotten too old, it will refresh those stats.
                 *
                 * @param   deltaTime   f32 - Elapsed wall-clock time since the last call to this function.
                 */
                virtual void UpdatePeriodicData(f32 deltaTime) = 0;

                /**
                 * Instrumentation::getCPUCount>
                 * Get the number of available CPUs in the system, including physical and logical CPUs.
                 *
                 * @return  i32 - Number of available CPUs in the system.
                 */
                virtual i32 getCPUCount() = 0;

                /**
                 * Instrumentation::getNumCounters>
                 * Get the number of CPU performance counters that we're using.  There will be one for each
                 * available CPU, and one more for the total.
                 *
                 * @return  i32 - Number of CPU performance counters, which will be returned by the
                 *          getCPUCounters call.
                 */
                virtual i32 getNumCounters() = 0;

                /**
                 * Instrumentation::getCPUCounters>
                 * Get the most recently measured CPU counters.  This value is automatically refreshed.
                 *
                 * @param   CPUCounters f64* - Array, filled by this call, of CPU counters showing percent CPU
                 *                      load. Must be big enough to hold all counters, see
                 *                      Instrumentation::getNumCounters().
                 */
                virtual void getCPUCounters(f64* CPUCounters) = 0;

                /**
                 * Instrumentation::getCurrentFPS>
                 * Get the most recently-measured frame rate (in Frames Per Second).  This value is
                 * automatically refreshed.  It is averaged over the last few frames (defined by the update
                 * interval in this class).
                 *
                 * @return  f32 - Frame rate, in frames per second.
                 */
                virtual f32  getCurrentFPS() = 0;

                /**
                 * Instrumentation::setActiveThreadCount>
                 * Set the number of threads the application will now run.
                 *
                 * @param   activeThreadCount   i32 - Number of active threads the app should use now.
                 */
                virtual void setActiveThreadCount(i32 activeThreadCount) = 0;

                /**
                 * Instrumentation::getActiveThreadCount>
                 * Get the number of threads that we're currently using in this application.
                 *
                 * @return  i32 - Current active thread count.
                 */
                virtual i32 getActiveThreadCount() = 0;

                /**
                 * Instrumentation::CaptureJobCounterTicks>
                 * Called when some job has finished.  Keep track of how much time this job has spent in this
                 * frame. There may be many jobs of one type passed in during a single frame; their results will
                 * be appended.
                 *
                 * @param   jobType         u32 - The type of the job that has just completed; a member of
                 *                          System::Types.
                 * @param   jobCounterTicks i64 - The number of clock ticks, from _RDTSC, that this job used
                 *                          during this frame.
                 */
                virtual void CaptureJobCounterTicks(u32 jobType, i64 jobCounterTicks) = 0;

                /**
                 * Instrumentation::getJobCount>
                 * Get the max number of job types possible in the system, so the caller can allocate the right
                 * sized array.
                 *
                 * @return  i32 - Max number of job types.
                 */
                virtual i32 getJobCount() = 0;

                /**
                 * Instrumentation::getJobRatios>
                 * Get the ratios of job work done in this most recent frame.
                 *
                 * @param   jobRatios   f32* - Array that this function should fill with the ratios of time spent
                 *                      in each workload on this frame. Must be the right length; call
                 *                      getJobCount.
                 */
                virtual void getJobRatios(f32* jobRatios) = 0;

        };

        /**
         * Get the instrumentation interface.
         *
         * @return  .
         */
        virtual IInstrumentation& Instrumentation(void) = 0;

        /**
         * Register the given instrumentation object as the global provider. Call this once during
         * startup.
         *
         * @param   pInstrumentation    IInstrumentation* - Instrumentation object to register.
         */
        virtual void RegisterInstrumentationProvider(IInstrumentation* pInstrumentation) = 0;

        /**
         * Unregister the given instrumentation object as the global provider. Call this once during
         * shutdown.
         *
         * @param   pInstrumentation    IInstrumentation* - Instrumentation object to unregister.
         */
        virtual void UnregisterInstrumentationProvider(IInstrumentation* pInstrumentation) = 0;

};
