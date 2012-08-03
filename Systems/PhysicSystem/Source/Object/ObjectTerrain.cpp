//
// extern includes
//
#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6385 6323 6386 )
#include "Common/Base/hkBase.h"
#include "Common/SceneData/Graph/hkxNode.h"
#include "Common/SceneData/Scene/hkxScene.h"
#include "Common/Serialize/Util/hkLoader.h"
#include "Common/Serialize/Util/hkRootLevelContainer.h"

#include "Physics/Dynamics/Entity/hkpRigidBody.h"
#include "Physics/Dynamics/Entity/hkpEntityListener.h"
#include "Physics/Dynamics/Entity/hkpEntityActivationListener.h"
#include "Physics/Dynamics/Phantom/hkpSimpleShapePhantom.h"
#include "Physics/Dynamics/World/hkpWorld.h"
#include "Physics/Dynamics/Collide/Deprecated/hkpCollisionListener.h"
#include "Physics/Collide/Dispatch/hkpAgentRegisterUtil.h"
#include "Physics/Collide/Filter/Group/hkpGroupFilterSetup.h"
#include "Physics/Collide/Shape/Compound/Collection/ExtendedMeshShape/hkpExtendedMeshShape.h"
#include "Physics/Collide/Shape/Compound/Collection/List/hkpListShape.h"
#include "Physics/Collide/Shape/Compound/Collection/SimpleMesh/hkpSimpleMeshShape.h"
#include "Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppBvTreeShape.h"
#include "Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppCompilerInput.h"
#include "Physics/Collide/Shape/Compound/Tree/Mopp/hkpMoppUtility.h"
#include "Physics/Collide/Shape/Convex/Box/hkpBoxShape.h"
#include "Physics/Collide/Shape/Convex/ConvexVertices/hkpConvexVerticesShape.h"
#include "Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldBaseCinfo.h"
#include "Physics/Utilities/Dynamics/Inertia/hkpInertiaTensorComputer.h"
#include "Physics/Utilities/Serialize/hkpPhysicsData.h"

#ifdef __HAVOK_VDB__
#include <Physics/Utilities/VisualDebugger/hkpPhysicsContext.h>
#endif

#pragma warning( pop )

#include "OgreImage.h"
#include "OgreColourValue.h"
#include "OgreResourceGroupManager.h"

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
#include "Object/ObjectTerrain.h"
#include "Shape/TerrainShape.h"

//
// constants
//
#define PHAVOKSCENE         reinterpret_cast<HavokPhysicsScene*>(m_pSystemScene)

pcstr PhysicObjectTerrain::sm_kapszCommonPropertyNames[] = {
    "Size",
};

const Properties::Property PhysicObjectTerrain::sm_kaCommonDefaultProperties[] = {
    Properties::Property(sm_kapszCommonPropertyNames[ Property_Size ],
    VALUE2(Properties::Values::Int32, Properties::Values::Int32),
    Properties::Flags::Valid | Properties::Flags::InitOnly,
    0, 0),

    /*Properties::Property( sm_kapszCommonPropertyNames[ Property_Gravity ],
                          Properties::Values::Vector3,
                          Properties::Flags::Valid,
                          sm_kDefaultGravity ),

    Properties::Property( sm_kapszCommonPropertyNames[ Property_Material ],
                          Properties::Values::String,
                          Properties::Flags::Valid | Properties::Flags::Multiple,
                          "" ),

    Properties::Property( sm_kapszCommonPropertyNames[ Property_Elasticity ],
                          VALUE3( Properties::Values::String, Properties::Values::String,
                                  Properties::Values::Float32 ),
                          Properties::Flags::Valid | Properties::Flags::Multiple,
                          "", "", 0.0f ),

    Properties::Property( sm_kapszCommonPropertyNames[ Property_Friction ],
                          VALUE4( Properties::Values::String, Properties::Values::String,
                                  Properties::Values::Float32, Properties::Values::Float32 ),
                          Properties::Flags::Valid | Properties::Flags::Multiple,
                          "", "", 0.0f, 0.0f ),

    Properties::Property( sm_kapszCommonPropertyNames[ Property_Softness ],
                          VALUE3( Properties::Values::String, Properties::Values::String,
                                  Properties::Values::Float32 ),
                          Properties::Flags::Valid | Properties::Flags::Multiple,
                          "", "", 0.0f ),*/
};

