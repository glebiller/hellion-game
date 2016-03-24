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

#include "Universal/UObject.h"

#include <schema/component_type_generated.h>
#include "Manager/ChangeManager.h"
#include "Universal/UScene.h"

/**
 * @inheritDoc
 */
UObject::UObject(UScene* pScene, const Schema::SceneEntity &sceneEntity, UObject* parent)
        : m_pObjectCCM(pScene->getObjectCCM())
        , m_pScene(pScene) {
    entitySchema_ = &sceneEntity;
}

/**
 * @inheritDoc
 */
UObject::~UObject() {
    for (auto& it : m_ObjectExtensions) {
        Unextend(it.second->GetSystemScene<ISystemScene>());
    }

    m_ObjectExtensions.clear();
}

/**
 * @inheritDoc
 */
ISystemObject* UObject::Extend(ISystemScene* pSystemScene, const Schema::SystemComponent* component) {
    BOOST_ASSERT_MSG(pSystemScene != nullptr, "SystemScene cannot be null");

    ISystemObject* pSystemObject = pSystemScene->CreateObject(this, component);
    for (auto it : m_pScene->GetSystemScenes()) {
        m_pObjectCCM->Register(pSystemObject, it.second);
    }
    for (auto it : m_ObjectExtensions) {
        m_pObjectCCM->Register(it.second, pSystemObject);
        m_pObjectCCM->Register(pSystemObject, it.second);
    }

    m_ObjectExtensions[component->data_type()] = pSystemObject;
    return pSystemObject;
}

/**
 * @inheritDoc
 */
void UObject::Unextend(ISystemScene* pSystemScene) {
    BOOST_ASSERT(pSystemScene != NULL);

    for(auto iterator = m_ObjectExtensions.begin(); iterator != m_ObjectExtensions.end(); iterator++) {
        auto systemObject = iterator->second;
        if (systemObject->GetSystemType() != pSystemScene->GetSystemType()) {
            continue;
        }

        //
        // Go through all the other systems and unregister them with this as subject and observer.
        //  The CCM should know if the objects are registered or not, and if not won't do anything.
        //
        for (auto& it : m_ObjectExtensions) {
            if (systemObject != it.second) {
                m_pObjectCCM->Unregister(it.second, systemObject);
                m_pObjectCCM->Unregister(systemObject, it.second);
            }
        }

        //
        // Unregister each object with scenes that cared about the object's changes.
        //
        UScene::SystemScenes pScenes = m_pScene->GetSystemScenes();
        for (auto it = pScenes.begin(); it != pScenes.end(); it++) {
            if (systemObject->GetPotentialSystemChanges() & it->second->GetDesiredSystemChanges()) {
                m_pObjectCCM->Unregister(systemObject, it->second);
            }
        }

        pSystemScene->DestroyObject(systemObject);
        m_ObjectExtensions.erase(iterator->first);
    }
}

/**
 * @inheritDoc
 */
const std::map<Schema::ComponentType, ISystemObject*> &UObject::GetExtensions() {
    return m_ObjectExtensions;
}

/**
 * @inheritDoc
 */
ISystemObject* UObject::GetExtension(Schema::ComponentType componentType) {
    ISystemObject* pSystemObject = nullptr;

    auto it = m_ObjectExtensions.find(componentType);
    if (it != m_ObjectExtensions.end()) {
        pSystemObject = it->second;
    }

    return pSystemObject;
}

/**
 * @inheritDoc
 */
Error UObject::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {
    // Objects are probably not observers,
    // Objects can be subjects through links
    // TODO : check
    /*if (ChangeType & System::Changes::Generic::All) {
        //
        // Post the pertinent changes made by the extension to the scene CCM.
        //
        PostChanges(ChangeType);
    }*/

    return Errors::Success;
}
