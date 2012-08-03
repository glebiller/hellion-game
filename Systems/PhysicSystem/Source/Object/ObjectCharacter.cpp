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


//
// extern includes
//
#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include "Common/Base/hkBase.h"
#include "Physics/Collide/Shape/hkpShape.h"
#include "Physics/Collide/Shape/Convex/Capsule/hkpCapsuleShape.h"
#include "Physics/Collide/Shape/Misc/Transform/hkpTransformshape.h"
#include "Physics/Dynamics/World/hkpWorld.h"
#include "Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h"
#include "Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h"
#include "Physics/Utilities/CharacterControl/StateMachine/hkpDefaultCharacterStates.h"
#include "Physics/Collide/Query/Collector/PointCollector/hkpClosestCdPointCollector.h"
#include "Physics/Utilities/Destruction/BreakOffParts/hkpBreakOffPartsUtil.h"
#pragma warning( pop )

//
// core includes
//
#include <BaseTypes.h>
#include <Interface.h>

//
// system includes
//
#include "Scene.h"
#include "Object/Object.h"
#include "Object/ObjectCharacter.h"

//
// constants
//
#define PHAVOKSCENE         reinterpret_cast<HavokPhysicsScene*>(m_pSystemScene)

pcstr HavokCharacterObject::sm_kapszCommonPropertyNames[] = {
    "CapsuleA", "CapsuleB", "Radius",
};

const Properties::Property HavokCharacterObject::sm_kaCommonDefaultProperties[] = {
    Properties::Property(sm_kapszCommonPropertyNames[ Property_CapsuleA ],
    Properties::Values::Vector3,
    Properties::Flags::Valid, 0.0f),

    Properties::Property(sm_kapszCommonPropertyNames[ Property_CapsuleB ],
    Properties::Values::Vector3,
    Properties::Flags::Valid, 0.0f),

    Properties::Property(sm_kapszCommonPropertyNames[ Property_Radius ],
    Properties::Values::Float32,
    Properties::Flags::Valid, 0.0f),
};

COMPILE_ASSERT(HavokCharacterObject::Property_Count == sizeof HavokCharacterObject::sm_kapszCommonPropertyNames / sizeof HavokCharacterObject::sm_kapszCommonPropertyNames[ 0 ]);

///////////////////////////////////////////////////////////////////////////////
// HavokCharacterObject - Default constructor
HavokCharacterObject::HavokCharacterObject(
    ISystemScene* pSystemScene,
    pcstr pszName
)
    : HavokObject(pSystemScene, pszName)
    , m_CharacterProxy(NULL)
    , m_characterContext(NULL)
    , m_CharacterOrientation(hkQuaternion(1, 0, 0, 0))
    , m_Velocity(Math::Vector3::Zero)
    , m_CapsuleA(Math::Vector3::Zero)
    , m_CapsuleB(Math::Vector3::Zero)
    , m_Radius(0.0f) {
}


///////////////////////////////////////////////////////////////////////////////
// ~HavokCharacterObject - Default destructor
HavokCharacterObject::~HavokCharacterObject(
    void
) {
    // Free resources
    if (m_CharacterProxy) {
        hkpWorld* pWorld = PHAVOKSCENE->GetWorld();
        // Free Havok resources for m_CharacterProxy
        pWorld->lock();
        pWorld->removePhantom(m_CharacterProxy->getShapePhantom());
        m_CharacterProxy->removeReference();
        pWorld->unlock();
    }
}


///////////////////////////////////////////////////////////////////////////////
// Initialize - Initializes this object with the given properties
Error
HavokCharacterObject::Initialize(
    std::vector<Properties::Property> Properties
) {
    ASSERT(!m_bInitialized);

    //
    // Read in the properties.
    //
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();

            if (sName == sm_kapszCommonPropertyNames[ Property_CapsuleA ]) {
                m_CapsuleA = it->GetVector3();
                it->ClearFlag(Properties::Flags::Valid);
            } else if (sName == sm_kapszCommonPropertyNames[ Property_CapsuleB ]) {
                m_CapsuleB = it->GetVector3();
                it->ClearFlag(Properties::Flags::Valid);
            } else if (sName == sm_kapszCommonPropertyNames[ Property_Radius ]) {
                m_Radius = it->GetFloat32(0);
                it->ClearFlag(Properties::Flags::Valid);
            }
        }
    }

    //
    // Get the world.
    //
    hkpWorld* pWorld = PHAVOKSCENE->GetWorld();
    ASSERT(pWorld != NULL);
    pWorld->lock();
    //
    // Create a capsule to represent the character
    //
    hkVector4 VertexA(m_CapsuleA.x, m_CapsuleA.y, m_CapsuleA.z);
    hkVector4 VertexB(m_CapsuleB.x, m_CapsuleB.y, m_CapsuleB.z);
    hkpShape* standShape = new hkpCapsuleShape(VertexA, VertexB, m_Radius);
    VertexA.setZero4();
    hkpShape* crouchShape = new hkpCapsuleShape(VertexA, VertexB, m_Radius);
    //
    // Construct a Shape Phantom
    //
    hkpShapePhantom* Phantom = new hkpSimpleShapePhantom(standShape, hkTransform::getIdentity());
    //
    // Add the phantom to the world
    //
    pWorld->addPhantom(Phantom);
    Phantom->removeReference();
    //
    // Construct a character proxy
    //
    hkpCharacterProxyCinfo Cinfo;
    Cinfo.m_position.set(m_Position.x, m_Position.y, m_Position.z);
    Cinfo.m_staticFriction = 0.0f;
    Cinfo.m_dynamicFriction = 1.0f;
    Cinfo.m_up.setNeg4(pWorld->getGravity());
    Cinfo.m_up.normalize3();
    Cinfo.m_maxSlope = HK_REAL_PI / 3.0f;
    Cinfo.m_shapePhantom = Phantom;
    m_CharacterProxy = new hkpCharacterProxy(Cinfo);
    //
    // Create the Character state machine and context
    //
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
    //
    // Set this as initialized.
    //
    m_bInitialized = True;
    //
    // Set the properties for this object.
    //
    SetProperties(Properties);
    return Errors::Success;
}


