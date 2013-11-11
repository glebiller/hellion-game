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


#include "Errors.h"
#include "System/Types.h"
#include "Generic/IObserver.h"

class ISubject;

/**
 * The <c>IObserver</c> interface supplies loosely coupling systems with dependency
 *     It facilitates a lazy behaviour where by systems only need to react to a change and
 *     facilitates dependent callback threading.
 * This interface follows the pattern commonly know as the Observer pattern, the
 *      Publish/Subscribe pattern, or the Dependents pattern.
 *      
 * @sa  IObserver
 * @sa  IObserver  
 */
class IChangeManager : public IObserver {
    public:

        /**
         * Associates the provided IObserver with the given ISubject aspects of interest.
         *
         * @param   pInSubject      A pointer to the ISubject.
         * @param   uInIntrestBits  The aspects of interest that changed as defined by the supplied
         *                          ISubject's published interest bits..
         * @param   pInObserver     A pointer to the IObserver.
         * @param   observerIdBits  Bitmask of the interest bits. (default = System::Types::All)
         * @return  One of the following Error codes: Error::Success No error. Error::InvalidAddress
         *          pInObserver and/or pInSubject was NULL. Error::OutOfMemory Not enough memory is
         *          available to resolve the change.
         */
        virtual Error Register(ISubject* pInSubject, u32 uInIntrestBits, IObserver* pInObserver, System::Types::BitMask observerIdBits = System::Types::All) = 0;

        /**
         * Disassociates the provided IObserver from the supplied ISubject.
         *
         * @param   pInSubject  A pointer to the ISubject.
         * @param   pInObserver A pointer to the IObserver.
         * @return  One of the following Error codes: Error::Success No error. Error::InvalidAddress
         *          pInObserver and/or pInSubject was NULL.
         */
        virtual Error Unregister(ISubject* pInSubject, IObserver* pInObserver) = 0;

        /**
         * Distributes the queued changes.
         * Intended to be called after all ISubject's have changed state to deliver relevant
         * queued change notifications to registered IObservers via. IObserver::Update().
         *
         * @param   systems2BeNotified  (optional) the systems 2 be notified.
         * @param   ChangesToDist       (optional) the changes to distance.
         * @return  One of the following Error codes: Error::Success No error. Error::InvalidAddress
         *          pInObserver and/or pInSubject was NULL. Error::OutOfMemory Not enough memory is
         *          available to resolve the change.
         */
        virtual Error DistributeQueuedChanges(System::Types::BitMask systems2BeNotified = System::Types::All,
                                              System::Changes::BitMask ChangesToDist = System::Changes::All) = 0;

};