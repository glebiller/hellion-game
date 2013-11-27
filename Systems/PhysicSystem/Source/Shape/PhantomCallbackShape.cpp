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

#include "Shape/PhantomCallbackShape.h"

#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include <Common\Base\hkBase.h>
#include <Physics2012\Dynamics\Entity\hkpRigidBody.h>
#include <Physics2012\Collide\Shape\hkpShape.h>
#include <Physics2012\Collide\Shape\Convex\Box\hkpBoxShape.h>
#include <Physics2012\Collide\Shape\Convex\Sphere\hkpSphereShape.h>
#include <Physics2012\Collide\Shape\Convex\ConvexVertices\hkpConvexVerticesShape.h>
#include <Physics2012\Collide\Shape\Deprecated\Mesh\hkpMeshShape.h>
#include <Physics2012\Collide\Shape\Compound\Tree\Mopp\hkpMoppCompilerInput.h>
#include <Physics2012\Collide\Shape\Compound\Tree\Mopp\hkpMoppUtility.h>
#include <Physics2012\Collide\Shape\Compound\Tree\Mopp\hkpMoppBvTreeShape.h>
#include <Physics2012\Collide\Shape\HeightField\hkpHeightFieldShape.h>
#include <Physics2012\Collide\Shape\Misc\Bv\hkpBvShape.h>
#include <Physics2012\Collide\Shape\Misc\PhantomCallback\hkpPhantomCallbackShape.h>
#include <Physics2012\Utilities\Destruction\BreakOffParts\hkpBreakOffPartsUtil.h>
#pragma warning( pop )

#include "Scene.h"
#include "Object/PhysicObject.h"

#define PHAVOKSCENE         reinterpret_cast<PhysicScene*>(m_pSystemScene)

///////////////////////////////////////////////////////////////////////////////
// PhantomShape - Default constructor
PhantomCallbackShape::PhantomCallbackShape(PhysicScene* pSystemScene)
    : m_pSystemScene(pSystemScene) {
}


///////////////////////////////////////////////////////////////////////////////
// ~PhantomShape - Default destructor
PhantomCallbackShape::~PhantomCallbackShape() {
}


///////////////////////////////////////////////////////////////////////////////
// phantomEnterEvent - Callback that handle objects entering this phantom
void
PhantomCallbackShape::phantomEnterEvent(
    const hkpCollidable* pPhantomCollidable,
    const hkpCollidable* pOtherCollidable,
    const hkpCollisionInput& CollisionInput
) {
    //
    // Get the objects for the collidables.
    //
    hkpRigidBody* pOtherBody = reinterpret_cast<hkpRigidBody*>(pOtherCollidable->getOwner());
    hkpRigidBody* pPhantomBody = reinterpret_cast<hkpRigidBody*>(pPhantomCollidable->getOwner());
    PhysicObject* pSubject = reinterpret_cast<PhysicObject*>(pOtherBody->getUserData());
    PhysicObject* pObserver = reinterpret_cast<PhysicObject*>(pPhantomBody->getUserData());
}


///////////////////////////////////////////////////////////////////////////////
// phantomLeaveEvent - Callback that handle objects leaving this phantom
void
PhantomCallbackShape::phantomLeaveEvent(
    const hkpCollidable* pPhantomCollidable,
    const hkpCollidable* pOtherCollidable
) {
    //
    // Get the objects for the collidables.
    //
    hkpRigidBody* pOtherBody = reinterpret_cast<hkpRigidBody*>(pOtherCollidable->getOwner());
    hkpRigidBody* pPhantomBody = reinterpret_cast<hkpRigidBody*>(pPhantomCollidable->getOwner());
    PhysicObject* pSubject = reinterpret_cast<PhysicObject*>(pOtherBody->getUserData());
    PhysicObject* pObserver = reinterpret_cast<PhysicObject*>(pPhantomBody->getUserData());
}