///////////////////////////////////////////////////////////////////////////////
// GetProperties - Get the properties for this Object
void
HavokCharacterObject::GetProperties(
    Properties::Array& Properties
) {
    //
    // Get the index of our first item.
    //
    i32 iProperty = static_cast<i32>(Properties.size());
    //
    // Add the common properties.
    //
    Properties.reserve(Properties.size() + Property_Count);

    for (i32 i = 0; i < Property_Count; i++) {
        Properties.push_back(sm_kaCommonDefaultProperties[ i ]);
    }
}


///////////////////////////////////////////////////////////////////////////////
// Properties - Set the properties for this Object
void
HavokCharacterObject::SetProperties(
    Properties::Array Properties
) {
    ASSERT(m_bInitialized);
}


///////////////////////////////////////////////////////////////////////////////
// GetDesiredSystemChanges - Get system changes this Object is interested in
System::Types::BitMask
HavokCharacterObject::GetDesiredSystemChanges(
    void
) {
    return (System::Changes::Geometry::Position
            | System::Changes::Geometry::Orientation
            | System::Changes::Input::Velocity);
}


///////////////////////////////////////////////////////////////////////////////
// Update - Update this Object (should be called every frame)
void
HavokCharacterObject::Update(
    f32 DeltaTime
) {
    //
    // Get the world.
    //
    hkpWorld* pWorld = PHAVOKSCENE->GetWorld();
    ASSERT(pWorld != NULL);
    hkVector4 up = HavokPhysicsScene::GetDefaultUp();
    hkVector4 down;
    down.setNeg4(up);
    hkpCharacterInput input;
    hkpCharacterOutput output;
    {
        input.m_inputLR = m_Velocity.x;
        input.m_inputUD = m_Velocity.z;
        input.m_wantJump = m_Velocity.y != 0;
        input.m_atLadder = False;
        input.m_up = up;
        input.m_forward.set(1, 0, 0);
        input.m_forward.setRotatedDir(m_CharacterOrientation, input.m_forward);
        input.m_stepInfo.m_deltaTime = DeltaTime;
        input.m_stepInfo.m_invDeltaTime = 1.0f / DeltaTime;
        input.m_characterGravity.set(0, -16, 0);
        input.m_velocity = m_CharacterProxy->getLinearVelocity();
        input.m_position = m_CharacterProxy->getPosition();
        m_CharacterProxy->checkSupport(down, input.m_surfaceInfo);
    }
    // Apply the character state machine
    {
        HK_TIMER_BEGIN("update character state", HK_NULL);
        m_characterContext->update(input, output);
        HK_TIMER_END();
    }
    //
    // Set the new linear velocity
    //
    m_CharacterProxy->setLinearVelocity(output.m_velocity);
    //
    // Integrate (update) character proxy
    //
    hkStepInfo StepInfo(hkTime(0.0f), hkTime(DeltaTime));
    m_CharacterProxy->integrate(StepInfo, pWorld->getGravity());
    //
    // Update the position
    //
    m_CharacterProxy->getPosition().store3(m_Position);
    PostChanges(System::Changes::Physics::Position);
}


///////////////////////////////////////////////////////////////////////////////
// ChangeOccurred - Give this Object a change to process this system change
Error
HavokCharacterObject::ChangeOccurred(
    ISubject* pSubject,
    System::Changes::BitMask ChangeType
) {
    ASSERT(m_bInitialized);
    //
    // Get the world.
    //
    hkpWorld* pWorld = PHAVOKSCENE->GetWorld();
    ASSERT(pWorld != NULL);
    pWorld->markForWrite();

    // Update this objects position
    if (ChangeType & System::Changes::Geometry::Position) {
        m_Position = *dynamic_cast<IGeometryObject*>(pSubject)->GetPosition();

        if (m_CharacterProxy != NULL) {
            // Modify the phantom's position.
            hkVector4 Position(m_Position.x, m_Position.y, m_Position.z);
            m_CharacterProxy->setPosition(Position);
        }
    }

    // Update this objects orientation
    if (ChangeType & System::Changes::Geometry::Orientation) {
        m_Orientation = *dynamic_cast<IGeometryObject*>(pSubject)->GetOrientation();

        if (m_CharacterProxy != NULL) {
            // Modify the phantom's orientation.
            m_CharacterOrientation.set(m_Orientation.x, m_Orientation.y, m_Orientation.z, m_Orientation.w);
        }
    }

    // Update this objects velocity
    if (ChangeType & System::Changes::Input::Velocity) {
        m_Velocity = *dynamic_cast<IMoveObject*>(pSubject)->GetVelocity();
    }

    pWorld->unmarkForWrite();
    return Errors::Success;
}


System::Changes::BitMask
HavokCharacterObject::GetPotentialSystemChanges(
    void
) {
    return System::Changes::Physics::Position;
}


