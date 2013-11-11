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

#include "Generic/IEntity.h"
#include "Object/Object.h"
#include "System/ISystemObject.h"

/**
 * @inheritDoc
 */
InputObject::InputObject(ISystemScene* pSystemScene, IEntity* entity) 
    : ISystemObject(pSystemScene, entity) {
    m_propertySetters["Velocity"] = boost::bind(&IProperty::setVector3, this, System::Changes::Input::Velocity, &m_velocity, _1);
    m_propertyGetters["Velocity"] = boost::bind(&IProperty::getVector3, this, &m_velocity, _1);

    m_propertySetters["Rotation"] = boost::bind(&IProperty::setVector3, this, System::Changes::Input::Rotation, &m_rotation, _1);
    m_propertyGetters["Rotation"] = boost::bind(&IProperty::getVector3, this, &m_rotation, _1);
}


/**
 * @inheritDoc
 */
InputObject::~InputObject(void) {

}

