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

#include <flatbuffers/util.h>
#include <Environment_generated.h>
#include "Generic/Framework.h"
#include "Universal/UScene.h"
#include "Universal/UObject.h"
#include "System/ISystemScene.h"
#include "Object/ISceneObject.h"
#include "Manager/ServiceManager.h"
#include "Manager/IChangeManager.h"

/**
 * @inheritDoc
 */
UScene::UScene(IChangeManager* pSceneCCM, IChangeManager* pObjectCCM, std::map<Schema::SystemType, ISystem*>& systems)
    : m_pSceneCCM(pSceneCCM)
    , m_pObjectCCM(pObjectCCM) {
    flatbuffers::LoadFile("UniversalScene.bin", true, &universalSceneData_);
    universalSceneSchema_ = Schema::GetUniversalScene(universalSceneData_.c_str());
    for (auto it : systems) {
        Extend(*it.second);
    }
}

/**
 * @inheritDoc
 */
UScene::~UScene() {
    //
    // Send "pre-destroying objects" message to the scene extensions.
    //
    for (auto it = m_SystemScenes.begin(); it != m_SystemScenes.end(); it++) {
        it->second->GlobalSceneStatusChanged(
            ISystemScene::GlobalSceneStatus::PreDestroyingObjects
        );
    }

    //
    // Get rid of all the links.
    //
    for (auto objectLink : m_ObjectLinks) {
        m_pObjectCCM->Unregister(objectLink.pSubject, objectLink.pObserver);
    }
    m_ObjectLinks.clear();

    //
    // Get rid of all the objects.
    //
    Objects objects = m_Objects;
    for (auto object : objects) {
        delete object;
    }
    m_Objects.clear();

    //
    // Send "post-destroying objects" message to the scene extensions then delete the scene.
    //
    SystemScenes systemScenes = m_SystemScenes;
    for (auto systemScene : systemScenes) {
        ISystemScene* pSystemScene = systemScene.second;
        pSystemScene->GlobalSceneStatusChanged(
            ISystemScene::GlobalSceneStatus::PostDestroyingObjects
        );
        Unextend(pSystemScene);
    }
    m_SystemScenes.clear();
}

/**
 * @inheritDoc
 */
void UScene::init() {
    // Create Entities
    for (auto entity : *universalSceneSchema_->entities()) {
        createSceneEntity(entity);
    }

    //
    // Process the link messages in the CCMs first, for both the object and scene CCMs.
    // The link needs to be established before any other messages come through.
    //
    m_pObjectCCM->DistributeQueuedChanges(System::Types::All, System::Changes::Link | System::Changes::ParentLink);
    m_pSceneCCM->DistributeQueuedChanges(System::Types::All, System::Changes::Link | System::Changes::ParentLink);

    //
    // Distribute changes for object and scene CCMs.
    // The UObject propagates some object messages up to the scene so it needs to go first.
    //
    m_pObjectCCM->DistributeQueuedChanges(System::Types::All, System::Changes::All);
    m_pSceneCCM->DistributeQueuedChanges(System::Types::All, System::Changes::All);
}

/**
 * @inheritDoc
 */
void UScene::update() {
    // Process first the object creation messages alone since it will
    // generate some object messages that need to be processed by the object CCM.
    m_pSceneCCM->DistributeQueuedChanges(System::Types::All, System::Changes::Generic::CreateObject);
    //
    // Distribute changes for object and scene CCMs.  The UObject propagates some object
    // messages up to the scene CCM so it needs to go first.
    m_pObjectCCM->DistributeQueuedChanges(System::Types::All, System::Changes::All);
    m_pSceneCCM->DistributeQueuedChanges(System::Types::All, System::Changes::All ^ System::Changes::Generic::CreateObject);
}

/**
 * @inheritDoc
 */
