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

#include "Object/CharacterPhysicObject.h"

#include <boost/bind.hpp>

#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include <Common/Base/hkBase.h>
#include <Physics2012/Collide/Shape/hkpShape.h>
#include <Physics2012/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h>
#include <Physics2012/Collide/Shape/Misc/Transform/hkpTransformshape.h>
#include <Physics2012/Dynamics/World/hkpWorld.h>
#include <Physics2012/Dynamics/Phantom/hkpSimpleShapePhantom.h>
#include <Physics2012/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Physics2012/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h>
#include <Physics2012/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h>
#include <Physics2012/Utilities/Destruction/BreakOffParts/hkpBreakOffPartsUtil.h>
#pragma warning( pop )

#include "Scene.h"
#include "Task.h"
#include "Shape/IgnorePhantomOverlapListener.h"

///
/// @inheritDoc
///
CharacterPhysicObject::CharacterPhysicObject(ISystemScene* pSystemScene, UObject* entity)
    : PhysicObject(pSystemScene, entity)
    , m_CharacterProxy(nullptr)
    , m_characterContext(nullptr)
    , m_CapsuleA(Math::Vector3::Zero)
    , m_CapsuleB(Math::Vector3::Zero)
    , m_Radius(0.0f) {
    m_propertySetters["CapsuleA"] = boost::bind(&IProperty::setVector3, this, System::Changes::None, &m_CapsuleA, _1);
    m_propertyGetters["CapsuleA"] = boost::bind(&IProperty::getVector3, this, &m_CapsuleA, _1);
    m_propertySetters["CapsuleB"] = boost::bind(&IProperty::setVector3, this, System::Changes::None, &m_CapsuleB, _1);
    m_propertyGetters["CapsuleB"] = boost::bind(&IProperty::getVector3, this, &m_CapsuleB, _1);
    m_propertySetters["Radius"] = boost::bind(&IProperty::setSimpleType<f32>, this, System::Changes::None, &m_Radius, _1);
    m_propertyGetters["Radius"] = boost::bind(&IProperty::getSimpleType<f32>, this, &m_Radius, _1);
}

///
/// @inheritDoc
///
CharacterPhysicObject::~CharacterPhysicObject() {
    // Free resources
    if (m_bInitialized) {
        PhysicTask* task = GetSystemScene<PhysicScene>()->GetSystemTask<PhysicTask>();
        task->getActiveObjects()->remove(this);

        hkpWorld* pWorld = GetSystemScene<PhysicScene>()->getWorld();
        // Free Havok resources for m_CharacterProxy
        pWorld->lock();
        pWorld->removePhantom(m_CharacterProxy->getShapePhantom());
        m_CharacterProxy->removeReference();
        pWorld->unlock();
    }
}

///
/// @inheritDoc
///
Error CharacterPhysicObject::initialize() {


        hkpWorld* pWorld = GetSystemScene<PhysicScene>()->getWorld();
        BOOST_ASSERT(pWorld != NULL);
        pWorld->lock();

        hkVector4 VertexA(m_CapsuleA.x, m_CapsuleA.y, m_CapsuleA.z);
        hkVector4 VertexB(m_CapsuleB.x, m_CapsuleB.y, m_CapsuleB.z);
        hkpShape* standShape = new hkpCapsuleShape(VertexA, VertexB, m_Radius);
        VertexA.setZero4();
        hkpShape* crouchShape = new hkpCapsuleShape(VertexA, VertexB, m_Radius);
        hkpShapePhantom* phantom = new hkpSimpleShapePhantom(standShape, hkTransform::getIdentity());
        phantom->setName(m_entity->getName().c_str());
        phantom->addPhantomOverlapListener(new IgnorePhantomOverlapListener());
        pWorld->addPhantom(phantom);
        phantom->removeReference();

        hkpCharacterProxyCinfo Cinfo;
        Cinfo.m_position.set(m_position.x, m_position.y, m_position.z);
        Cinfo.m_staticFriction = 0.0f;
        Cinfo.m_dynamicFriction = 1.0f;
        Cinfo.m_up.setNeg4(pWorld->getGravity());
        Cinfo.m_up.normalize3();
        Cinfo.m_keepDistance = 0.1f;
        Cinfo.m_userPlanes = 4;
        Cinfo.m_shapePhantom = phantom;
        m_CharacterProxy = new hkpCharacterProxy(Cinfo);

        {
            hkpCharacterState* state;
            hkpCharacterStateManager* manager = new hkpCharacterStateManager();
            state = new hkpCharacterStateOnGround();
            manager->registerState(state,   HK_CHARACTER_ON_GROUND);
            state->removeReference();
            state = new hkpCharacterStateInAir();
            manager->registerState(state,   HK_CHARACTER_IN_AIR);
            state->removeReference();
            state = new hkpCharacterStateJumping();
            manager->registerState(state,   HK_CHARACTER_JUMPING);
            state->removeReference();
            m_characterContext = new hkpCharacterContext(manager, HK_CHARACTER_ON_GROUND);
            manager->removeReference();
        }

        pWorld->unlock();
        
        PhysicTask* task = GetSystemScene<PhysicScene>()->GetSystemTask<PhysicTask>();
        task->getActiveObjects()->push_back(this);

        m_bInitialized = true;
        return Errors::Success;
}

///
/// @inheritDoc
///
Error CharacterPhysicObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {


    if (ChangeType & System::Changes::Input::Velocity) {
        m_velocity = *dynamic_cast<IMoveObject*>(pSubject)->getVelocity();
    }
    if (ChangeType & System::Changes::Input::Rotation) {
        m_rotation = *dynamic_cast<IMoveObject*>(pSubject)->getRotation();
    }

    return Errors::Success;
}

///
/// @inheritDoc
///
void CharacterPhysicObject::Update(f32 DeltaTime) {
    if (m_rotation != Math::Vector3::Zero) {
        Math::Quaternion rotation = Math::Quaternion();
        rotation.Set(Math::Vector3::UnitY, m_rotation.y * DeltaTime * 5);
        m_orientation *= rotation;
    }

    hkpWorld* pWorld = GetSystemScene<PhysicScene>()->getWorld();
    BOOST_ASSERT(pWorld != NULL);

    pWorld->markForWrite();
    
    hkpCharacterInput input;
    hkpCharacterOutput output;
    {
        input.m_inputLR = m_velocity.z;
        input.m_inputUD = m_velocity.x;
        input.m_wantJump = false;
        input.m_atLadder = false;
        input.m_up = s_up;
        input.m_forward = s_forward;
        input.m_forward.setRotatedDir(hkQuaternion(m_orientation.x, m_orientation.y, m_orientation.z, m_orientation.w), input.m_forward);
        input.m_stepInfo.m_deltaTime = DeltaTime;
        input.m_stepInfo.m_invDeltaTime = 1.0f / DeltaTime;
        input.m_characterGravity = s_gravity;
        input.m_velocity = m_CharacterProxy->getLinearVelocity();
        input.m_position = m_CharacterProxy->getPosition();
        m_CharacterProxy->checkSupport(s_down, input.m_surfaceInfo);
    }
    {
        HK_TIMER_BEGIN("update character state", HK_NULL);
        m_characterContext->update(input, output);
        HK_TIMER_END();
    }

    m_CharacterProxy->setLinearVelocity(output.m_velocity);
    m_CharacterProxy->integrate(input.m_stepInfo, pWorld->getGravity());
    m_CharacterProxy->getPosition().store3(m_position);
    
    pWorld->unmarkForWrite();

    PostChanges(System::Changes::Physic::Position | System::Changes::Physic::Orientation);
}