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

#include "Object/PhysicObject.h"

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "Scene.h"

/**
 * @inheritDoc
 */
PhysicObject::PhysicObject(ISystemScene& pSystemScene, UObject& entity, const Schema::SystemComponent& component)
    : ISystemObject(&pSystemScene, &entity)
    , m_bStatic(false) {
    position_ = const_cast<Schema::PhysicPosition*>(static_cast<const Schema::PhysicPosition*>(component.data()));
}

/**
 * @inheritDoc
 */
PhysicObject::~PhysicObject() {

}

/**
 * @inheritDoc
 */
Error PhysicObject::initialize() {
    ASSERT(!m_bInitialized);

    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error PhysicObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void PhysicObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);

    float newX = position_->x() + (1 * DeltaTime);
    if (newX >= 20) {
        newX = 0;
    }
    bool result = position_->mutate_x(newX);
    ASSERT(result);

    PostChanges(System::Changes::Physic::Position);
}