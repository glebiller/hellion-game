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

#include "BaseTypes.h"
#include "Interface.h"

#include "System.h"
#include "Scene.h"
#include "Object/Object.h"
#include "Task.h"


extern ManagerInterfaces   g_Managers;

/**
 * @inheritDoc
 */
NetworkScene::NetworkScene(ISystem* pSystem) : ISystemScene(pSystem)
    , m_pTask(NULL) {

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
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void NetworkScene::Update(f32 DeltaTime) {
    ObjectsList Objects = m_pObjects;

    //
    // Cycle through all of our objects and apply the changes.
    // Also post our change notifications to the CCM.
    //
    for (ObjectsList::iterator it = Objects.begin(); it != Objects.end(); it++) {
        NetworkObject* pObject = static_cast<NetworkObject*>(*it);
        pObject->Update(DeltaTime);
    }
}
