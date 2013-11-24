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

#include <Common\Base\hkBase.h>

#include "System/ISystemScene.h"
#include "System/ISystemObject.h"
#include "Object/IGeometryObject.h"
#include "Generic/IEntity.h"

class HavokPhysicsSystem;
class HavokPhysicsScene;
class HavokPhysicsTask;
class hkpRigidBody;

///
/// <c>HavokObject</c> Implementation of the ISystemObject interface. This is the base object
/// created by the HavokPhysics Scene.
///
/// @sa ISystemObject
/// @sa IGeometryObject
///
class PhysicObject : public ISystemObject, public IGeometryObject {
public:
    ///
    /// @inheritDoc.
    ///
    PhysicObject(ISystemScene* pSystemScene, IEntity* entity);

    ///
    /// @inheritDoc.
    ///
    virtual ~PhysicObject(void);

    ///
    /// @inheritDoc.
    ///
    inline Proto::SystemType GetSystemType(void) {
        return Proto::SystemType::Physic;
    };

    ///
    /// Query if this object is static.
    ///
    /// @return true if static, false if not.
    ///
    inline bool isStatic() {
        return m_bStatic;
    };

    ///
    /// Gets the body.
    ///
    /// @return null if it fails, else the body.
    ///
    inline hkpRigidBody* getBody() {
        return m_pBody;
    };

protected:
    static hkVector4    s_up;
    static hkVector4    s_down;
    static hkVector4    s_forward;
    static hkVector4    s_gravity;
    hkpRigidBody*       m_pBody;
    bool                m_bStatic;

};
