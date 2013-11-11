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

#include <list>

#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include <Common\Base\hkBase.h>
#include <Physics2012\Dynamics\Entity\hkpEntityListener.h>
#include <Physics2012\Dynamics\Entity\hkpEntityActivationListener.h>
#pragma warning( pop )

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
class PhysicTask : public ISystemTask, public hkpEntityListener, public hkpEntityActivationListener {
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
    void Update(f32 DeltaTime);
    
    ///
    /// @inheritDoc.
    ///
    bool IsPrimaryThreadOnly() {
        return false;
    };

    ///
    /// @inheritDoc.
    ///
    Proto::SystemType GetSystemType() {
        return Proto::SystemType::Physic;
    }

    ///
    /// Step update s.
    /// Advance the world owned by this thread (static version).
    ///
    /// @param [in,out] pTask   Pointer to Task that generated this call.
    /// @param  uStart          Start time (currently unused)
    /// @param  uEnd            The end.
    ///
    static void stepUpdateS(PhysicTask* pTask, u32 uStart, u32 uEnd);

    ///
    /// Sets object activation.
    /// Add or removes the given object from actively tracked objects.
    ///
    /// @param [in,out] pObject The Object that we want to activate/deactivate.
    /// @param  bActivated      (Optional) the activated.
    ///
    void setObjectActivation(PhysicObject* pObject, bool bActivated = true);
    
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

    ///
    /// Callback, called when the entity added.
    /// Callback for when an entity is added by Havok. (See Havok documentation for more details)
    ///
    /// @param [in,out] pEntity If non-null, the entity.
    ///
    virtual void entityAddedCallback(hkpEntity* pEntity);

    ///
    /// Callback, called when the entity removed. Callback for when an entity is removed by
    /// Havok. (See Havok documentation for more details)
    ///
    /// @param [in,out] pEntity If non-null, the entity.
    ///
    virtual void entityRemovedCallback(hkpEntity* pEntity);

    ///
    /// Callback, called when the entity deactivated.
    /// Callback for when an entity is deactived by Havok. (See Havok documentation for more details)
    ///
    /// @param [in,out] pEntity If non-null, the entity.
    ///
    virtual void entityDeactivatedCallback(hkpEntity* pEntity);

    ///
    /// Callback, called when the entity activated.
    /// Callback for when an entity is actived by Havok. (See Havok documentation for more details)
    ///
    /// @param [in,out] pEntity If non-null, the entity.
    ///
    virtual void entityActivatedCallback(hkpEntity* pEntity);

private:
#ifdef __HAVOK_VDB__
    hkpPhysicsContext*                      m_pPhysicsContext;
    hkVisualDebugger*                       m_pVisualDebugger;
#endif

    hkJobQueue*                             m_jobQueue;
    std::list<PhysicObject*>                m_ActiveObjects;

    u32                                     m_cJobs;
    f32                                     m_DeltaTime;

};

