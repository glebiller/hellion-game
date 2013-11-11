// Copyright © 2008-2009 Intel Corporation
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

// NOTE: The Network system depends on DirectNetwork in order to function properly.

//
// This is the core file for the Network system.  It uses DirectNetwork ActionMapping to manage Network.  In DirectNetwork any controller
// attached to the PC is a "device" and any item on a device is an "object."  We use buffered Network which means that when we retrieve
// data from a device it returns all of the changes made since the last time data was fetched up to the size of the receiving buffer.
//

#include <boost/functional/factory.hpp>

#include "System.h"
#include "Scene.h"
#include "Task.h"
#include "Object/Object.h"
#include "Object/ConnectNetworkObject.h"
#include "Object/PlayerNetworkObject.h"
#include "Object/ReplicableNetworkObject.h"
#include "Object/UpdatableNetworkObject.h"
#include "Proto/Server/DownstreamMessage.pb.h"

/**
 * @inheritDoc
 */
NetworkScene::NetworkScene(ISystem* pSystem) 
    : ISystemScene(pSystem)
    , ISceneObject() {
    m_TaskFactory = boost::factory<NetworkTask*>();
    
    m_ObjectFactories["Connect"] = boost::factory<ConnectNetworkObject*>();
    m_ObjectFactories["Player"] = boost::factory<PlayerNetworkObject*>();
    m_ObjectFactories["Replicable"] = boost::factory<ReplicableNetworkObject*>();
    m_ObjectFactories["Updatable"] = boost::factory<UpdatableNetworkObject*>();
}

/**
 * @inheritDoc
 */
NetworkScene::~NetworkScene(void) {
}

/**
 * @inheritDoc
 */
Error NetworkScene::initialize(void) {
    ASSERT(!m_bInitialized);

    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void NetworkScene::Update(f32 DeltaTime) {
    for (auto object : m_pObjects) {
        NetworkObject* pObject = static_cast<NetworkObject*>(object.second);
        pObject->Update(DeltaTime);
    }
}

/**
 * @inheritDoc
 */
void NetworkScene::queueCreateObjects(Proto::RepeatedObject objectProtoList) {
    for (auto object : objectProtoList) {
        m_createObjectQueue->push_back(object);
    }
    PostChanges(System::Changes::Generic::CreateObject);
}

/**
 * @inheritDoc
 */
void NetworkScene::queueDeleteObjects(Proto::RepeatedObject objectProtoList) {
    for (auto object : objectProtoList) {
        m_deleteObjectQueue->push_back(object);
    }
    PostChanges(System::Changes::Generic::DeleteObject);
}

/**
 * @inheritDoc
 */
void NetworkScene::updateObjects(Proto::RepeatedObject objectProtoList) {
    for (auto object : objectProtoList) {
        auto systemObjectIterator = m_pObjects.find(object.id());
        // ignore updates if the object is not found
        if (systemObjectIterator != m_pObjects.end()) {
            systemObjectIterator->second->setProperties(object.systemobjects().Get(0).properties());
        }
    }
}