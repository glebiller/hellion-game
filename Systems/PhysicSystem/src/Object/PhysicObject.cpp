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


#include <BulletCollision/CollisionShapes/btCapsuleShape.h>
#include <schema/entity_change_generated.h>
#include <boost/core/ignore_unused.hpp>

#include "Object/PhysicObject.h"


#include "Scene.h"

/**
 * @inheritDoc
 */
PhysicObject::PhysicObject(ISystemScene& pSystemScene, UObject& entity, const Schema::SystemComponent& component)
    : ISystemObject(&pSystemScene, &entity, component)
    , m_bStatic(false) {
    position_ = const_cast<Schema::Components::PhysicPosition*>(static_cast<const Schema::Components::PhysicPosition*>(component.data()));

}

/**
 * @inheritDoc
 */
PhysicObject::~PhysicObject() {

}

/**
 * @inheritDoc
 */
Error PhysicObject::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void PhysicObject::Update(float DeltaTime) {
    boost::ignore_unused(DeltaTime);
}