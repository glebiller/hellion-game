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

#include "System/ISystemObject.h"

#include "Universal/UObject.h"

class UObject;

/**
 * @inheritDoc
 */
ISystemObject::ISystemObject(ISystemScene* pSystemScene, UObject* entity, const Schema::SystemComponent& component)
        : component_(component), m_pSystemScene(pSystemScene), m_entity(entity) {
}

/**
 * @inheritDoc
 */
ISystemObject::~ISystemObject() {
    for (auto observer : observers_) {
        observer.m_pObserver->ChangeOccurred(this, 0);
    }

    observers_.clear();
}

unsigned int ISystemObject::getObserverId(IObserver* pObserver) const {
    for (auto observer : observers_) {
        if (observer.m_pObserver == pObserver) {
            return observer.m_myID;
        }
    }

    return InvalidObserverID;
}

void ISystemObject::Attach(IObserver* pInObserver, System::Types::BitMask uInIntrestBits, unsigned int uID) {
    BOOST_ASSERT_MSG(pInObserver, "ISystemObject::Attach: Valid pointer to observer object must be specified");
    BOOST_ASSERT_MSG(std::find(observers_.begin(), observers_.end(), pInObserver) == observers_.end(),
                     "ISystemObject::Attach: Observer has already been attached. Use ISystemObject::UpdateInterestBits instead.");
    observers_.push_back(ObserverRequest(pInObserver, uID, uInIntrestBits));
}

void ISystemObject::Detach(IObserver* pInObserver) {
    auto it = std::find(observers_.begin(), observers_.end(), pInObserver);
    if (it != observers_.end()) {
        observers_.erase(it);
    }
}

void ISystemObject::UpdateInterestBits(IObserver* pInObserver, unsigned int uInIntrestBits) {
    auto it = std::find(observers_.begin(), observers_.end(), pInObserver);
    if (it != observers_.end()) {
        // No lock is used, but updates can happen concurrently. So use interlocked operation
        long prevBits;
        long newBits = long(it->m_interestBits | uInIntrestBits);
        do {
            prevBits = it->m_interestBits;
        } while (AtomicCompareAndSwap((long*) &it->m_interestBits, newBits, prevBits) != prevBits);
    }
}

void ISystemObject::PostChanges(System::Changes::BitMask uInChangedBits) {
    for (auto observerRequest : observers_) {
        unsigned int changedBitsOfInterest = observerRequest.m_interestBits & uInChangedBits;
        if (changedBitsOfInterest) {
            observerRequest.m_pObserver->ChangeOccurred(this, changedBitsOfInterest);
        }
    }
}

const void* ISystemObject::getComponent() {
    return component_.data();
}

long ISystemObject::AtomicCompareAndSwap(long* interestBits, long newBits, long prevBits) {
#if defined(_MSC_VER)
    return _InterlockedCompareExchange(interestBits, newBits, prevBits);
#elif defined(__GNUC__)
    return __sync_val_compare_and_swap(interestBits, prevBits, newBits);
#endif
}






