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

#include <Physics/Dynamics/World/hkpWorld.h>

#include <SpinMutex.h>

// Forward declarations
class HavokPhysicsScene;

struct CollData {
    Coll::Request m_Request;    // Collision request
    Coll::Result  m_Result;     // Result of collision
};


///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   <c>HavokCollisionService</c> Implementation of the ICollision interface
///   for HavokPhysics.  This service provides collision test to other systems.
/// </summary>
///////////////////////////////////////////////////////////////////////////////

class HavokCollisionService : public IService::ICollision {
    public:

        HavokCollisionService(void);
        ~HavokCollisionService(void);

        /// <summary cref="HavokCollisionService::SetWorld">
        ///   Set the Havok World used to test collisions.
        /// </summary>
        /// <param name="pWorld">Pointer Havok World.</param>
        void SetWorld(hkpWorld* pWorld);

        /// <summary cref="HavokCollisionService::ProcessRequests">
        ///   Processes all outstanding collision test requests.
        /// </summary>
        /// <param name="pScene">Pointer to HavokPhysics system.</param>
        void ProcessRequests(HavokPhysicsScene* pScene);

        /// <summary cref="HavokCollisionService::Test">
        ///   Implementation of the <c>ICollision::Test</c> function.
        ///   Registers a test request and returns a unique handle to make future requests.
        /// </summary>
        /// <returns>Coll::Handle - A unique handle for this registered test.</returns>
        /// <seealso cref="ICollision::Test"/>
        virtual Coll::Handle Test(const Coll::Request& Request);

        /// <summary cref="HavokCollisionService::LineTest">
        ///   Registers a line test and returns a unique handle to make future requests.
        /// </summary>
        /// <returns>Coll::Handle - A unique handle for this registered test.</returns>
        /// <seealso cref="HavokCollisionService::Test"/>
        virtual Coll::Handle LineTest(const Math::Vector3& Start, const Math::Vector3& End, Coll::Request& Request);

        /// <summary cref="HavokCollisionService::Finalize">
        ///   Implementation of the <c>ICollision::Finalize</c> function.
        ///   Request the results of a collision test.  If the test is not complete,
        ///   this will return false.
        /// </summary>
        /// <returns>Coll::Handle - A unique handle for this registered test.</returns>
        /// <seealso cref="ICollision::Finalize"/>
        virtual Bool Finalize(Coll::Handle Handle, Coll::Result* pResult);


    protected:

        /// <summary cref="HavokCollisionService::GetNextHandle">
        ///   Get the next unique collision handle.
        /// </summary>
        /// <returns>Coll::Handle - A unique handle.</returns>
        Coll::Handle GetNextHandle(void);

        hkpWorld*                            m_pWorld;               // Havok world

        u32                                  m_HandleCount;          // Handle counter (next unique handle)
        std::map<Coll::Handle, CollData>      m_CollData;            // Collision system workspace
        std::vector<Coll::Request>           m_PendingRequests;      // Store pending collision tests
        std::map<Coll::Handle, Coll::Result>  m_PendingResults;      // Store pending results
        std::vector<Coll::Handle>            m_DeadHandles;          // Store a list of used (dead) results

        DEFINE_SPIN_MUTEX(m_PendingRequestsLock);  // Lock for m_PendingRequests
        DEFINE_SPIN_MUTEX(m_PendingResultsLock);   // Lock for m_PendingResults
        DEFINE_SPIN_MUTEX(m_DeadHandlesLock);      // Lock for m_DeadHandles
};

