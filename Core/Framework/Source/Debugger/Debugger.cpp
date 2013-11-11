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

#include "Debugger.h"

#ifdef DEBUG_BUILD

#include "Proto/Debug/DebugEntity.pb.h"

#include "Manager/IChangeManager.h"
#include "System/ISystemObject.h"
#include "System/Components.h"
#include "Universal/UScene.h"
#include "Universal/UObject.h"

void protobuf_uint8_free(void* data, void* hint) {
    delete[] data;
}

/**
 * @inheritDoc
 */
Debugger::Debugger(void) 
    : m_bDebuggerActive(false) {
}

/**
 * @inheritDoc
 */
Debugger::~Debugger(void) {
    if (!m_bDebuggerActive) {
        return;
    }

    delete m_pSocket;
    delete m_pObjectChangesDebugger;
}

/**
 * @inheritDoc
 */
void Debugger::initialize(bool debuggerActive) {
    return;
    if (!debuggerActive) {
        return;
    }

    m_bDebuggerActive = debuggerActive;
    m_pSceneChangesDebugger = new SceneChangesDebugger(this);
    m_pObjectChangesDebugger = new ObjectChangesDebugger(this);

    m_pContext = new zmq::context_t(1);
    m_pSocket = new zmq::socket_t(*m_pContext, ZMQ_PUSH);
    m_pSocket->connect("tcp://kissy.synology.me:26901");
}

/**
 * @inheritDoc
 */
void Debugger::setChangeManagers(IChangeManager* pSceneCCM, IChangeManager* pObjectCCM) {
    m_pSceneCCM = pSceneCCM;
    m_pObjectCCM = pObjectCCM;
}

/**
 * @inheritDoc
 */
void Debugger::setScene(const UScene* pUScene) {
    m_pUScene = const_cast<UScene*>(pUScene);

    if (!m_bDebuggerActive) {
        return;
    }

    DebugProto debugProto;
    UScene::SystemScenes Scenes = m_pUScene->GetSystemScenes();
    for (auto it = Scenes.begin(); it != Scenes.end(); it++) {
        ISystemScene* pScene = it->second;
        DebugEntityProto* debugEntityProto = debugProto.add_entities();
        debugEntityProto->set_id(Proto::SystemType_Name(pScene->GetSystemType()));
        debugEntityProto->set_name(Proto::SystemType_Name(pScene->GetSystemType()));
        debugEntityProto->set_category(System::getComponentName(System::Components::Scene));
        m_pSceneCCM->Register(pScene, System::Changes::All, m_pSceneChangesDebugger);
    }

    UScene::Objects Objects = m_pUScene->GetObjects();
    for (auto it = Objects.begin(); it != Objects.end(); it++) {
        debugObject(*it, debugProto);
    }

    send(&debugProto);
}

/**
 * @inheritDoc
 */
void Debugger::clean(void) {
    if (!m_bDebuggerActive) {
        return;
    }

    UScene::SystemScenes Scenes = m_pUScene->GetSystemScenes();
    for (auto it = Scenes.begin(); it != Scenes.end(); it++) {
        ISystemScene* pScene = it->second;
        m_pSceneCCM->Unregister(pScene, m_pSceneChangesDebugger);
    }

    UScene::Objects Objects = m_pUScene->GetObjects();
    for (auto it = Objects.begin(); it != Objects.end(); it++) {
        UObject* pUObject = *it;
        UObject::SystemObjects SystemObjects = pUObject->GetExtensions();
        for (auto it = SystemObjects.begin(); it != SystemObjects.end(); it++) {
            ISystemObject* pObject = it->second;
            m_pObjectCCM->Unregister(pObject, m_pObjectChangesDebugger);
        }
    }
}

/**
 * @inheritDoc
 */
void Debugger::update(f32 deltaTime) {
    if (!m_bDebuggerActive) {
        return;
    }
    
    DebugProto debugProto;

    for (auto createdObjectId : m_createdObjectIds) {
        UObject* pUObject = m_pUScene->FindObject(createdObjectId.c_str());
        if (pUObject != NULL) {
            debugObject(pUObject, debugProto);
        }
    }
    m_createdObjectIds.clear();
    
    for (auto updatedObject : m_updatedObjects) {
        debugObject(dynamic_cast<ISystemObject*>(updatedObject), debugProto);
    }
    m_updatedObjects.clear();

    send(&debugProto);
}

/**
 * @inheritDoc
 */
void Debugger::send(DebugProto* debugProto) {
    int size = debugProto->ByteSize(); 
    google::protobuf::uint8* buffer = new google::protobuf::uint8[size];
    debugProto->SerializeWithCachedSizesToArray(buffer);
    zmq::message_t* message = new zmq::message_t(buffer, size, protobuf_uint8_free);
    m_pSocket->send(*message);
}

/**
 * @inheritDoc
 */
void Debugger::addCreatedObjectIds(std::string objectId) {
    m_createdObjectIds.push_back(objectId);
}

/**
 * @inheritDoc
 */
void Debugger::addUpdatedObject(ISubject* object) {
    m_updatedObjects.insert(object);
}

/**
 * @inheritDoc
 */
void Debugger::debugObject(UObject* object, DebugProto& debugProto) {
    DebugEntityProto* debugEntityProto = debugProto.add_entities();
    debugEntityProto->set_id(object->getName());
    debugEntityProto->set_name(object->getName());
    debugEntityProto->set_category(System::getComponentName(System::Components::Object));
    UObject::SystemObjects SystemObjects = object->GetExtensions();
    for (auto it = SystemObjects.begin(); it != SystemObjects.end(); it++) {
        ISystemObject* systemObject = it->second;
        DebugPropertyProto* debugPropertyProto = debugEntityProto->add_properties();
        debugPropertyProto->set_category(Proto::SystemType_Name(systemObject->GetSystemType()));
        const auto properties = systemObject->getProperties();
        //debugPropertyProto->mutable_properties()->CopyFrom(properties);
        m_pObjectCCM->Register(systemObject, System::Changes::All, m_pObjectChangesDebugger);
    }
}

/**
 * @inheritDoc
 */
void Debugger::debugObject(ISystemObject* object, DebugProto& debugProto) {
    DebugEntityProto* debugEntityProto = debugProto.add_entities();
    debugEntityProto->set_id(object->getEntity()->getId());
    debugEntityProto->set_name(object->getEntity()->getName());
    debugEntityProto->set_category(System::getComponentName(System::Components::Object));

    DebugPropertyProto* debugPropertyProto = debugEntityProto->add_properties();
    debugPropertyProto->set_category(Proto::SystemType_Name(object->GetSystemType()));
    
    const auto properties = object->getProperties();
    //debugPropertyProto->mutable_properties()->CopyFrom(properties);
}

#endif