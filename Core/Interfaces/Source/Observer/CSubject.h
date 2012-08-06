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

class IObserver;
class IChangeManager;


#include <list>

#include "Errors.h"
#include "System.h"
#include "Observer/ISubject.h"


// THREAD SAFETY NOTE
//
// Affects CSubject class implementation
//
// Currently there are no concurrent initial attach or detach operations on subject
// objects. Thus corresponding locking operations (protecting the integrity of
// the observers list) are disabled.
//
// It is not recommended to introduce concurrency of this sort, because it not only
// would result in additional performance impact to _all_ operations with subject
// (including frequent PostChanges), but also would introduce the risk of deadlocks
// (implementation of the Change Control Manager will have to be carefully revised).
// Most probably eliminating the deadlocks threat will further increase the overhead.
// One of the ways to avoid deadlock is posting change notifications (ChangeOccured)
// after the lock is released, but this would require ref counting on the IObserver
// interface.
//
// Yet concurrent _repeated_ attaches (updating interest bits for already registered
// CCM observer) are possible, so the protection against race conditions introduced
// by them must always be in place.
#define SUPPORT_CONCURRENT_ATTACH_DETACH_TO_SUBJECTS 0

/**
 * The Subject implementation.
 * 
 * @sa  ISubject
 */
class CSubject : public ISubject {
    public :

        static const u32 InvalidID = u32(-1);

        /**
         * Default constructor.
         */
        CSubject(void);

        /**
         * Destructor.
         */
        ~CSubject(void);

        /**
         * @inheritDoc
         */
        virtual Error Attach(IObserver* pObserver, u32 Interest, u32 nID, u32 Shift = 0);
        
        /**
         * @inheritDoc
         */
        virtual Error Detach(IObserver* pObserver);
        
        /**
         * @inheritDoc
         */
        virtual Error UpdateInterestBits(IObserver* pInObserver, u32 uInIntrestBits);
        
        /**
         * @inheritDoc
         */
        virtual u32 GetID(IObserver* pObserver) const;
        
        /**
         * @inheritDoc
         */
        virtual void PostChanges(System::Changes::BitMask changedBits);
        
        /**
         * @inheritDoc
         */
        void PreDestruct(void);

    protected:

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
        }; // struct CSubject::ObserverRequest

        /**
         * Gets the bits to post.
         *
         * @param [in,out]  parameter1  The first parameter.
         * @param   parameter2          The second parameter.
         * @return  The bits to post.
         */
        inline u32 GetBitsToPost(CSubject::ObserverRequest& req, System::Changes::BitMask changedBits) {
            return req.m_interestBits & changedBits;
        }

    private:

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
