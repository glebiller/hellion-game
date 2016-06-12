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

#include "Object/MovablePhysicObject.h"

#include "Scene.h"

/**
 * @inheritDoc
 */
MovablePhysicObject::MovablePhysicObject(ISystemScene& pSystemScene, UObject& entity, const Schema::SystemComponent& component)
    : PhysicObject(pSystemScene, entity, component)
    , m_constraint_position(true)
    , m_velocity_multiplier(40)
    , m_rotation_multiplier(400) {
}

/**
 * @inheritDoc
 */
MovablePhysicObject::~MovablePhysicObject() {
    
}

/**
 * @inheritDoc
 */
Error MovablePhysicObject::ChangeOccurred(ISystemObject* systemObject, IObserver::Changes changes) {
    if (changes & System::Changes::Input::Velocity) {
        //m_velocity = *dynamic_cast<IMoveObject*>(pSubject)->getVelocity();
    }
    if (changes & System::Changes::Input::Rotation) {
        //m_rotation = *dynamic_cast<IMoveObject*>(pSubject)->getRotation();
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void MovablePhysicObject::Update(float DeltaTime) {
    // Rotation
    /*if (!transform_.getRotation()..isZero()) {
        Math::Quaternion additionalOrientation;
        //additionalOrientation.Set(m_rotation, Math::Angle::Deg2Rad(m_rotation_multiplier * DeltaTime));
        m_orientation *= additionalOrientation;

        m_modified |= System::Changes::Physic::Orientation;
    }*/
    
    // Velocity
    /*if (transform_.getOrigin().isZero()) {
        Math::Vector3 normalizedVelocity = Math::Vector3(m_velocity.x, m_velocity.y, m_velocity.z);
        m_orientation.Rotate(normalizedVelocity);
        normalizedVelocity.Normalize();

        m_position.x += normalizedVelocity.x * m_velocity_multiplier * DeltaTime;
        m_position.y += normalizedVelocity.y * m_velocity_multiplier * DeltaTime;
        m_position.z += normalizedVelocity.z * m_velocity_multiplier * DeltaTime;

        m_position.x = (m_constraint_position && m_position.x > 35) ? 35 : ((m_constraint_position && m_position.x < -35) ? -35 : m_position.x);
        m_position.y = (m_constraint_position && m_position.y > 25) ? 25 : ((m_constraint_position && m_position.y < -25) ? -25 : m_position.y);
            
        m_modified |= System::Changes::Physic::Position;
    }

    if (m_modified != System::Changes::None) {
        PostChanges(m_modified);
    }
     */
}