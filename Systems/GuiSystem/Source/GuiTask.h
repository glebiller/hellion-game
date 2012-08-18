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

#include "System/ISystemTask.h"
#include "System.h"

class GuiScene;

///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   Implementation of the ISystemTask interface for OGRE graphics.
///   See Interfaces\System.h for a definition of the class and its functions.
/// </summary>
///////////////////////////////////////////////////////////////////////////////

class GuiTask : public ISystemTask {
        
        friend GuiScene;

    protected:

        /**
         * Constructor.
         *
         * @param [in,out]  pScene  If non-null, the scene.
         */
        GuiTask(GuiScene* pScene);

        /**
         * Destructor.
         */
        ~GuiTask(void);

        /**
         * Gets the system type.
         * Implementation of the <c>ISystemTask::GetSystemType</c> function. Gets the system type for
         * this system task.
         *
         * @return  System::Type - The type of the system.
         *
         * @sa   ISystemTask::GetSystemType  .
         */
        virtual System::Type GetSystemType(void) {
            return System::Types::Graphic;
        }

        /**
         * Updates the given DeltaTime.
         * Implementation of the <c>ISystemTask::Update</c> function. Function informing the task to
         * perform its updates.  This does all the work needed to update graphics for this frame.
         *
         * @param   DeltaTime   The time delta from the last call.
         *
         * @sa   ISystemTask::Update .
         */
        virtual void Update(f32 DeltaTime);

        /**
         * Query if this GuiTask is primary thread only.
         * Implementation of the <c>ISystemTask::IsPrimaryThreadOnly</c> function.
         *
         * @return  bool - Returns if this system should only run on the primary thread.
         *
         * @sa   ISystemTask::IsPrimaryThreadOnly    .
         */
        virtual bool IsPrimaryThreadOnly(void) {
            return false;
        };

    private:

        GuiScene*                  m_pScene;

};

