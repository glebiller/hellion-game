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

#include <string>
#include <vector>

#include "Errors.h"
#include "Property.h"
#include "System.h"
#include "Observer/CSubject.h"
#include "Observer/IObserver.h"

class ISystemScene;

/**
 * <c>ISystemObject</c> is an interface class designed to be an extension of the UObject.
 * Systems can extend the UObject by implementing this interface to give it new  properties.
 * An example would be a physics system implementing this interface so that the UObject would
 * now be able to interact with the physics system.
 * 
 * @sa  CSubject
 * @sa  CSubject
 * @sa  IObserver
 */
class ISystemObject : public CSubject, public IObserver {
        friend class ISystem;
        friend class ISystemScene;

    public:

        /**
         * Constructor.
         *
         * @param   pSystemScene    The scene this object belongs to.
         * @param   pszName         Name of this GUI object.
         *
         * ### remarks  Inlined for performance.
         */
        ISystemObject(ISystemScene* pSystemScene, const char* pszName)
            : m_bInitialized(false),
            m_pSystemScene(pSystemScene) {
            if (pszName) {
                SetName(pszName);
            }
        }

        /**
         * Gets the system scene this object belongs to.
         *
         * @return  A pointer to the system.
         */
        ISystemScene* GetSystemScene(void) {
            return m_pSystemScene;
        }

        /**
         * Returns a handle to the parent object that this system object belongs to.
         *
         * @return  A handle to the parent object.
         */
        Handle GetParentObject(void) {
            return m_hParentObject;
        }

        /**
         * Set a handle to the parent object that this system object belongs to.
         *
         * @param   hParentObject   Handle of the parent object.
         *
         * ### remarks  This should only be called by the parent object.
         * ### return   A handle to the parent object.
         */
        void SetParentObject(Handle hParentObject) {
            m_hParentObject = hParentObject;
        }

        /**
         * Gets the system type for this system object.
         *
         * @return  The type of the system.
         *
         * ### remarks  This is a shortcut to getting the system type w/o having to go the system first.
         */
        virtual SystemProto::Type GetSystemType(void) = 0;

        /**
         * Gets the name of the object.
         *
         * @return  The name of the object.
         */
        virtual inline const char* GetName(void) {
            return m_sName.c_str();
        }

        /**
         * Sets the name of the object.
         *
         * @param   pszName The new name of the object.
         */
        virtual inline void SetName(const char* pszName) {
            m_sName = pszName;
        }

        /**
         * One time initialization function for the object.
         *
         * @param   Properties  Property structure array to fill in.
         * @return  An error code.
         */
        virtual Error Initialize(std::vector<Properties::Property> Properties) = 0;

        /**
         * Gets the properties of this object.
         *
         * @param   Properties  The Property structure array to fill.
         */
        virtual void GetProperties(std::vector<Properties::Property>& Properties) = 0;

        /**
         * Sets the properties for this object.
         *
         * @param   Properties  Property structure array to get values from.
         */
        virtual void SetProperties(std::vector<Properties::Property> Properties) = 0;

        /**
         * Returns a bit mask of System Changes that this system wants to receive changes for.  Used
         *  to inform the change control manager if this system's object should be informed of the
         *  change.
         *
         * @return  A System::Changes::BitMask.
         */
        virtual System::Changes::BitMask GetDesiredSystemChanges(void) = 0;

        /**
         * Update the system object.
         *
         * @param   DeltaTime   Time of the delta.
         */
        virtual void Update(f32 DeltaTime) = 0;

    protected:

        /**
         * The initialized.
         */
        bool                        m_bInitialized;
        ISystemScene*               m_pSystemScene;

        /**
         * The parent object.
         */
        Handle                      m_hParentObject;
        std::string                 m_sName;

};