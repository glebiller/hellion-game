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

#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include <Common\Base\hkBase.h>
#include <Physics2012\Collide\Shape\Misc\PhantomCallback\hkpPhantomCallbackShape.h>
#pragma warning( pop )

class PhysicScene;

///
/// <c>PhantomShape</c> Extends hkpPhantomCallbackShape to implement Phantom tracking.  See Havok
/// documentation for more details.
///
/// @sa hkpPhantomCallbackShape
///
class PhantomCallbackShape : public hkpPhantomCallbackShape {
public:

    ///
    /// Constructor.
    ///
    /// @param [in,out] pSystemScene    If non-null, the system scene.
    ///
    PhantomCallbackShape(PhysicScene* pSystemScene);

    ///
    /// Destructor.
    ///
    ~PhantomCallbackShape(void);

protected:

    /// <summary cref="PhantomShape::phantomEnterEvent">
    ///   Callback that handle objects entering this phantom
    ///   (see Havok documentation for more details)
    /// </summary>
    /// <param name="pPhantomCollidable">Phantom that was entered</param>
    /// <param name="pOtherCollidable">Collidable that entered the Phantom</param>
    /// <param name="CollisionInput">Collision input</param>
    virtual void phantomEnterEvent(const hkpCollidable* pPhantomCollidable,
                                    const hkpCollidable* pOtherCollidable,
                                    const hkpCollisionInput& CollisionInput);

    /// <summary cref="PhantomShape::phantomLeaveEvent">
    ///   Callback that handle objects leaving this phantom
    ///   (see Havok documentation for more details)
    /// </summary>
    /// <param name="pPhantomCollidable">Phantom that was departed</param>
    /// <param name="pOtherCollidable">Collidable that is leaving the Phantom</param>
    virtual void phantomLeaveEvent(const hkpCollidable* pPhantomCollidable,
                                    const hkpCollidable* pOtherCollidable);

private:
    PhysicScene*              m_pSystemScene;

};

