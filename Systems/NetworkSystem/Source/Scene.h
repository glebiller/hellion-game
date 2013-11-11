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

#pragma once

#include "Object/ISceneObject.h"
#include "System/ISystemScene.h"
#include "System/ISystem.h"
#include "System.h"
#include "Task.h"

class NetworkSystem;
class NetworkObject;

/**
* Implementation of the ISystemScene interface. See Interfaces\System.h for a definition of the
* class and its functions.
*
* @sa  ISystemScene
*/
class NetworkScene : public ISystemScene, public ISceneObject {
public:

    /**
     * @inheritDoc
     */
    NetworkScene(ISystem* pSystem);

    /**
     * @inheritDoc
     */
    ~NetworkScene(void);

    /**
     * @inheritDoc
     */
    Error initialize(void);

    /**
     * @inheritDoc
     */
    void Update(f32 DeltaTime);

    /**
     * @inheritDoc
     */
    System::Changes::BitMask GetPotentialSystemChanges(void) {
        return System::Changes::Generic::All;
    };

    /**
     * @inheritDoc
     */
    System::Changes::BitMask GetDesiredSystemChanges(void) {
        return System::Changes::None;
    };

    /**
     * @inheritDoc
     */
    Proto::SystemType GetSystemType(void) {
        return Proto::SystemType::Network;
    };

    /**
     * Queue create objects.
     *
     * @param   objectProtoList List of object prototypes.
     */
    void queueCreateObjects(Proto::RepeatedObject objectProtoList);

    /**
     * Queue delete objects.
     *
     * @param   objectProtoList List of object prototypes.
     */
    void queueDeleteObjects(Proto::RepeatedObject objectProtoList);
    
    /**
     * Updates the objects described by objectProtoList.
     *
     * @param   objectProtoList List of object prototypes.
     */
    void updateObjects(Proto::RepeatedObject objectProtoList);

};
