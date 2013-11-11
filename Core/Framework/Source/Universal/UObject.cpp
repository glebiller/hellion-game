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

#include "Universal/UScene.h"
#include "Universal/UObject.h"
#include "Object/ISceneObject.h"
#include "Service/SystemService.h"
#include "Manager/IChangeManager.h"
#include "Manager/IServiceManager.h"
#include "System/ISystemScene.h"
#include "System/ISystemObject.h"

/**
 * @inheritDoc
 */
UObject::UObject(UScene* pScene, std::string id, std::string name, IEntity* parent)
    : ISubject()
    , IEntity(id, name, parent)
    , m_pScene(pScene)
    , m_pObjectCCM(pScene->getObjectCCM()) {
}

/**
 * @inheritDoc
 */
UObject::~UObject(void) {
    //
    // Remove all the extensions.
    //
    SystemObjects SysObjs = m_ObjectExtensions;

    for (auto it = SysObjs.begin(); it != SysObjs.end(); it++) {
        Unextend(it->second->GetSystemScene<ISystemScene>());
    }

    m_ObjectExtensions.clear();
}

/**
 * @inheritDoc
 */
ISystemObject* UObject::Extend(ISystemScene* pSystemScene, std::string systemObjectType) {
    ASSERT(pSystemScene != NULL);
    ASSERT(m_ObjectExtensions.find(pSystemScene->GetSystemType()) == m_ObjectExtensions.end());

    //
    // Create the system object.
    //
    ISystemObject* pSystemObject = pSystemScene->CreateObject(this, systemObjectType);
    ASSERT(pSystemObject != NULL);

    //
    // Get the changes this object will make and is looking for.
    //
    System::Changes::BitMask SysObjPotentialChanges = pSystemObject->GetPotentialSystemChanges();
    System::Changes::BitMask SysObjDesiredChanges = pSystemObject->GetDesiredSystemChanges();;

    //
    // Register each object with scenes that care about the object's changes.
    //
    UScene::SystemScenes pScenes = m_pScene->GetSystemScenes();
    for (auto it : pScenes) {
        ISystemScene* pScene = it.second;

        if (pSystemObject->GetPotentialSystemChanges() & pScene->GetDesiredSystemChanges()) {
            m_pObjectCCM->Register(pSystemObject, pScene->GetDesiredSystemChanges(), pScene);
        }
    }

    //
    // Register each of the systems with each other.
    //
    System::Changes::BitMask Changes = pSystemObject->GetDesiredSystemChanges();
    for (auto it : m_ObjectExtensions) {
        ISystemObject* pObj = it.second;

        if (pObj->GetPotentialSystemChanges() & SysObjDesiredChanges) {
            m_pObjectCCM->Register(pObj, Changes, pSystemObject);
        }

        if (SysObjPotentialChanges & pObj->GetDesiredSystemChanges()) {
            m_pObjectCCM->Register(pSystemObject, pObj->GetDesiredSystemChanges(), pObj);
        }
    }

    //
    // Add the system object to the list.
    //
    Proto::SystemType SystemType = pSystemObject->GetSystemType();
    m_ObjectExtensions[SystemType] = pSystemObject;

    //
    // Set up the speed path for the geometry and graphics objects.
    //
    // TODO is it really usefull ?
    /*if (SystemType == Proto::SystemType::Physic) {
        m_pGeometryObject = dynamic_cast<IGeometryObject*>(pSystemObject);
        ASSERT(m_pGeometryObject != NULL);
    }*/

    return pSystemObject;
}

/**
 * @inheritDoc
 */
void UObject::Unextend(ISystemScene* pSystemScene) {
    ASSERT(pSystemScene != NULL);
    //
    // Get the iterator for the object.
    //
    Proto::SystemType SystemType = pSystemScene->GetSystem<ISystem>()->GetSystemType();
    auto SysObjIt = m_ObjectExtensions.find(SystemType);
    ASSERTMSG(SysObjIt != m_ObjectExtensions.end(), "The object to delete doesn't exist in the scene.");
    ISystemObject* pSystemObject = SysObjIt->second;

    //
    // Go through all the other systems and unregister them with this as subject and observer.
    //  The CCM should know if the objects are registered or not, and if not won't do anything.
    //
    for (auto it : m_ObjectExtensions) {
        ISystemObject* pObj = it.second;

        //
        // Make sure he system object is not unregistering as an observer of itself.
        //
        if (pSystemObject != pObj) {
            m_pObjectCCM->Unregister(pObj, pSystemObject);
            m_pObjectCCM->Unregister(pSystemObject, pObj);
        }
    }

    //
    // Unregister each object with scenes that cared about the object's changes.
    //
    UScene::SystemScenes pScenes = m_pScene->GetSystemScenes();
    for (auto it = pScenes.begin(); it != pScenes.end(); it++) {
        ISystemScene* pScene = it->second;

        if (pSystemObject->GetPotentialSystemChanges() & pScene->GetDesiredSystemChanges()) {
            m_pObjectCCM->Unregister(pSystemObject,  pScene);
        }
    }

    //
    // Destroy the object.
    //
    pSystemScene->DestroyObject(pSystemObject);
    //
    // Remove the object from the map.
    //
    m_ObjectExtensions.erase(SysObjIt);
}

/**
 * @inheritDoc
 */
const UObject::SystemObjects& UObject::GetExtensions(void) {
    return m_ObjectExtensions;
}

/**
 * @inheritDoc
 */
ISystemObject* UObject::GetExtension(Proto::SystemType SystemType) {
    ISystemObject* pSystemObject = nullptr;

    auto it = m_ObjectExtensions.find(SystemType);
    if (it != m_ObjectExtensions.end()) {
        pSystemObject = it->second;
    }

    return pSystemObject;
}

/**
 * @inheritDoc
 */
Error UObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
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
