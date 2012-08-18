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
#include "Property.h"
#include "Observer/CSubject.h"
#include "Observer/IObserver.h"

/**
 * The Subject implementation.
 * 
 * @sa  ISubject
 */
class ISystemSubject : public CSubject, public IObserver {

    public :

        /**
         * Default constructor.
         */
        ISystemSubject(void);

        /**
         * Destructor.
         * Virtual destructor, CSubject is an abstract class.
         */
        virtual ~ISystemSubject(void);
        
        /**
         * Gets the system type for this system object.
         * This is a shortcut to getting the system type w/o having to go the system first.
         *
         * @return  The type of the system. 
         */
        virtual System::Type GetSystemType(void) {
            return System::Types::Null;
        }

        /**
         * One time initialization function for the object.
         *
         * @param   Properties  Property structure array to fill in.
         * @return  An error code.
         */
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /**
         * Gets the properties of this object.
         *
         * @param   Properties  The Property structure array to fill.
         */
        virtual void GetProperties(std::vector<Properties::Property>& Properties);

        /**
         * Sets the properties for this object.
         *
         * @param   Properties  Property structure array to get values from.
         */
        virtual void SetProperties(std::vector<Properties::Property> Properties);
        
        /**
         * Returns a bit mask of System Changes that this scene wants to receive changes for.  Used
         *  to inform the change control manager if this scene should be informed of the change.
         *
         * @return  A System::Changes::BitMask.
         */
        virtual System::Changes::BitMask GetDesiredSystemChanges(void) {
            return System::Changes::None;
        }

        /**
         * Update the system object.
         *
         * @param   DeltaTime   Time of the delta.
         */
        virtual void Update(f32 DeltaTime) {
            
        }

        /**
         * @inheritDoc
         */
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
            return Errors::Success;    
        };

    protected:
        bool            m_bInitialized;
};
