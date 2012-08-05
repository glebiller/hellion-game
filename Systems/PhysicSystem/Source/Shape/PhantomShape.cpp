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



//
// extern includes
//
#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include "Common\Base\hkBase.h"
#include "Physics\Dynamics\Entity\hkpRigidBody.h"
#include "Physics\Collide\Shape\hkpShape.h"
#include "Physics\Collide\Shape\Convex\Box\hkpBoxShape.h"
#include "Physics\Collide\Shape\Convex\Sphere\hkpSphereShape.h"
#include "Physics\Collide\Shape\Convex\ConvexVertices\hkpConvexVerticesShape.h"
#include "Physics\Collide\Shape\Deprecated\Mesh\hkpMeshShape.h"
#include "Physics\Collide\Shape\Compound\Tree\Mopp\hkpMoppCompilerInput.h"
#include "Physics\Collide\Shape\Compound\Tree\Mopp\hkpMoppUtility.h"
#include "Physics\Collide\Shape\Compound\Tree\Mopp\hkpMoppBvTreeShape.h"
#include "Physics\Collide\Shape\HeightField\hkpHeightFieldShape.h"
#include "Physics\Collide\Shape\Misc\Bv\hkpBvShape.h"
#include "Physics\Collide\Shape\Misc\PhantomCallback\hkpPhantomCallbackShape.h"
#include "Physics\Utilities\Destruction\BreakOffParts\hkpBreakOffPartsUtil.h"
#pragma warning( pop )

//
// core includes
//
#include "BaseTypes.h"
#include "Interface.h"

//
// system includes
//
#include "Scene.h"
#include "Object/Object.h"
#include "Object/ObjectPhysics.h"
#include "PhantomShape.h"


#define PHAVOKSCENE         reinterpret_cast<HavokPhysicsScene*>(m_pSystemScene)


///////////////////////////////////////////////////////////////////////////////
// PhantomShape - Default constructor
PhantomShape::PhantomShape(
    HavokPhysicsScene* pSystemScene
)
    : m_pSystemScene(pSystemScene) {
}


///////////////////////////////////////////////////////////////////////////////
// ~PhantomShape - Default destructor
PhantomShape::~PhantomShape(
    void
) {
}


///////////////////////////////////////////////////////////////////////////////
// phantomEnterEvent - Callback that handle objects entering this phantom
void
PhantomShape::phantomEnterEvent(
    const hkpCollidable* pPhantomCollidable,
    const hkpCollidable* pOtherCollidable,
    const hkpCollisionInput& CollisionInput
) {
    //
    // Get the objects for the collidables.
    //
    hkpRigidBody* pOtherBody = reinterpret_cast<hkpRigidBody*>(pOtherCollidable->getOwner());
    hkpRigidBody* pPhantomBody = reinterpret_cast<hkpRigidBody*>(pPhantomCollidable->getOwner());
    HavokPhysicsObject* pSubject = reinterpret_cast<HavokPhysicsObject*>(pOtherBody->getUserData());
    HavokPhysicsObject* pObserver = reinterpret_cast<HavokPhysicsObject*>(pPhantomBody->getUserData());
    //
    // Track the movement within the body.
    //
    m_pSystemScene->AddTrackCollision(pSubject, pObserver);
}


///////////////////////////////////////////////////////////////////////////////
// phantomLeaveEvent - Callback that handle objects leaving this phantom
void
PhantomShape::phantomLeaveEvent(
    const hkpCollidable* pPhantomCollidable,
    const hkpCollidable* pOtherCollidable
) {
    //
    // Get the objects for the collidables.
    //
    hkpRigidBody* pOtherBody = reinterpret_cast<hkpRigidBody*>(pOtherCollidable->getOwner());
    hkpRigidBody* pPhantomBody = reinterpret_cast<hkpRigidBody*>(pPhantomCollidable->getOwner());
    HavokPhysicsObject* pSubject = reinterpret_cast<HavokPhysicsObject*>(pOtherBody->getUserData());
    HavokPhysicsObject* pObserver = reinterpret_cast<HavokPhysicsObject*>(pPhantomBody->getUserData());
    //
    // Removing tracking within the body.
    //
    m_pSystemScene->RemoveTrackCollision(pSubject, pObserver);
}
