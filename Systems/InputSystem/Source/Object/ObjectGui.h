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

#include "Error.h"
#include "System.h"
#include "System/ISystemScene.h"
#include "Object/Object.h"

class InputScene;
class InputTask;


/**
 * <c>InputMouseObject</c> Implementation of the ISystemObject interface.
 * This is the Mouse object created objects.
 * 
 * @sa  InputObject
 * @sa  IMoveObject
 */
class InputGuiObject : public InputObject {

    public:
        
        /**
         * @inheritDoc
         */
        InputGuiObject(ISystemScene* pSystemScene, const char* pszName);
        
        /**
         * @inheritDoc
         */
        ~InputGuiObject(void);

        /**
         * @inheritDoc
         */
        Error initialize(void);
        
        /**
         * @inheritDoc
         */
        void Update(f32 DeltaTime);
        
        /**
         * @inheritDoc
         */
        System::Changes::BitMask GetPotentialSystemChanges(void) {
            return System::Changes::None;
        };

        /**
         * @inheritDoc
         */
        System::Types::BitMask GetDesiredSystemChanges(void) {
            return System::Changes::None;
        };

        /**
         * @inheritDoc
         */
        Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

    private:


};

