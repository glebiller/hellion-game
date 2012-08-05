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
#include "SystemTypes.h"

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

        /**
         * Destructor.
         * All interfaces must have virtual destructors
         */
        virtual ~ISubject() {}

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
        virtual Error Attach(IObserver* pInObserver, u32 uInIntrestBits, u32 uID, u32 shiftBits = 0) = 0;

        /**
         * Disassociates the provided Observer with the Subject.This method is typically called from @e ChangeManager::Register()
         *  or the IObserver, if used without a ChangeManager.
         *
         * @param   pInObserver A pointer to the IObserver that should be disassociated.
         * @return  One of the following Error codes: Error::Success No error. Error::InvalidAddress
         *          pInObserver and/or pInSubject was NULL.
         */
        virtual Error Detach(IObserver* pInObserver) = 0;

        /**
         * Updates the interest bits.
         *
         * @param [in,out]  pInObserver If non-null, the in observer.
         * @param   uInIntrestBits      The in intrest bits.
         * @return  The Error code. Error::Success No error.
         */
        virtual Error UpdateInterestBits(IObserver* pInObserver, u32 uInIntrestBits) = 0;

        /**
         * Returns the ID assigned by pObserver to this subject.
         *
         * @param [in,out]  pObserver   If non-null, the observer.
         * @return  The identifier.
         */
        virtual u32 GetID(IObserver* pObserver) const = 0;

        /**
         * Publishes to attached Observers and ChanageManager that changes have occurred.
         * This method is typically called from @e ChangeManager::Register()
         * or the IObserver, if used without a ChangeManager.
         * 
         * @param   uInChangedBits  The u32 bit field that describes the conceptual change with respect
         *                          to the published interests.
         * @return   One of the following Error codes: Error::Success No error. Error::InvalidAddress
         *              pInObserver and/or pInSubject was NULL.
         */
        virtual void PostChanges(System::Changes::BitMask uInChangedBits) = 0;

        /**
         * Identifies the system changes that this subject could possibly make.
         *
         * @return  A bitmask of the possible system changes.
         */
        virtual System::Changes::BitMask GetPotentialSystemChanges(void) = 0;

        /**
         * Forces the Destruction.
         * 
         * @return One of the following Error codes: Error::Success No error. Error::InvalidAddress
         *          pInObserver and/or pInSubject was NULL.
         */
        virtual void PreDestruct(void) = 0;

};