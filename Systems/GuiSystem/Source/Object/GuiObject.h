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

#include "System/ISystemObject.h"
#include "System.h"

class GuiSystem;
class GuiScene;

/**
 * Implementation of the IGraphicsObject interface. See Interfaces\Graphics.h and Interfaces\
 * System.h for a definition of the class and its functions.
 * 
 * @sa  ISystemObject
 */
class GuiObject : public ISystemObject {

    public:

        enum Types {
            Type_Generic
        };
        
        /**
         * @inheritDoc
         */
        GuiObject(ISystemScene* pSystemScene, const char* pszName);
        
        /**
         * @inheritDoc
         */
        virtual ~GuiObject(void);
        
        /**
         * @inheritDoc
         */
        virtual void Update(f32 DeltaTime);

        /**
         * @inheritDoc
         */
        virtual System::Changes::BitMask GetPotentialSystemChanges(void){
            return System::Changes::None;
        }

        /**
         * @inheritDoc
         */
        virtual System::Types::BitMask GetDesiredSystemChanges(void) {
            return System::Changes::Input::Velocity;
        }
        
        /**
         * @inheritDoc
         */
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);
        
        /**
         * @inheritDoc
         */
        System::Type GetSystemType(void) {
            return System::Types::Gui;
        }

        /**
         * Gets the type.
         * Returns the type of graphics object.
         *
         * @return  GuiObject::Types - Type object.
         */
        Types GetType(void) {
            return m_Type;
        }

        /**
         * @inheritDoc
         */
        virtual Error Initialize(std::vector<Properties::Property> Properties);
        
    protected:
        
        static const char*                  sm_kapszTypeNames[];
        static const char*                  sm_kapszPropertyNames[];
        static const Properties::Property   sm_kaDefaultProperties[];
        
        const char*                         m_pszName;
        Types                               m_Type;

};