PhysicObjectTerrain::PhysicObjectTerrain(
    ISystemScene* pSystemScene,
    pcstr pszName
)
    : HavokObject(pSystemScene, pszName) {
    ASSERT(Property_Count == sizeof sm_kapszCommonPropertyNames / sizeof sm_kapszCommonPropertyNames[ 0 ]);
    ASSERT(Property_Count == sizeof sm_kaCommonDefaultProperties / sizeof sm_kaCommonDefaultProperties[ 0 ]);
}

PhysicObjectTerrain::~PhysicObjectTerrain(
    void
) {
}

Error
PhysicObjectTerrain::Initialize(
    std::vector<Properties::Property> Properties
) {
    ASSERT(!m_bInitialized);

    //
    // Read the properties.
    //
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();

            if (sName == sm_kapszCommonPropertyNames[ Property_Size ]) {
                //it->GetString( 0 )
                it->ClearFlag(Properties::Flags::Valid);
            }
        }
    }

    //
    // Generate terrain
    //
    Ogre::Image img;
    img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    const int xRes = 257;
    const int zRes = 257;
    hkUint16* pHeightData = hkAllocate<hkUint16>(xRes * zRes, HK_MEMORY_CLASS_DEMO);
    {
        for (int x = 0; x < xRes; x++) {
            for (int z = 0; z < zRes; z++) {
                float height = img.getColourAt(x, z, 0).r;
                // Convert to 16 bit
                pHeightData[x * zRes + z] = static_cast<hkUint16>(hkUint16(-1) * height);
            }
        }
    }
    //
    // Create the shape
    //
    hkpSampledHeightFieldBaseCinfo ci;
    ci.m_xRes = xRes;
    ci.m_zRes = zRes;
    ci.m_maxHeight = 100.0f;
    ci.m_minHeight = 0.0f;
    ci.m_scale = hkVector4(4.0f, 1.0f, 4.0f, 0.0f);
    //
    // Start editing the world.
    //
    PHAVOKSCENE->GetWorld()->markForWrite();
    //
    // now to make a rigid body out of our height field
    //
    {
        TerrainShape* pTerrainShape = new TerrainShape(ci, pHeightData);
        hkpRigidBodyCinfo bodyInfo;
        bodyInfo.m_motionType = hkpMotion::MOTION_FIXED;
        bodyInfo.m_position = hkVector4(- 2.0f * (xRes - 1.0f), 0.0f, - 2.0f * (zRes - 1.0f));
        bodyInfo.m_shape = pTerrainShape;
        bodyInfo.m_friction = 0.5f;
        hkpRigidBody* body = new hkpRigidBody(bodyInfo);
        PHAVOKSCENE->GetWorld()->addEntity(body);
        body->removeReference();
        pTerrainShape->removeReference();
    }
    //
    // Now we have finished modifying the world, release our write marker.
    //
    PHAVOKSCENE->GetWorld()->unmarkForWrite();
    return Errors::Success;
}

void
PhysicObjectTerrain::SetProperties(
    Properties::Array Properties
) {
    ASSERT(m_bInitialized);
}

System::Types::BitMask
PhysicObjectTerrain::GetDesiredSystemChanges(
    void
) {
    return (System::Changes::None);
}

void
PhysicObjectTerrain::GetProperties(
    Properties::Array& Properties
) {
}

void
PhysicObjectTerrain::Update(f32 DeltaTime) {
    UNREFERENCED_PARAM(DeltaTime);
}

///////////////////////////////////////////////////////////////////////////////
// ChangeOccurred - Give this Object a change to process this system change
Error
PhysicObjectTerrain::ChangeOccurred(
    ISubject* pSubject,
    System::Changes::BitMask ChangeType
) {
    ASSERT(m_bInitialized);
    return Errors::Success;
}


///////////////////////////////////////////////////////////////////////////////
// GetPotentialSystemChanges - Get all system change possible for this Object
System::Changes::BitMask
PhysicObjectTerrain::GetPotentialSystemChanges(
    void
) {
    return System::Changes::None;
}
