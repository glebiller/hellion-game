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
#include "Generic/IComponent.h"
#include "Generic/IUpdatable.h"

class ISystemScene;

/**
 * <c>ISystemTask</c> is an interface class designed to work with a task manager for starting the
 *  system's task and spawning off new tasks as need be.
 */
class ISystemTask : public IComponent, public IUpdatable {
public:

    ///
    /// Constructor.
    ///
    /// @param [in,out] pSystemScene    If non-null, the system scene.
    ///
    ISystemTask(ISystemScene* pSystemScene);

    ///
    /// Destructor.
    ///
    virtual ~ISystemTask();
    
    ///
    /// @inheritDoc.
    ///
    virtual Error initialize() = 0;

    ///
    /// Query if this ISystemTask is primary thread only. Implementing tasks should return true
    /// to indicate that their <c>Update</c> function should only be called from the primary thread.
    /// false allows their <c>Update</c> function to be called from an arbitrary thread.
    ///
    /// @return true if primary thread only, false if not.
    ///
    virtual bool IsPrimaryThreadOnly() = 0;

    ///
    /// Gets the system scene. Gets the scene for this task.
    ///
    /// @tparam typename TSystemScene   Type of the typename t system scene.
    ///
    /// @return The scene for this task.
    ///
    template <typename TSystemScene>
    inline TSystemScene* GetSystemScene() {
        return static_cast<TSystemScene*>(m_pSystemScene);
    }

protected:
    ISystemScene*               m_pSystemScene;

};