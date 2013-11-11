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
#include "Object/ReplicableNetworkObject.h"
#include "Proto/Server/DownstreamMessage.pb.h"
#include "Proto/Message.pb.h"

/**
 * @inheritDoc
 */
ReplicableNetworkObject::ReplicableNetworkObject(ISystemScene* pSystemScene, IEntity* entity) 
    : NetworkObject(pSystemScene, entity)
    , m_velocityDirty(true)
    , m_rotationDirty(true)
    , m_position(Math::Vector3::Zero)
    , m_orientation(Math::Quaternion::Zero) 
    , m_velocity(Math::Vector3::Zero)
    , m_rotation(Math::Vector3::Zero) {
}

/**
 * @inheritDoc
 */
ReplicableNetworkObject::~ReplicableNetworkObject(void) {
    
}

/**
 * @inheritDoc
 */
Error ReplicableNetworkObject::initialize(void) {
    ASSERT(!m_bInitialized);
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error ReplicableNetworkObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
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
void ReplicableNetworkObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);

    // Send the packet everytime it's dirty
    if (m_velocityDirty || m_rotationDirty) {
        Proto::ObjectUpdated objectUpdated;
        Proto::Object* object = objectUpdated.add_objects();
        object->set_id(m_entity->getId());
        object->set_name(m_entity->getName());
        object->set_template_("ShotTemplate");
        
        Proto::SystemObject* inputSystemObject = object->add_systemobjects();
        inputSystemObject->set_type("Passive");
        inputSystemObject->set_systemtype(Proto::SystemType::Input);
        Proto::SystemObject* physicSystemObject = object->add_systemobjects();
        physicSystemObject->set_type("Movable");
        physicSystemObject->set_systemtype(Proto::SystemType::Physic);

        if (m_velocityDirty) {
            m_velocityDirty = false;
            Proto::Property* positionProperty = physicSystemObject->add_properties();
            positionProperty->set_name("Position");
            getVector3(&m_position, positionProperty->mutable_value());
            Proto::Property* velocityProperty = inputSystemObject->add_properties();
            velocityProperty->set_name("Velocity");
            getVector3(&m_velocity, velocityProperty->mutable_value());
        }

        if (m_rotationDirty) {
            m_rotationDirty = false;
            Proto::Property* orientationProperty = physicSystemObject->add_properties();
            orientationProperty->set_name("Orientation");
            getQuaternion(&m_orientation, orientationProperty->mutable_value());
            Proto::Property* rotationProperty = inputSystemObject->add_properties();
            rotationProperty->set_name("Rotation");
            getVector3(&m_rotation, rotationProperty->mutable_value());
        }

        std::string data;
        objectUpdated.AppendToString(&data);
        DownstreamMessageProto downstreamMessage;
        downstreamMessage.set_type(DownstreamMessageProto::PLAYER_SHOT);
        downstreamMessage.set_data(data);
        GetSystemScene<NetworkScene>()->GetSystem<NetworkSystem>()->getNetworkService()->send(downstreamMessage);
    }
}