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

#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include <Common\Base\hkBase.h>
#include <Physics2012/Dynamics/Phantom/hkpPhantomOverlapListener.h>
#pragma warning( pop )

///
/// <c>PhantomShape</c> Extends hkpPhantomCallbackShape to implement Phantom tracking.  See Havok
/// documentation for more details.
///
/// @sa hkpSimpleShapePhantom
///
class IgnorePhantomOverlapListener : public hkpPhantomOverlapListener {
public:

    ///
    /// Constructor.
    ///
    /// @param [in,out] pSystemScene    If non-null, the system scene.
    ///
    IgnorePhantomOverlapListener();

    ///
    /// Destructor.
    ///
    ~IgnorePhantomOverlapListener();

    ///
    /// Callback, called when the collidable added.
    ///
    /// @param  event   The event.
    ///
    virtual void collidableAddedCallback(const hkpCollidableAddedEvent& event);

    ///
    /// Callback, called when the collidable removed.
    ///
    /// @param  event   The event.
    ///
    virtual void collidableRemovedCallback(const hkpCollidableRemovedEvent& event);

};

