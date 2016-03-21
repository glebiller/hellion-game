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

#pragma once

#include <OISB.h>

#include "Errors.h"
#include "System/ISystem.h"
#include "System/ISystemScene.h"

class InputTask;
class InputObject;
class InputAction;

/**
 * Implementation of the ISystemScene interface. See Interfaces\System.h for a definition of the
 * class and its functions.
 *
 * @sa  ISystemScene
 */
class InputScene : public ISystemScene {
public:

    /**
     * @inheritDoc
     */
    InputScene(ISystem* pSystem, const Schema::SystemScene* systemScene);

    /**
     * @inheritDoc
     */
    ~InputScene();

    /**
     * @inheritDoc
     */
    void Update(float DeltaTime) override;

    /**
     * @inheritDoc
     */
    System::Changes::BitMask GetDesiredSystemChanges() override {
        return System::Changes::None;
    };

    /**
     * Access the DefaultSchema.
     *
     * @return  null if it fails, else the default schema.
     */
    OISB::ActionSchema* getDefaultSchema() const {
        return m_defaultSchema;
    };

    virtual void createTask() override;

private:
    OISB::ActionSchema*             m_defaultSchema;
    OISB::TriggerAction*            m_quitInputAction;

};
