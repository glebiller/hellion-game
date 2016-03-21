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

#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>

#include "Scene.h"
#include "Generic/UObject.h"
#include "Object/Object.h"
#include "Object/UpdatableNetworkObject.h"
#include "Proto/Server/DownstreamMessage.pb.h"
#include "Proto/Message.pb.h"

/**
 * @inheritDoc
 */
UpdatableNetworkObject::UpdatableNetworkObject(ISystemScene* pSystemScene, UObject* entity)
    : NetworkObject(pSystemScene, entity)
    , IGeometryObject() {
    m_propertySetters["Position"] = boost::bind(&IProperty::setVector3, this, System::Changes::Physic::Position, &m_position, _1);
    m_propertyGetters["Position"] = boost::bind(&IProperty::getVector3, this, &m_position, _1);
    
    m_propertySetters["Orientation"] = boost::bind(&IProperty::setQuaternion, this, System::Changes::Physic::Orientation, &m_orientation, _1);
    m_propertyGetters["Orientation"] = boost::bind(&IProperty::getQuaternion, this, &m_orientation, _1);

    m_propertySetters["Velocity"] = boost::bind(&IProperty::setVector3, this, System::Changes::Input::Velocity, &m_velocity, _1);
    m_propertyGetters["Velocity"] = boost::bind(&IProperty::getVector3, this, &m_velocity, _1);
    
    m_propertySetters["Rotation"] = boost::bind(&IProperty::setVector3, this, System::Changes::Input::Rotation, &m_rotation, _1);
    m_propertyGetters["Rotation"] = boost::bind(&IProperty::getVector3, this, &m_rotation, _1);
}

/**
 * @inheritDoc
 */
UpdatableNetworkObject::~UpdatableNetworkObject() {
    
}

/**
 * @inheritDoc
 */
Error UpdatableNetworkObject::initialize() {

    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error UpdatableNetworkObject::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {


    return Errors::Success;
}

/**
 * @inheritDoc
 */
void UpdatableNetworkObject::Update(float DeltaTime) {


}
