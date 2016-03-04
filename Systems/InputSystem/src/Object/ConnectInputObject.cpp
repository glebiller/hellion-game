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
#include <OIS.h>

#include "Generic/IProperty.h"
#include "Generic/UObject.h"
#include "Scene.h"
#include "ConnectInputObject.h"

/**
 * @inheritDoc
 */
ConnectInputObject::ConnectInputObject(ISystemScene* pSystemScene, UObject* entity)
    : InputObject(pSystemScene, entity) {
    m_propertySetters["Key"] = boost::bind(&IProperty::setString, this, System::Changes::None, &m_key, _1);
    m_propertyGetters["Key"] = boost::bind(&IProperty::getString, this, &m_key, _1);
    
    InputScene* inputScene = GetSystemScene<InputScene>();
    m_connectInputAction = inputScene->getDefaultSchema()->createAction<OISB::TriggerAction>(entity->getName());
}

/**
 * @inheritDoc
 */
ConnectInputObject::~ConnectInputObject() {
}

/**
 * @inheritDoc
 */
Error ConnectInputObject::initialize() {
    ASSERT(!m_bInitialized);
    
    m_connectInputAction->bind(m_key);

    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error ConnectInputObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    return Errors::Success;
}

void ConnectInputObject::Update(f32 DeltaTime) {
    ASSERT(m_bInitialized);
    m_modified = 0;

    if (m_connectInputAction->hasChanged()) {
        m_keyboardButtonData.type = 0;
        m_keyboardButtonData.down = m_connectInputAction->isActive();
        m_modified |= System::Changes::Input::Action;
        Proto::Object objectProto;
        objectProto.set_id(this->getEntity()->getParent()->getId());
        static_cast<InputScene*>(m_pSystemScene)->queueDeleteObject(objectProto);
    }
    
    PostChanges(m_modified);
}
