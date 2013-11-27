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

#include "Shape/IgnorePhantomOverlapListener.h"

#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include <Common\Base\hkBase.h>
#include <Physics2012/Collide/Agent/Collidable/hkpCollidable.h>
#include <Physics2012/Dynamics/Entity/hkpRigidBody.h>
#pragma warning( pop )

///
/// @inheritDoc
///
IgnorePhantomOverlapListener::IgnorePhantomOverlapListener() {
}

///
/// @inheritDoc
///
IgnorePhantomOverlapListener::~IgnorePhantomOverlapListener() {
}

///
/// @inheritDoc
///
void IgnorePhantomOverlapListener::collidableAddedCallback(const hkpCollidableAddedEvent& event) {
    const hkpCollidable *collidable = event.m_collidable;

    int type = collidable->getType();
    if (type == hkpRigidBody::BROAD_PHASE_PHANTOM) {
        hkpWorldObject* wo = hkpGetWorldObject(collidable);
        //if (wo->hasProperty(OBJECT_MARKED_FOR_DETECTION)) {
            event.m_collidableAccept = HK_COLLIDABLE_REJECT;
        //}
    }
}

///
/// @inheritDoc
///
void IgnorePhantomOverlapListener::collidableRemovedCallback(const hkpCollidableRemovedEvent& event) {
}