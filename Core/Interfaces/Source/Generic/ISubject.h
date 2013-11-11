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

#include <stddef.h>
#include <list>

#include "DataTypes.h"
#include "System/Changes.h"

class IObserver;

/**
 * The <c>ISubject</c> interface supplies loosely coupling systems with dependency
 *     It facilitates a lazy behaviour where by systems only need to react to a change and
 *     facilitates dependent callback threading.
 *     This interface follows the pattern commonly know as the Observer pattern, the
 *     Publish/Subscribe pattern, or the Dependents pattern.
 */
class ISubject {
public:

    static const u32 InvalidObserverID = u32(-1);

    /**
     * Default constructor.
     */
    ISubject(void);

    /**
     * Destructor.
     * All interfaces must have virtual destructors
     */
    virtual ~ISubject();

    /**
     * Returns the ID assigned by pObserver to this subject.
     *
     * @param [in,out]  pObserver   If non-null, the observer.
     * @return  The identifier.
     */
    u32 getObserverId(IObserver* pObserver) const;

    /**
     * Associates the provided IObserver with the given ISubject aspects of interest.
     * This method is typically called from @e ChangeManager::Register()
     *  or the IObserver, if used without a ChangeManager.
     *
     * @param   pInObserver     A pointer to the IObserver.
     * @param   uInIntrestBits  The aspects of interest that changed as defined by the supplied
     *                          ISubject's published interest bits.
     * @param   uID             ID assigned by pInObserver to this subject.
     * @param   shiftBits       Used for components supporting multiply inherited interfaces each
     *                          with subject interfaces.
     * @return  One of the following Error codes: Error::Success No error. Error::InvalidAddress
     *          pInObserver and/or pInSubject was NULL. Error::OutOfMemory Not enough memory is
     *          available to resolve the change.
     */
    virtual Error Attach(IObserver* pInObserver, u32 uInIntrestBits, u32 uID, u32 shiftBits = 0);

    /**
     * Disassociates the provided Observer with the Subject.This method is typically called from @e ChangeManager::Register()
     *  or the IObserver, if used without a ChangeManager.
     *
     * @param   pInObserver A pointer to the IObserver that should be disassociated.
     * @return  One of the following Error codes: Error::Success No error. Error::InvalidAddress
     *          pInObserver and/or pInSubject was NULL.
     */
    virtual Error Detach(IObserver* pInObserver);

    /**
     * Updates the interest bits.
     *
     * @param [in,out]  pInObserver If non-null, the in observer.
     * @param   uInIntrestBits      The in intrest bits.
     * @return  The Error code. Error::Success No error.
     */
    virtual Error UpdateInterestBits(IObserver* pInObserver, u32 uInIntrestBits);

    /**
     * Identifies the system changes that this subject could possibly make.
     *
     * @return  A bitmask of the possible system changes.
     */
    virtual System::Changes::BitMask GetPotentialSystemChanges(void) = 0;

    /**
     * Publishes to attached Observers and ChanageManager that changes have occurred.
     * This method is typically called from @e ChangeManager::Register()
     * or the IObserver, if used without a ChangeManager.
     *
     * @param   uInChangedBits  The u32 bit field that describes the conceptual change with respect
     *                          to the published interests.
     */
    void PostChanges(System::Changes::BitMask uInChangedBits);

protected:

    /**
     * Forces the Destruction.
     *
     * @return One of the following Error codes: Error::Success No error. Error::InvalidAddress
     *          pInObserver and/or pInSubject was NULL.
     */
    virtual void PreDestruct(void);

    /**
     * Defines a structure used by the subject to store information about observers for PostChanges
     * notifications (CCMs).
     */
    struct ObserverRequest {
        ObserverRequest(IObserver* pObserver = NULL, u32 Interests = 0, u32 myID = 0)
            : m_pObserver(pObserver)
            , m_interestBits(Interests)
            , m_myID(myID) {
        }

        IObserver*  m_pObserver;
        u32         m_interestBits;
        u32         m_myID;

        bool operator == (IObserver* rhs) const {
            return m_pObserver == rhs;
        }
    }; // struct ISubject::ObserverRequest

    /**
     * Gets the bits to post.
     *
     * @param [in,out]  parameter1  The first parameter.
     * @param   parameter2          The second parameter.
     * @return  The bits to post.
     */
    inline u32 GetBitsToPost(ISubject::ObserverRequest& req, System::Changes::BitMask changedBits) {
        return req.m_interestBits & changedBits;
    }

private:

    /**
     * Performs an Atomic Compare and Swap.
     *
     * @param [in,out]  interestBits  The value to compare and write to.
     * @param   newBits          The new value to set if needed.
     * @param   prevBits         The old value to compare to.
     * @return  The new value if it was written to or original value.
     */
    long AtomicCompareAndSwap(long* interestBits, long newBits, long prevBits);

    // IMPLEMENTATION NOTE
    // Since only Change Control Managers (CCM) are supposed to subscribe for
    // notifications sent by PostChange, there are not many observers expected
    // to be in the list. Another assumption is that repeated attaches are infrequent.
    // Thus the most frequent operation is the traversal and thus the usual std::list
    // will suit fine here.
    typedef std::list<ObserverRequest> ObserverList;

    // List of the observers (CCMs) that need notifications about changes in this subject
    ObserverList    m_observerList;

#if SUPPORT_CONCURRENT_ATTACH_DETACH_TO_SUBJECTS
    // Synchronization object to protect m_observerList
    DEFINE_SPIN_MUTEX(m_observerListMutex);
#endif /* SUPPORT_MULTIPLE_OBSERVERS_IN_POST_CHANGES */

};