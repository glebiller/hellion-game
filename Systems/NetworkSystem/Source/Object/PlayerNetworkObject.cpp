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

#include <boost/lexical_cast.hpp>

#include "Scene.h"
#include "Object/Object.h"
#include "Object/IMoveObject.h"
#include "Object/IGeometryObject.h"
#include "Object/PlayerNetworkObject.h"
#include "Proto/Server/DownstreamMessage.pb.h"
#include "Proto/Message.pb.h"

/**
 * @inheritDoc
 */
PlayerNetworkObject::PlayerNetworkObject(ISystemScene* pSystemScene, IEntity* entity) 
    : NetworkObject(pSystemScene, entity)
    , m_velocityDirty(true)
    , m_rotationDirty(true)
    , m_position(Math::Vector3::Zero)
    , m_orientation(Math::Quaternion::Zero)
    , m_velocity(Math::Vector3::Zero)
    , m_rotation(Math::Vector3::Zero)
    , m_heartbeat_delay(500000000LL) /* 500ms */ {
    m_heartbeat.stop();
}

/**
 * @inheritDoc
 */
PlayerNetworkObject::~PlayerNetworkObject(void) {
    
}

/**
 * @inheritDoc
 */
Error PlayerNetworkObject::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error PlayerNetworkObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    
    if (ChangeType & System::Changes::Physic::Position) {
        m_position = *dynamic_cast<IGeometryObject*>(pSubject)->GetPosition();
    }
    if (ChangeType & System::Changes::Physic::Orientation) {
        m_orientation = *dynamic_cast<IGeometryObject*>(pSubject)->GetOrientation();
    }
    if (ChangeType & System::Changes::Input::Velocity) {
        m_velocity = *dynamic_cast<IMoveObject*>(pSubject)->getVelocity();
        m_velocityDirty = true;
    }
    if (ChangeType & System::Changes::Input::Rotation) {
        m_rotation = *dynamic_cast<IMoveObject*>(pSubject)->getRotation();
        m_rotationDirty = true;
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void PlayerNetworkObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);

    // Send the packet everytime it's dirty or for a heartbeat
    bool heartbeat_triggered = !m_heartbeat.is_stopped() && m_heartbeat.elapsed().wall >= m_heartbeat_delay;
    if (m_velocityDirty || m_rotationDirty || heartbeat_triggered) {
        bool velocityNotNull = m_velocity != Math::Vector3::Zero;
        bool rotationNotNull = m_rotation != Math::Vector3::Zero;

        m_heartbeat.stop();
        if (velocityNotNull || rotationNotNull) {
            m_heartbeat.start();
        }
        
        Proto::ObjectUpdated objectUpdated;
        Proto::Object* object = objectUpdated.add_objects();
        object->set_id(m_entity->getId());
        object->set_name(m_entity->getName());
        Proto::SystemObject* systemObject = object->add_systemobjects();
        systemObject->set_type(Proto::SystemType_Name(Proto::SystemType::Network));
        systemObject->set_systemtype(Proto::SystemType::Network);

        if (m_velocityDirty || velocityNotNull) {
            m_velocityDirty = false;
            Proto::Property* positionProperty = systemObject->add_properties();
            positionProperty->set_name("Position");
            getVector3(&m_position, positionProperty->mutable_value());
            Proto::Property* velocityProperty = systemObject->add_properties();
            velocityProperty->set_name("Velocity");
            getVector3(&m_velocity, velocityProperty->mutable_value());
        }
        if (m_rotationDirty || rotationNotNull) {
            m_rotationDirty = false;
            Proto::Property* orientationProperty = systemObject->add_properties();
            orientationProperty->set_name("Orientation");
            getQuaternion(&m_orientation, orientationProperty->mutable_value());
            Proto::Property* rotationProperty = systemObject->add_properties();
            rotationProperty->set_name("Rotation");
            getVector3(&m_rotation, rotationProperty->mutable_value());
        }

        std::string data;
        objectUpdated.AppendToString(&data);
        DownstreamMessageProto downstreamMessage;
        downstreamMessage.set_type(DownstreamMessageProto::PLAYER_MOVE);
        downstreamMessage.set_data(data);
        GetSystemScene<NetworkScene>()->GetSystem<NetworkSystem>()->getNetworkService()->send(downstreamMessage);
    }
}