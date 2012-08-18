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
#include "System/ISystemSubject.h"

class ISystemScene;

/**
 * <c>ISystemObject</c> is an interface class designed to be an extension of the UObject.
 * Systems can extend the UObject by implementing this interface to give it new  properties.
 * An example would be a physics system implementing this interface so that the UObject would
 * now be able to interact with the physics system.
 * 
 * @sa  CSubject
 * @sa  IObserver
 */
class ISystemObject : public ISystemSubject {

        friend class ISystem;
        friend class ISystemScene;

    public:

        /**
         * Constructor.
         * Inlined for performance.
         *
         * @param   pSystemScene    The scene this object belongs to.
         * @param   pszName         Name of this GUI object.
         */
        ISystemObject(ISystemScene* pSystemScene, const char* pszName) : ISystemSubject()
            , m_pSystemScene(pSystemScene) {
            if (pszName) {
                SetName(pszName);
            }
        }
        
        /**
         * @inheritDoc
         */
        virtual ~ISystemObject(void) {

        }

        /**
         * Gets the system scene this object belongs to.
         *
         * @return  A pointer to the system.
         */
        inline ISystemScene* GetSystemScene(void) {
            return m_pSystemScene;
        }

        /**
         * Returns a handle to the parent object that this system object belongs to.
         *
         * @return  A handle to the parent object.
         */
        inline Handle GetParentObject(void) {
            return m_hParentObject;
        }

        /**
         * Set a handle to the parent object that this system object belongs to.
         * This should only be called by the parent object.
         *
         * @param   hParentObject   Handle of the parent object.
         *
         * @return   A handle to the parent object.
         */
        inline void SetParentObject(Handle hParentObject) {
            m_hParentObject = hParentObject;
        }

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

    protected:
        
        std::string                 m_sName;
        ISystemScene*               m_pSystemScene;
        Handle                      m_hParentObject;

};