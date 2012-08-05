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

#include "SystemTypes.h"

class ISystemScene;

/**
 * <c>ISystemTask</c> is an interface class designed to work with a task manager for starting the
 *  system's task and spawning off new tasks as need be.
 */
class ISystemTask {
        friend class ISystem;

    public:

        /**
         * Constructor.
         * Constructor.
         *
         * @param [in,out]  pSystemScene    If non-null, the system scene.
         */
        ISystemTask(ISystemScene* pSystemScene)
            : m_pSystemScene(pSystemScene) {
        }

        /**
         * Gets the system scene.
         * Gets the scene for this task.
         *
         * @return  The scene for this task.
         */
        ISystemScene* GetSystemScene(void) {
            return m_pSystemScene;
        }

        /**
         * Gets the system type.
         * Gets the system type for this system task.
         *
         * @return  The type of the system.
         *
         * ### remarks  This is a shortcut to getting the system type without having to go the system
         *              first.
         */
        virtual System::Type GetSystemType(void) = 0;

        /**
         * Updates the given DeltaTime.
         * Function informing the task to perform its updates.
         *
         * @param   DeltaTime   The time delta from the last call.
         */
        virtual void Update(f32 DeltaTime) = 0;

        /**
         * Query if this ISystemTask is primary thread only.
         * Implementing tasks should return true to indicate that their <c>Update</c> function should
         * only be called from the primary thread.  false allows their <c>Update</c> function to be
         * called from an arbitrary thread.
         *
         * @return  true if primary thread only, false if not.
         */
        virtual bool IsPrimaryThreadOnly(void) = 0;

    protected:

        ISystemScene*               m_pSystemScene;

};