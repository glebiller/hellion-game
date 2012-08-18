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

#include "Object/GuiObject.h"
#include "Property.h"
#include "System.h"

class ISystemScene;
class ISubject;

/**
 * Implementation of the IGraphicsObject interface. See Interfaces\Graphics.h and Interfaces\
 * System.h for a definition of the class and its functions.
 * 
 * @sa  GuiObject
 */
class GuiMouseObject : public GuiObject {

    public:
        
        /**
         * @inheritDoc
         */
        GuiMouseObject(ISystemScene* pSystemScene, const char* pszName);
        
        /**
         * @inheritDoc
         */
        ~GuiMouseObject(void);
        
        /**
         * @inheritDoc
         */
        virtual void Update(f32 DeltaTime);
        
        /**
         * @inheritDoc
         */
        virtual System::Changes::BitMask GetPotentialSystemChanges(void) {
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
        virtual Error Initialize(std::vector<Properties::Property> Properties);
        
        /**
         * @inheritDoc
         */
        virtual void GetProperties(Properties::Array& Properties);
        
        /**
         * @inheritDoc
         */
        virtual void SetProperties(Properties::Array Properties);

    private:



};

