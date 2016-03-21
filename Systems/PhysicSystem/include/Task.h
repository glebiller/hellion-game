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

#include <list>
#include "System.h"
#include "System/ISystemTask.h"

class PhysicScene;
class ISystemScene;
class PhysicObject;
class hkpWorld;

///
/// <c>HavokPhysicsTask</c> Implementation of the ISystemTask interface for HavokPhysics.
///
/// @sa ISystemTask
///
class PhysicTask : public ISystemTask {
public:
    ///
    /// @inheritDoc.
    ///
    PhysicTask(ISystemScene* pScene);

    ///
    /// @inheritDoc.
    ///
    ~PhysicTask();
        
    ///
    /// @inheritDoc.
    ///
    void Update(float DeltaTime);
    
    ///
    /// @inheritDoc.
    ///
    bool IsPrimaryThreadOnly() {
        return false;
    };

    ///
    /// @inheritDoc.
    ///
    Schema::SystemType GetSystemType() {
        return Schema::SystemType::Physic;
    }

    ///
    /// Step update s.
    /// Advance the world owned by this thread (static version).
    ///
    /// @param [in,out] pTask   Pointer to Task that generated this call.
    /// @param  uStart          Start time (currently unused)
    /// @param  uEnd            The end.
    ///
    static void stepUpdateS(PhysicTask* pTask, unsigned int uStart, unsigned int uEnd);

    ///
    /// Gets active objects.
    ///
    /// @return null if it fails, else the active objects.
    ///
    inline std::list<PhysicObject*>* getActiveObjects() {
        return &m_ActiveObjects;
    }
    
private:
    ///
    /// Step update.
    /// Advance the world owned by this thread.
    ///
    void stepUpdate();

    ///
    /// Updates the completion.
    /// Callback function called when all threads have finished.
    ///
    void updateCompletion();

private:
    std::list<PhysicObject*>                m_ActiveObjects;

    unsigned int                                     m_cJobs;
    float                                     m_DeltaTime;

};

