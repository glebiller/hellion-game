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

#include <boost/assert.hpp>
#include <schema/component_type_generated.h>
#include <schema/scene_generated.h>
#include <Universal/UObject.h>

/**
 * @inheritDoc
 */
ISystemScene::ISystemScene(ISystem* pSystem, const Schema::SystemScene* systemScene)
        : ISubject()
        , m_pSystem(pSystem),
          systemScene(systemScene) {
    BOOST_ASSERT(m_pSystem != nullptr);
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
ISystemObject* ISystemScene::CreateObject(UObject* entity, const Schema::SystemComponent* component) {
    // TODO handle unkown factory
    ObjectFactory objectFactory = m_ObjectFactories[component->data_type()];
    ISystemObject* systemObject = objectFactory(*this, *entity, *component);
    BOOST_ASSERT_MSG(systemObject != nullptr, "Impossible to create the object");
    m_pObjects[systemObject->getEntity()->getId()] = systemObject;
    return systemObject;
}

/**
 * @inheritDoc
 */
Error ISystemScene::DestroyObject(ISystemObject* pSystemObject) {

    BOOST_ASSERT(pSystemObject != nullptr);

    if (pSystemObject != nullptr) {
        m_pObjects.erase(pSystemObject->getEntity()->getId());
        delete pSystemObject;
    }

    return Errors::Success;
}

Schema::SystemType ISystemScene::GetSystemType() {
    return systemScene->systemType();
}