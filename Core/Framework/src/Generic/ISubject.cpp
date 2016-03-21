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
#include <boost/assert.hpp>

#include "Errors.h"
#include "Generic/IObserver.h"
#include "System/Types.h"
#include "Universal/UObject.h"

ISubject::ISubject(UObject* entity)
    : entity_(entity) {
}

ISubject::~ISubject() {
    PreDestruct();
}

unsigned int ISubject::getObserverId(IObserver* pObserver) const {
    for (auto observer : m_observerList) {
        if (observer.m_pObserver == pObserver) {
            return observer.m_myID;
        }
    }

    return InvalidObserverID;
}

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

void ISubject::Attach(IObserver* pObserver, System::Types::BitMask inInterest, unsigned int uID) {
    BOOST_ASSERT_MSG(pObserver, "ISubject::Attach: Valid pointer to observer object must be specified");
    BOOST_ASSERT_MSG(std::find(m_observerList.begin(), m_observerList.end(), pObserver) == m_observerList.end(),
           "ISubject::Attach: Observer has already been attached. Use ISubject::UpdateInterestBits instead.");
    m_observerList.push_back(ObserverRequest(pObserver, inInterest, uID));
}

/**
 * @inheritDoc
 */
void ISubject::Detach(IObserver* pObserver) {
    ObserverList::iterator it = std::find(m_observerList.begin(), m_observerList.end(), pObserver);
    if (it != m_observerList.end()) {
        m_observerList.erase(it);
    }
}
/**
 * @inheritDoc
 */
void ISubject::UpdateInterestBits(IObserver* pObserver, unsigned int uInIntrestBits) {
    ObserverList::iterator it = std::find(m_observerList.begin(), m_observerList.end(), pObserver);
    if (it != m_observerList.end()) {
        // No lock is used, but updates can happen concurrently. So use interlocked operation
        long prevBits;
        long newBits = long(it->m_interestBits | uInIntrestBits);
        do {
            prevBits = it->m_interestBits;
        } while (AtomicCompareAndSwap((long*)&it->m_interestBits, newBits, prevBits) != prevBits);
    }
}

/**
 * @inheritDoc
 */
void ISubject::PostChanges(System::Changes::BitMask changedBits) {
    for (auto observerRequest : m_observerList) {
        unsigned int changedBitsOfInterest = observerRequest.m_interestBits & changedBits;
        if (changedBitsOfInterest) {
            observerRequest.m_pObserver->ChangeOccurred(this, changedBitsOfInterest);
        }
    }
}

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

const void* ISubject::getComponent(Schema::ComponentType componentType) {
    return entity_->GetExtension(componentType)->getComponentData();
}

