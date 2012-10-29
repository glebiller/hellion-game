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

//
// extern includes
//
#include "tbb/concurrent_hash_map.h"
#include "tbb/atomic.h"

class HavokPhysicsTask;
class HavokCollisionService;

#define MAIN_THREAD_IS_PART_OF_POOL 1

typedef tbb::concurrent_hash_map<u32, hkMemoryRouter*> WorkerMemoryRouterMap_t;

/**
 * Implementation of the ISystem interface for graphics. See Interfaces\System.h for a
 * definition of the class and its functions.
 * 
 * @sa  ISystem
 */
class HavokPhysicsSystem : public ISystem {

    public:
        
        /**
         * @inheritDoc
         */
        HavokPhysicsSystem(void);
        
        /**
         * @inheritDoc
         */
        ~HavokPhysicsSystem(void);
        
        /**
         * @inheritDoc
         */
        Error initialize(void);

        /**
         * @inheritDoc
         */
        System::Type GetSystemType(void) {
            return System::Types::Physic;
        }

        HavokCollisionService* GetService(void) {
            return &m_Collision;
        }

        hkJobQueue* getJobQueue(void) {
            return m_jobQueue;
        }

    public:

        /**
         * Allocate resourses need to support multiple threads.
         *
         * @param pSystem The system allocating the resources.
         */
        static void AllocateThreadResources(HavokPhysicsSystem* pSystem);

        /**
         * Free resourses allocated to support multiple threads.
         *
         * @param pSystem The system allocating the resources.
         */
        static void FreeThreadResources(HavokPhysicsSystem* pSystem);

        /**
         * Callback for Havok to report an error.
         * (See Havok documentation for more details)
         *
         * @param pString The error string.
         * @param pErrorOutputObject Pointer to the error object.
         */
        static void ErrorReport(const char* pString, void* pErrorOutputObject);

    private:

        HavokCollisionService               m_Collision;
        hkJobQueue*                         m_jobQueue;

        static u32                          s_idMainThread;
        static tbb::atomic<u32>             s_threadNumberCount;
        static WorkerMemoryRouterMap_t      s_workerMemoryRouterMap;

};

