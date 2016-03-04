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

#include "Generic/ISubject.h"

#include <algorithm>

#include "Assert.h"
#include "Defines.h"
#include "Errors.h"
#include "Generic/IObserver.h"
#include "System/Types.h"

/**
 * @inheritDoc
 */
ISubject::ISubject() {
}

/**
 * @inheritDoc
 */
ISubject::~ISubject() {
    PreDestruct();
}

/**
 * @inheritDoc
 */
u32 ISubject::getObserverId(IObserver* pObserver) const {
    for (auto observer : m_observerList) {
        if (observer.m_pObserver == pObserver) {
            return observer.m_myID;
        }
    }

    return InvalidObserverID;
}

/**
 * @inheritDoc
 */
void ISubject::PreDestruct() {
    // THREAD SAFETY NOTE
    // Currently this method is called from the destructor only (that is it is
    // never called concurrently). Thus it does not lock the list. If ever in
    // the future it is called concurrently, then locking similar to that
    // in the ISubject::Detach method will have to be added.
    ObserverList::iterator it = m_observerList.begin();

    for (; it != m_observerList.end(); ++it) {
        it->m_pObserver->ChangeOccurred(this, 0);
    }

    m_observerList.clear();
}

/**
 * @inheritDoc
 */
Error ISubject::Attach(IObserver* pObserver, System::Types::BitMask inInterest, u32 uID, u32 shift) {
    // To make compiler happy in release builds while keeping the next assertion
    UNUSED_PARAM(shift);
    // If the following assertion fails, it means that Change Control Manager (CCM)
    // was modified to start using "shifts". Please update the code of this class
    // appropriately (original version did not have any meaningful support except
    // shifting inInterest on entry)
    ASSERT(!shift && "ISubject::Attach: Interest bits are shifted. Read the comment to this assertion");
    // Since the intended usage model is to use this method from CCMs only, and
    // their implementation provided by this framework ensures that pObs in nonzero
    // the following assertion should suffice.
    ASSERT(pObserver && "ISubject::Attach: Valid pointer to observer object must be specified");
#if SUPPORT_CONCURRENT_ATTACH_DETACH_TO_SUBJECTS
    SCOPED_SPIN_LOCK(m_observerListMutex);
#endif
    // Framework's CCM implementation ensures that the following assertion always holds
    ASSERT(std::find(m_observerList.begin(), m_observerList.end(), pObserver) == m_observerList.end() &&
           "ISubject::Attach: Observer has already been attached. Use ISubject::UpdateInterestBits instead.");
    // Add the observer to our list of observers
    m_observerList.push_back(ObserverRequest(pObserver, inInterest, uID));
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error ISubject::Detach(IObserver* pObserver) {
    // No need to check for pObs being nonzero since the find below guarantees correct work in any case
    Error curError = Errors::Failure;
#if SUPPORT_CONCURRENT_ATTACH_DETACH_TO_SUBJECTS
    SCOPED_SPIN_LOCK(m_observerListMutex);
#endif
    // Remove the give observer from our list of observers
    ObserverList::iterator it = std::find(m_observerList.begin(), m_observerList.end(), pObserver);

    if (it != m_observerList.end()) {
        m_observerList.erase(it);
        curError = Errors::Success;
    }

    return curError;
} // ISubject::Detach

/**
 * @inheritDoc
 */
Error ISubject::UpdateInterestBits(IObserver* pObserver, u32 uInIntrestBits) {
    // No need to check for pObs being nonzero since the find below guarantees correct work in any case
    Error curError = Errors::Failure;
#if SUPPORT_CONCURRENT_ATTACH_DETACH_TO_SUBJECTS
    SCOPED_SPIN_LOCK(m_observerListMutex);
#endif
    // Find the given observer in our observer list
    ObserverList::iterator it = std::find(m_observerList.begin(), m_observerList.end(), pObserver);
    if (it != m_observerList.end()) {
#if SUPPORT_CONCURRENT_ATTACH_DETACH_TO_SUBJECTS
        // We are under the lock in this case
        it->m_interestBits |= inInterest;
#else
        // No lock is used, but updates can happen concurrently. So use interlocked operation
        long prevBits;
        long newBits = long(it->m_interestBits | uInIntrestBits);

        do {
            prevBits = it->m_interestBits;
        } while (AtomicCompareAndSwap((long*)&it->m_interestBits, newBits, prevBits) != prevBits);

#endif
        curError = Errors::Success;
    }

    return curError;
}

// The following implementation could be used in case of concurrent initial attach
// or detach operations. But it is commented out since it is unsafe without ref
// counting on IObserver interface.
// See the comment to SUPPORT_CONCURRENT_ATTACH_DETACH_TO_SUBJECTS as well.
#if 0

/**
 * @inheritDoc
 */
void ISubject::PostChanges(System::Changes::BitMask changedBits) {
    if (m_observerList.empty()) {
        return;
    }

    typedef std::pair<IObserver*, u32> PostData;
    PostData* aPostData = NULL;
    u32 nNotificationsToPost = 0;
    // Double check to avoid unnecessary lock acquisition
    {
        SCOPED_SPIN_LOCK(m_observerListMutex);
        aPostData = (PostData*)alloca(m_observerList.size() * sizeof(PostData));
        for (auto observerRequest : m_observerList) {
            u32 changedBitsOfInterest = GetBitsToPost(observerRequest, changedBits);

            if (changedBitsOfInterest) {
                aPostData[nNotificationsToPost] = std::make_pair(observerRequest.m_pObserver, changedBitsOfInterest);
                ++nNotificationsToPost;
            }
        }
    }

    // Posting is done outside of the lock
    for (u32 i = 0; i < nNotificationsToPost; ++i) {
        aPostData[i].first->ChangeOccurred(this, aPostData[i].second);
    }
} // ISubject::PostChanges

#else

/**
 * @inheritDoc
 */
void ISubject::PostChanges(System::Changes::BitMask changedBits) {
#if SUPPORT_CONCURRENT_ATTACH_DETACH_TO_SUBJECTS
    SCOPED_SPIN_LOCK(m_observerListMutex);
#endif
    for (auto observerRequest : m_observerList) {
        u32 changedBitsOfInterest = GetBitsToPost(observerRequest, changedBits);
        if (changedBitsOfInterest) {
            observerRequest.m_pObserver->ChangeOccurred(this, changedBitsOfInterest);
        }
    }
}

#endif /* 0 */

/**
 * @inheritDoc
 */
long ISubject::AtomicCompareAndSwap(long* interestBits, long newBits, long prevBits) {
#if defined(_MSC_VER)
    return _InterlockedCompareExchange(interestBits, newBits, prevBits);
#elif defined(__GNUC__)
    return __sync_val_compare_and_swap(interestBits, prevBits, newBits);
#endif
}
