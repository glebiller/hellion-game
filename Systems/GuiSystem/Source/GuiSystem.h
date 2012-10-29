// Copyright © 2008-2009 Intel Corporation
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

#include "CEGUIRenderer.h"

#include "Errors.h"
#include "Proto.h"
#include "System.h"
#include "System/ISystem.h"

class GuiScene;
class GuiTask;

/**
 * Implementation of the ISystem interface for graphics. See Interfaces\System.h for a
 * definition of the class and its functions.
 * 
 * @sa  ISystem
 */
class GuiSystem : public ISystem {

    public:

        /**
         * @inheritDoc
         */
        GuiSystem(void);
        
        /**
         * @inheritDoc
         */
        ~GuiSystem(void);
        
        /**
         * @inheritDoc
         */
        Error initialize(void);

        /**
         * @inheritDoc
         */
        System::Type GetSystemType(void) {
            return System::Types::Gui;
        }

    protected:

        /**
         * Sets the imageset resource group.
         *
         * @param   values  The values.
         */
        void setImagesetResourceGroup(ProtoStringList values);

    private:

        CEGUI::Renderer*                    m_pRenderer;

};

