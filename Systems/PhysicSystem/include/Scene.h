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

#include <btBulletDynamicsCommon.h>

#include "DebugDrawer.h"
#include "System/ISystemScene.h"

class PhysicSystem;
class PhysicObject;
class ISystem;

class PhysicScene : public ISystemScene {
public:

    PhysicScene(ISystem* pSystem, const Schema::SystemScene* systemScene);

    ~PhysicScene();
        
    ///
    /// @inheritDoc.
    ///
    void Update(float DeltaTime) override;
        
    ///
    /// @inheritDoc.
    ///
    ISystemObject::Changes GetDesiredSystemChanges() override {
        return System::Changes::None;
    };

    void createTask() override;

    btDiscreteDynamicsWorld* getDynamicsWorld_() const {
        return dynamicsWorld_;
    }

protected:
    btSequentialImpulseConstraintSolver* constraintSolver_;
    btDiscreteDynamicsWorld* dynamicsWorld_;
    DebugDrawer* debugDrawer_;

};
