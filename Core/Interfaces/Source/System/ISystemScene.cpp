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

#include "System/ISystemScene.h"

#include "Assert.h"
#include "Errors.h"
#include "Generic/IEntity.h"
#include "Generic/IProperty.h"
#include "Generic/ISubject.h"
#include "System/ISystemObject.h"

/**
 * @inheritDoc
 */
ISystemScene::ISystemScene(ISystem* pSystem) 
    : ISubject()
    , IProperty()
    , m_pSystem(pSystem) {
    ASSERT(m_pSystem != nullptr);
}

/**
 * @inheritDoc
 */
ISystemScene::~ISystemScene() {

}

/**
 * @inheritDoc
 */
void ISystemScene::GlobalSceneStatusChanged(GlobalSceneStatus Status) {
}

/**
 * @inheritDoc
 */
Error ISystemScene::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void ISystemScene::createTask(void) {
    m_pSystemTask = m_TaskFactory(this);
}

/**
 * @inheritDoc
 */
ISystemObject* ISystemScene::CreateObject(IEntity* entity, std::string type) {
    ASSERT(m_bInitialized);

    ISystemObject* systemObject = m_ObjectFactories[type](this, entity);

    if (systemObject != nullptr) {
        m_pObjects[systemObject->getEntity()->getId()] = systemObject;
    } else {
        ASSERTMSG2(false, "Impossible to create the object with name %s and type %s", entity->getName(), type);
    }

    return systemObject;
}

/**
 * @inheritDoc
 */
Error ISystemScene::DestroyObject(ISystemObject* pSystemObject) {
    ASSERT(m_bInitialized);
    ASSERT(pSystemObject != nullptr);

    if (pSystemObject != nullptr) {
        m_pObjects.erase(pSystemObject->getEntity()->getId());
        delete pSystemObject;
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void ISystemScene::propertyChanged(System::Changes::BitMask uInChangedBits) {
    PostChanges(uInChangedBits);
}