ISystemScene* UScene::Extend(ISystem& system) {
    BOOST_ASSERT_MSG(system != NULL, "Cannot extend Universal Scene with null system");

    Schema::SystemType systemType = system.GetSystemType();
    BOOST_LOG(logger_) << "Extend Universal Scene with system " << Schema::EnumNameSystemType(systemType);

    BOOST_ASSERT_MSG(m_SystemScenes.find(systemType) == m_SystemScenes.end(),
              "The new scene to create for the selected system type already exists.");

    //
    // Have the system create it's scene.
    //
    ISystemScene* pScene = system.createScene();
    ASSERT(pScene != NULL);
    //
    // Create the associated task.
    pScene->createTask();
    //
    // Register all changes made by the scene.
    //
    m_pSceneCCM->Register(pScene, System::Changes::Generic::All, this);
    //
    // Add the scene to the collection.
    //
    m_SystemScenes[systemType] = pScene;

    return pScene;
}

/**
 * @inheritDoc
 */
Error UScene::Unextend(ISystemScene* pScene) {
    ASSERT(pScene != NULL);
    //
    // Get the system.
    //
    ISystem* pSystem = pScene->GetSystem<ISystem>();
    ASSERT(pSystem != NULL);
    //
    // Get the system's type.
    //
    Schema::SystemType SystemType = pSystem->GetSystemType();
    //
    // Find the system scene in the collection and remove it.
    //
    auto it = m_SystemScenes.find(SystemType);
    ASSERTMSG(it != m_SystemScenes.end(), "The scene to delete for its system type doesn't exist.");
    m_SystemScenes.erase(it);
    //
    // Unregister the scene from the CCM.
    //
    m_pSceneCCM->Unregister(pScene, this);
    //
    // Call the system to delete it's scene.
    //
    pSystem->DestroyScene(pScene);
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void UScene::addTemplates(const flatbuffers::Vector<flatbuffers::Offset<Schema::SceneEntity>>* templates) {
/*    for (auto template_ : objects) {
        Templates::iterator it = m_templates.find(template_->name());
        ASSERTMSG(it == m_templates.end(), "The template to add to the scene already exists.");
        if (it != m_templates.end()) {
            continue;
        }

        m_templates.insert(std::pair<flatbuffers::String, flatbuffers::Offset<Proto::Object>>(template_->name(), template_));
    }*/
}

/**
 * @inheritDoc
 */
UObject* UScene::createSceneEntity(const Schema::SceneEntity* sceneEntity) {
    IEntity* parent = nullptr;
    /*if (objectProto->parent()) {
        parent = FindObject(objectProto->parent()->c_str());
    }*/

    //
    // Create the new object.
    //
    auto id = sceneEntity->metaData()->entityId()->c_str();
    auto name = sceneEntity->metaData()->name()->c_str();
    UObject* pObject = new UObject(this, id, name, parent);
    ASSERT(pObject != NULL);
    //
    // Add the object to the collection.
    //
    m_Objects.push_back(pObject);
    if (parent != nullptr) {
        parent->addChildren(pObject);
    }

    SystemService* systemService = IServiceManager::get()->getSystemService();
    // TODO
    /*Templates::iterator templateIt = m_templates.find(std::string(objectProto->name()->c_str()));
    
    //
    // SystemObjects
    // 
    if (templateIt != m_templates.end()) {
        for (auto objectProto : *(*templateIt).second->systemObjects()) {
            createSystemObject(systemService, pObject, objectProto);
        }
    }*/
    for (auto component : *sceneEntity->systemComponents()) {
        createSystemObject(pObject, component);
    }

    /*
    // 
    // Properties
    // 
    if (templateIt != m_templates.end()) {
        for (auto objectProto : *(*templateIt).second->systemObjects()) {
            pObject->GetExtension(objectProto->systemType())->setProperties(objectProto->properties());
        }
    }
    for (auto objectProto : *objectProto->systemObjects()) {
        pObject->GetExtension(objectProto->systemType())->setProperties(objectProto->properties());
    }

    //
    // Init everything
    //
     */
    for (auto systemObject : pObject->GetExtensions()) {
        systemObject.second->initialize();
    }
    return pObject;
}

/**
 * @inheritDoc
 */
void UScene::createSystemObject(UObject* pObject, const Schema::SystemComponent* systemComponent) {
    UObject::SystemObjects extensions = pObject->GetExtensions();
    if (extensions.find(systemComponent->systemType()) != extensions.end()) {
        return;
    }

    const Schema::SystemType& type = systemComponent->systemType();
    ISystemScene* systemScene = m_SystemScenes.find(type)->second;
    ISystemObject* pSystemObject = pObject->Extend(systemScene, systemComponent);
    ASSERT(pSystemObject != NULL);
    m_pSceneCCM->Register(pSystemObject, System::Changes::Generic::All, this);
};

/**
 * @inheritDoc
 */
Error UScene::DestroyObject(UObject* pObject) {
    ASSERT(pObject != nullptr);
    // Delete all childrens
    for (auto children : pObject->getChildren()) {
        UObject* object = FindObject(children->getId());
        if (object != nullptr) {
            DestroyObject(object);
        }
    }
    // Delete object
    m_Objects.remove(pObject);
    delete pObject;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
UObject* UScene::FindObject(std::string id) {
    UObject* pObject = nullptr;

    for (auto object : m_Objects) {
        if (id == object->getId()) {
            pObject = object;
            break;
        }
    }

    return pObject;
}

/**
 * @inheritDoc
 */
void UScene::CreateObjectLink(ISystemObject* pSubject, ISystemObject* pObserver) {
    //
    // Register objects with the CCM.
    //
    System::Changes::BitMask Changes =
        pSubject->GetPotentialSystemChanges() & pObserver->GetDesiredSystemChanges();

    if (Changes) {
        m_pObjectCCM->Register(pSubject, Changes, pObserver);
        //
        // Hold on to the list for unregistering later.
        //
        ObjectLinkData old = { pSubject, pObserver };
        m_ObjectLinks.push_back(old);
        //
        // Inform the link requester that the link has been established.
        //
        pSubject->PostChanges(System::Changes::Link);
    }
}

/**
 * @inheritDoc
 */
void UScene::CreateObjectLink(UObject* pSubject, ISystemObject* pObserver) {
    //
    // Register objects with the CCM.
    //
    System::Changes::BitMask Changes =
        pSubject->GetPotentialSystemChanges() & pObserver->GetDesiredSystemChanges();

    if (Changes) {
        m_pObjectCCM->Register(pSubject, Changes, pObserver);
        //
        // Hold on to the list for unregistering later.
        //
        ObjectLinkData old = { pSubject, pObserver };
        m_ObjectLinks.push_back(old);
        //
        // Inform the link requester that the link has been established.
        //
        pSubject->PostChanges(System::Changes::ParentLink);
    }
}

/**
 * @inheritDoc
 */
Error UScene::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    // TODO
    /*switch (ChangeType) {
        case System::Changes::Generic::CreateObject: {
            ISceneObject* pScene = dynamic_cast<ISceneObject*>(pSubject);
            const ISceneObject::ObjectProtoQueue objectsToCreate = *pScene->getCreateObjects();
            for (auto objectProto : objectsToCreate) {
                ASSERT(FindObject(objectProto.id()) == NULL);
                //UObject* pObject = createObject(objectProto);
                //ASSERT(pObject != NULL);
            }
            pScene->resetCreateObjectQueues();
            break;
        }

        case System::Changes::Generic::DeleteObject: {
            ISceneObject* pScene = dynamic_cast<ISceneObject*>(pSubject);
            const ISceneObject::ObjectProtoQueue objectsToDestroy = *pScene->getDeleteObjects();
            for (auto objectProto : objectsToDestroy) {
                UObject* pObject = FindObject(objectProto->id()->c_str());
                ASSERT(pObject != NULL);
                DestroyObject(pObject);
            }
            pScene->resetDeleteObjectQueues();
            break;
        }
    }*/

    return Errors::Success;
}

