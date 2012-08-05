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
#include "Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h"
#include "Physics/Utilities/Destruction/BreakOffParts/hkpBreakOffPartsUtil.h"
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
#include "BaseTypes.h"
#include "Interface.h"

//
// Havok system includes
//
#include "Collision.h"
#include "System.h"
#include "Scene.h"
#include "Task.h"
#include "Object/Object.h"
#include "Object/ObjectPhysics.h"
#include "Object/ObjectCharacter.h"
#include "Object/ObjectTerrain.h"
#include "Shape/TerrainShape.h"

/*
///////////////////////////////////////////////////////////////////////////////
// CollisionListener - Local helper classes
class CollisionListener : public hkpCollisionListener
{
    virtual void  contactPointAddedCallback (hkpContactPointAddedEvent &event){}
    virtual void  contactPointConfirmedCallback (hkpContactPointConfirmedEvent &event);
    virtual void  contactPointRemovedCallback (hkpContactPointRemovedEvent &event){}
};


///////////////////////////////////////////////////////////////////////////////
// contactPointConfirmedCallback - Callback that handles when an object collide
void CollisionListener::contactPointConfirmedCallback(hkpContactPointConfirmedEvent &event)
{
    f32 Impact = -event.m_projectedVelocity;

    static const f32 skfMaxImpact = 100.0f;

    // Filter out collisions with low impact
    // (prevent collision messages spamming)
    if( Impact > skfMaxImpact )
    {
        // Get physics object for bodyA
        hkpEntity* pEntityA = (hkpEntity*)event.m_collidableA->getRootCollidable()->getOwner();
        HavokPhysicsObject* pObjectA = (HavokPhysicsObject*)pEntityA->getUserData();

        // Get physics object for bodyB
        hkpEntity* pEntityB = (hkpEntity*)event.m_collidableB->getRootCollidable()->getOwner();
        HavokPhysicsObject* pObjectB = (HavokPhysicsObject*)pEntityB->getUserData();

        // Fill out physics object
        IContactObject::Info ContactInfo;

        const hkVector4& Position = event.m_contactPoint->getPosition();
        Position.store3( ContactInfo.m_Position );

        const hkVector4& Normal = event.m_contactPoint->getNormal();
        Normal.store3( ContactInfo.m_Normal );

        ContactInfo.m_Impact = Impact;
        ContactInfo.m_Static = ( pEntityA->getMotion()->m_type == hkpMotion::MOTION_FIXED || pEntityB->getMotion()->m_type == hkpMotion::MOTION_FIXED );

        ContactInfo.m_VelocityObjectA = Math::Vector3::Zero;
        ContactInfo.m_VelocityObjectB = Math::Vector3::Zero;

        // Tell the objects about the contact
        if( pObjectA )
        {
            const hkVector4& linA = pObjectA->GetRigidBody()->getLinearVelocity();
            linA.store3( ContactInfo.m_VelocityObjectA );
            pObjectA->AddContact( ContactInfo );
        }

        if( pObjectB )
        {
            const hkVector4& linB = pObjectB->GetRigidBody()->getLinearVelocity();
            linB.store3( ContactInfo.m_VelocityObjectB );
            pObjectB->AddContact( ContactInfo );
        }
    }
}
*/

//
// global variables
//
extern ManagerInterfaces    g_Managers;

const Math::Vector3 HavokPhysicsScene::sm_kDefaultGravity(0.0f, -9.8f, 0.0f);
const hkVector4 HavokPhysicsScene::sm_kDefaultUp(0.0f, 1.0f, 0.0f);

const char* HavokPhysicsScene::sm_kapszPropertyNames[] = {
    "SceneFile", "Gravity", "Material", "Elasticity", "Friction", "Softness",
};

const Properties::Property HavokPhysicsScene::sm_kaDefaultProperties[] = {
    Properties::Property(sm_kapszPropertyNames[ Property_SceneFile ],
    VALUE4(Properties::Values::Path, Properties::Values::String,
    Properties::Values::Float32, Properties::Values::Float32),
    Properties::Flags::Valid | Properties::Flags::InitOnly,
    "", "", 0.0f, 0.0f),

    Properties::Property(sm_kapszPropertyNames[ Property_Gravity ],
    Properties::Values::Vector3,
    Properties::Flags::Valid,
    sm_kDefaultGravity),

    Properties::Property(sm_kapszPropertyNames[ Property_Material ],
    Properties::Values::String,
    Properties::Flags::Valid | Properties::Flags::Multiple,
    ""),

    Properties::Property(sm_kapszPropertyNames[ Property_Elasticity ],
    VALUE3(Properties::Values::String, Properties::Values::String,
    Properties::Values::Float32),
    Properties::Flags::Valid | Properties::Flags::Multiple,
    "", "", 0.0f),

    Properties::Property(sm_kapszPropertyNames[ Property_Friction ],
    VALUE4(Properties::Values::String, Properties::Values::String,
    Properties::Values::Float32, Properties::Values::Float32),
    Properties::Flags::Valid | Properties::Flags::Multiple,
    "", "", 0.0f, 0.0f),

    Properties::Property(sm_kapszPropertyNames[ Property_Softness ],
    VALUE3(Properties::Values::String, Properties::Values::String,
    Properties::Values::Float32),
    Properties::Flags::Valid | Properties::Flags::Multiple,
    "", "", 0.0f),
};

///////////////////////////////////////////////////////////////////////////////
// HavokPhysicsScene - Default constructor
HavokPhysicsScene::HavokPhysicsScene(
    ISystem* pSystem
)
    : ISystemScene(pSystem)
    , m_kVelocityModifier(0.5f, 0.5f, 0.5f)
    , m_kfMass(50.0f)
    , m_pTask(NULL)
    , m_pWorld(NULL)
    , m_pLoader(NULL) {
    ASSERT(Property_Count == sizeof sm_kapszPropertyNames / sizeof sm_kapszPropertyNames[ 0 ]);
    ASSERT(Property_Count == sizeof sm_kaDefaultProperties / sizeof sm_kaDefaultProperties[ 0 ]);
}


///////////////////////////////////////////////////////////////////////////////
// ~HavokPhysicsScene - Default destructor
HavokPhysicsScene::~HavokPhysicsScene(
    void
) {
    if (m_pWorld != NULL) {
        m_pWorld->markForWrite();
    }

    //
    // Delete the task first because it attached itself to the world as a listener.
    //
    SAFE_DELETE(m_pTask);

    if (m_pWorld != NULL) {
        m_pWorld->removeReference();
        //
        // Can't do this since the world is deleted at this point.  Can't do it before the reference
        //  because it needs the mark for write ro remove the reference.
        //
        //m_pWorld->unmarkForWrite();
    }

    /*if ( m_pLoader != NULL )
    {
        reinterpret_cast<hkLoader*>(m_pLoader)->removeReference();
    }*/
}


///////////////////////////////////////////////////////////////////////////////
// GetSystemType - Returns System type for this Scene
System::Type
HavokPhysicsScene::GetSystemType(
    void
) {
    return System::Types::Physic;
}


///////////////////////////////////////////////////////////////////////////////
// GlobalSceneStatusChanged - Called from the framework to inform the scene
// extension of the overall scene status.
void
HavokPhysicsScene::GlobalSceneStatusChanged(
    GlobalSceneStatus Status
) {
    if (m_pWorld != NULL  &&  Status == ISystemScene::GlobalSceneStatus::PostLoadingObjects) {
        /*
        //
        // Create the loader.
        //
        if ( m_pLoader == NULL )
        {
            m_pLoader = new hkLoader();
            ASSERT( m_pLoader != NULL );
        }
        hkLoader* pLoader = reinterpret_cast<hkLoader*>(m_pLoader);

        //
        // Iterate through all the scene files, load them, and create the contents.
        //
        for ( SceneFiles::iterator itSF=m_SceneFiles.begin(); itSF != m_SceneFiles.end();
              itSF++ )
        {
            //
            // Load the scene file and get the physics data.
            //
            hkRootLevelContainer* pRootLevelContainer = pLoader->load( itSF->sPathName.c_str() );
            ASSERT( pRootLevelContainer != NULL );

            hkxScene* pScene =
                reinterpret_cast<hkxScene*>(pRootLevelContainer->findObjectByType(
                    hkxSceneClass.getName()
                    ));

            hkpPhysicsData* pPhysicsData =
                reinterpret_cast<hkpPhysicsData*>(pRootLevelContainer->findObjectByType(
                    hkpPhysicsDataClass.getName()
                    ));
            ASSERT( pPhysicsData != NULL );
            pPhysicsData->addReference();
        }

        pLoader->removeReference();
        */
    }
}


///////////////////////////////////////////////////////////////////////////////
// Initialize - Initializes this Scene with the given properties
Error
HavokPhysicsScene::Initialize(
    std::vector<Properties::Property> Properties
) {
    ASSERT(!m_bInitialized);

    //
    // Read the properties.
    //
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();

            if (sName == sm_kapszPropertyNames[ Property_SceneFile ]) {
                SceneFileData sfd;
                sfd.sPathName = it->GetString(0);
                std::string sType =  it->GetString(1);

                if (sType == "Extend") {
                    sfd.Type = SceneFileTypes::Extend;
                } else if (sType == "Loose") {
                    sfd.Type = SceneFileTypes::Loose;
                } else if (sType == "BreakOffParts") {
                    sfd.Type = SceneFileTypes::BreakOffParts;
                } else {
                    ASSERTMSG(false, "Unknown scene type.");
                }

                sfd.Value1 = it->GetFloat32(2);
                sfd.Value2 = it->GetFloat32(3);
                m_SceneFiles.push_back(sfd);
                it->ClearFlag(Properties::Flags::Valid);
            }
        }
    }

    HavokPhysicsSystem* physicSystem = reinterpret_cast<HavokPhysicsSystem*>(GetSystem());
    hkJobQueue* pJobQueue = physicSystem->getJobQueue();
    //
    // Create the world
    //
    {
        hkpWorldCinfo worldInfo;
        worldInfo.m_simulationType = hkpWorldCinfo::SIMULATION_TYPE_MULTITHREADED;
        worldInfo.m_broadPhaseBorderBehaviour = hkpWorldCinfo::BROADPHASE_BORDER_REMOVE_ENTITY;
        worldInfo.setBroadPhaseWorldSize(10000.0f);
        worldInfo.m_collisionTolerance = 0.03f;
        m_pWorld = new hkpWorld(worldInfo);
    }
    //
    // Start editing the world.
    //
    m_pWorld->markForWrite();
    hkpAgentRegisterUtil::registerAllAgents(m_pWorld->getCollisionDispatcher());
    m_pWorld->registerWithJobQueue(pJobQueue);
    //
    // Now we have finished modifying the world, release our write marker.
    //
    m_pWorld->unmarkForWrite();
    //
    // Create the task for simulating physics.
    //
    m_pTask = new HavokPhysicsTask(this, pJobQueue, m_pWorld);
    ASSERT(m_pTask != NULL);
    //
    // Set this set as initialized.
    //
    m_bInitialized = true;
    //
    // Set the properties for this scene.
    //
    //SetProperties( Properties );
    return Errors::Success;
}


///////////////////////////////////////////////////////////////////////////////
// GetProperties - Properties for this Scene are returned in Properties
void
HavokPhysicsScene::GetProperties(
    Properties::Array& Properties
) {
    //
    // Get the index of our first item.
    //
    i32 iProperty = static_cast<i32>(Properties.size());
    //
    // Add all the properties.
    //
    Properties.reserve(Properties.size() + Property_Count);

    for (i32 i = 0; i < Property_Count; i++) {
        Properties.push_back(sm_kaDefaultProperties[ i ]);
    }

    //
    // Modify the default values.
    //
    if (m_pWorld != NULL) {
        const hkVector4 hkGravity = m_pWorld->getGravity();
        Math::Vector3 Gravity;
        hkGravity.store3(Gravity);
        Properties[ iProperty + Property_Gravity ].SetValue(Gravity);
    }
}


///////////////////////////////////////////////////////////////////////////////
// SetProperties - Set properties for this Scene
void
HavokPhysicsScene::SetProperties(
    Properties::Array Properties
) {
    ASSERT(m_bInitialized);
    m_pWorld->markForWrite();

    //
    // Read in the properties.
    //
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();

            if (sName == sm_kapszPropertyNames[ Property_Gravity ]) {
                ASSERT(m_pWorld != NULL);
                //
                // Set the gravity.
                //
                const Math::Vector3& Gravity = it->GetVector3();
                hkVector4 hkGravity(Gravity.x, Gravity.y, Gravity.z);
                m_pWorld->setGravity(hkGravity);
            } else if (sName == sm_kapszPropertyNames[ Property_Material ]) {
                const std::string sMaterialName = it->GetString(0);
                ASSERTMSG1(GetMaterialId(sMaterialName) == -1,
                           "Physical material %s already exists.",
                           it->GetStringPtr(0));
                //
                // Create the material and add it to the list.
                //
                //Material m;
                //m.Name = sMaterialName;
                //m.Id = HavokMaterialCreateGroupID( m_pWorld );
                //m_Materials.push_back( m );
            } else if (sName == sm_kapszPropertyNames[ Property_Elasticity ]) {
                //i32 Material1Id = GetMaterialId( it->GetString( 0 ) );
                //ASSERTMSG1( Material1Id != -1, "Physical material %s does not exist.",
                //            it->GetStringPtr( 0 ) );
                //i32 Material2Id = GetMaterialId( it->GetString( 1 ) );
                //ASSERTMSG1( Material1Id != -1, "Physical material %s does not exist.",
                //            it->GetStringPtr( 1 ) );
                //f32 Coefficient = it->GetFloat32( 2 );
                //
                // Set the elasticity between the two materials.
                //
                //HavokMaterialSetDefaultElasticity(
                //    m_pWorld, Material1Id, Material2Id, Coefficient
                //    );
            } else if (sName == sm_kapszPropertyNames[ Property_Friction ]) {
                //i32 Material1Id = GetMaterialId( it->GetString( 0 ) );
                //ASSERTMSG1( Material1Id != -1, "Physical material %s does not exist.",
                //            it->GetStringPtr( 0 ) );
                //i32 Material2Id = GetMaterialId( it->GetString( 1 ) );
                //ASSERTMSG1( Material1Id != -1, "Physical material %s does not exist.",
                //            it->GetStringPtr( 1 ) );
                //f32 Static = it->GetFloat32( 2 );
                //f32 Kinetic = it->GetFloat32( 3 );
                //
                // Set the friction between the two materials.
                //
                //HavokMaterialSetDefaultFriction(
                //    m_pWorld, Material1Id, Material2Id, Static, Kinetic
                //    );
            } else if (sName == sm_kapszPropertyNames[ Property_Softness ]) {
                //i32 Material1Id = GetMaterialId( it->GetString( 0 ) );
                //ASSERTMSG1( Material1Id != -1, "Physical material %s does not exist.",
                //            it->GetStringPtr( 0 ) );
                ////i32 Material2Id = GetMaterialId( it->GetString( 1 ) );
                //ASSERTMSG1( Material1Id != -1, "Physical material %s does not exist.",
                //            it->GetStringPtr( 1 ) );
                //f32 Value = it->GetFloat32( 2 );
                //
                // Set the softness between the two materials.
                //
                //HavokMaterialSetDefaultSoftness(
                //    m_pWorld, Material1Id, Material2Id, Value
                //    );
            } else {
                ASSERTMSG(false, "Unknown property");
            }

            //
            // Set this property to invalid since it's already been read.
            //
            it->ClearFlag(Properties::Flags::Valid);
        }
    }

    m_pWorld->unmarkForWrite();
}


///////////////////////////////////////////////////////////////////////////////
// GetObjectTypes - Get Object types for this Scene
const char**
HavokPhysicsScene::GetObjectTypes(
    void
) {
    return HavokPhysicsObject::sm_kapszTypeNames;
}


///////////////////////////////////////////////////////////////////////////////
// CreateObject - Create an Object with the given Name and Type
ISystemObject*
HavokPhysicsScene::CreateObject(
    const char* pszName,
    const char* pszType
) {
    ASSERT(m_bInitialized);
    ASSERT(pszType != NULL);
    ASSERT(pszName != NULL);

    if (strcmp(pszType, "Terrain") == 0) {
        PhysicObjectTerrain* pObject = new PhysicObjectTerrain(this, pszName);
        pObject->SetType(pszType);
        return pObject;
    } else if (strcmp(pszType, "Character") == 0) {
        HavokCharacterObject* pObject = new HavokCharacterObject(this, pszName);
        pObject->SetType(pszType);
        m_Characters.push_back(pObject);
        return pObject;
    } else {
        HavokPhysicsObject* pObject = new HavokPhysicsObject(this, pszType, pszName);
        pObject->SetType(pszType);
        return pObject;
    }
}


///////////////////////////////////////////////////////////////////////////////
// DestroyObject - Destorys the given Object, removing it from the Scene
Error
HavokPhysicsScene::DestroyObject(
    ISystemObject* pSystemObject
) {
    ASSERT(m_bInitialized);
    ASSERT(pSystemObject != NULL);
    //
    // Cast to a HavokCharacterObject or HavokPhysicsObject so that the correct destructor will be called.
    //
    HavokObject* pObject = reinterpret_cast<HavokObject*>(pSystemObject);

    if (strcmp(pObject->GetType(), "Terrain") == 0) {
        PhysicObjectTerrain* pTerrainObject = reinterpret_cast<PhysicObjectTerrain*>(pSystemObject);
        SAFE_DELETE(pTerrainObject);
    } else if (strcmp(pObject->GetType(), "Character") == 0) {
        HavokCharacterObject* pCharacterObject = reinterpret_cast<HavokCharacterObject*>(pSystemObject);
        SAFE_DELETE(pCharacterObject);
    } else {
        HavokPhysicsObject* pPhysicsObject = reinterpret_cast<HavokPhysicsObject*>(pSystemObject);
        SAFE_DELETE(pPhysicsObject);
    }

    return Errors::Success;
}


///////////////////////////////////////////////////////////////////////////////
// GetSystemTask - Returns the task associated with this Scene
ISystemTask*
HavokPhysicsScene::GetSystemTask(
    void
) {
    return m_pTask;
}


void
HavokPhysicsScene::Update(
    f32 DeltaTime
) {
    UNREFERENCED_PARAM(DeltaTime);
}


///////////////////////////////////////////////////////////////////////////////
// GetPotentialSystemChanges - Returns systems changes possible for this Scene
System::Changes::BitMask
HavokPhysicsScene::GetPotentialSystemChanges(
    void
) {
    return System::Changes::Generic::ExtendObject;
}


///////////////////////////////////////////////////////////////////////////////
// GetCreateObjects - Returns the creation data of all the universal objects to create.
void
HavokPhysicsScene::GetCreateObjects(
    std::vector<CreateObjectData>& apszNames
) {
    UNREFERENCED_PARAM(apszNames);
    ASSERT(false, "Not implemented.");
}


///////////////////////////////////////////////////////////////////////////////
// GetDestroyObjects - Returns the names of all the universal objects to destroy.
void
HavokPhysicsScene::GetDestroyObjects(
    std::vector<const char*>& apszNames
) {
    UNREFERENCED_PARAM(apszNames);
    ASSERT(false, "Not implemented.");
}


///////////////////////////////////////////////////////////////////////////////
// GetExtendObjects - Returns the names of all the universal objects to extend.
void
HavokPhysicsScene::GetExtendObjects(
    ExtendObjectDataArray& apszNames
) {
    SCOPED_SPIN_LOCK(m_BrokenOffPartsSpinWait);

    for (std::map<void*, ExtensionData>::iterator it = m_aExtensions.begin();
            it != m_aExtensions.end(); it++) {
        IGenericScene::ExtendObjectData eod;
        eod.pszName = it->second.pszName;
        eod.pUserData = it->second.pBody;
        apszNames.push_back(eod);
    }

    m_aExtensions.clear();
}


///////////////////////////////////////////////////////////////////////////////
// GetUnextendObjects - Returns the names of all the universal objects to unextend.
void
HavokPhysicsScene::GetUnextendObjects(
    std::vector<const char*>& apszNames
) {
    UNREFERENCED_PARAM(apszNames);
    ASSERT(false, "Not implemented.");
}


///////////////////////////////////////////////////////////////////////////////
// ExtendObject - Returns the universal object extension.
ISystemObject*
HavokPhysicsScene::ExtendObject(
    const char* pszName,
    void* pUserData
) {
    ASSERT(pszName != NULL);
    ASSERT(pUserData != NULL);
    //
    // Get the body and create the object.
    //
    hkpRigidBody* pBody = reinterpret_cast<hkpRigidBody*>(pUserData);
    ASSERT(pBody != NULL);
    HavokPhysicsObject* pObject = new HavokPhysicsObject(this, "Dynamic", pszName, pBody);
    ASSERT(pObject != NULL);
    //
    // Set the type and set the object as activated.
    //
    pObject->SetType("Dynamic");
    m_pTask->SetObjectActivation(pObject);
    return pObject;
}


///////////////////////////////////////////////////////////////////////////////
// UnextendObject - Informs the ISystemScene of the object extension being removed.
ISystemObject*
HavokPhysicsScene::UnextendObject(
    const char* pszName
) {
    UNREFERENCED_PARAM(pszName);
    ASSERT(false, "Not implemented.");
    return NULL;
}


///////////////////////////////////////////////////////////////////////////////
// GetMaterialId - Returns the ID for the given material name
i32
HavokPhysicsScene::GetMaterialId(
    const std::string& sName
) {
    i32 Id = -1;

    for (Materials::const_iterator it = m_Materials.begin(); it != m_Materials.end(); it++) {
        if ((*it).Name == sName) {
            Id = (*it).Id;
            break;
        }
    }

    return Id;
}


///////////////////////////////////////////////////////////////////////////////
// breakOffSubPart - Callback when a piece needs to be broken off
/*hkResult
HavokPhysicsScene::breakOffSubPart(
   const ContactImpulseLimitBreachedEvent& Event,
   hkArray<hkpShapeKey>& KeysBrokenOff,
   hkpPhysicsSystem& PhysicsSystem
)
{
    hkResult Result = HK_FAILURE;

    hkInplaceArray<const hkpBreakOffPartsListener::ContactImpulseLimitBreachedEvent::PointInfo,4>::iterator Iterator = Event.m_points.begin();

    for( ; Iterator != Event.m_points.end(); Iterator++ )
    {
        //
        // Associate the broken pieces with a universal object.
        //
        const hkpBreakOffPartsListener::ContactImpulseLimitBreachedEvent::PointInfo* pPointInfo = Iterator;
        hkpShapeKey BrokenPieceKey = pPointInfo->m_brokenShapeKey;
        hkpRigidBody* pBreakingBody = Event.m_breakingBody;
        hkpRigidBody* pCollidingBody = pPointInfo->m_collidingBody;

        //
        // Determine what kind of shape it is.
        //
        hkpShape* pShape = const_cast<hkpShape*>(pBreakingBody->getCollidable()->getShape());
        const hkpShape* pBrokenOffShape = HK_NULL;

        switch ( pShape->m_type )
        {
        case hkcdShapeType::LIST:
            {
                hkpListShape* pShapeList = reinterpret_cast<hkpListShape*>( pShape );
                pBrokenOffShape = pShapeList->m_childInfo[ BrokenPieceKey ].m_shape;
                break;
            }

        case hkcdShapeType::MOPP:
            {
                hkpShapeBuffer ShapeBuffer;
                hkpMoppBvTreeShape* pMoppShape = reinterpret_cast<hkpMoppBvTreeShape*>( pShape );
                pBrokenOffShape = pMoppShape->getShapeCollection()->getChildShape(
                    BrokenPieceKey, ShapeBuffer
                    );
                ASSERT( (void*)pBrokenOffShape != (void*)ShapeBuffer );
                break;
            }

        default:
            ASSERTMSG( false, "This shape is not implemented yet" );
        }

        //
        // Make sure we have a shape that has volume.
        //
        if ( pBrokenOffShape != NULL )
        {
            hkAabb Aabb;
            pBrokenOffShape->getAabb( hkTransform::getIdentity(), 0.0f, Aabb );

            hkVector4 Diff;
            Diff.setSub4(Aabb.m_max, Aabb.m_min);
            Diff.setAbs4( Diff );

            Math::Vector3 vDiff;
            Diff.store3( vDiff );

            if ( vDiff.x < 0.1f || vDiff.y < 0.1f || vDiff.z < 0.1f )
            {
                pBrokenOffShape = NULL;
            }
        }

        //
        // Check to see if it has already been detached by another thread.
        //
        bool bProcessDetach = false;

        if ( pBrokenOffShape != NULL )
        {

            //
            // The first thread to add this to the map wins.
            //
            SCOPED_SPIN_LOCK( m_BrokenOffPartsSpinWait );

            //
            // Check to see if the part isn't already in the map.
            //
            if ( m_aExtensions.find( (void*)pBrokenOffShape ) == m_aExtensions.end() )
            {
                //
                // Add a dummy entry.
                //
                ExtensionData ed;
                ::memset( &ed, 0, sizeof ed );
                m_aExtensions[ (void*)pBrokenOffShape ] = ed;

                bProcessDetach = true;
            }
            else
            {
                Result = HK_SUCCESS;
            }
        }

        //
        // Detach the shape from the big body and create new a body for the broken off shape.
        //
        if ( bProcessDetach )
        {
            //
            // Create a body.
            //
            hkpRigidBody* pNewBody = NULL;

            hkVector4 LinearVelocity = pCollidingBody->getLinearVelocity();
            hkVector4 AngularVelocity = pCollidingBody->getAngularVelocity();
            LinearVelocity.mul4( m_kVelocityModifier );
            AngularVelocity.mul4( m_kVelocityModifier );
            LinearVelocity.add4( pBreakingBody->getLinearVelocity() );
            AngularVelocity.add4( pBreakingBody->getAngularVelocity() );

            hkpRigidBodyCinfo RigidBodyCinfo;
            RigidBodyCinfo.m_shape = pBrokenOffShape;
            RigidBodyCinfo.m_position = pBreakingBody->getPosition();
            RigidBodyCinfo.m_rotation = pBreakingBody->getRotation();
            RigidBodyCinfo.m_linearVelocity = LinearVelocity;
            RigidBodyCinfo.m_angularVelocity = AngularVelocity;
            RigidBodyCinfo.m_mass = m_kfMass;
            RigidBodyCinfo.m_solverDeactivation = hkpRigidBodyCinfo::SOLVER_DEACTIVATION_HIGH;
            RigidBodyCinfo.m_qualityType = HK_COLLIDABLE_QUALITY_DEBRIS;

            hkpInertiaTensorComputer::setShapeVolumeMassProperties(
                pBrokenOffShape, RigidBodyCinfo.m_mass, RigidBodyCinfo
                );

            static const f32 kfMaxInertiaRatio = 2000.0f;
            hkpInertiaTensorComputer::clipInertia( kfMaxInertiaRatio, RigidBodyCinfo );

            pNewBody = new hkpRigidBody( RigidBodyCinfo );

            //
            // If the original unbroken body is fixed, the colliding impulse is lost.  Just apply the
            //  impulse to the new piece
            //
            if ( pBreakingBody->isFixedOrKeyframed() )
            {
                hkReal BreakImpulse = pPointInfo->m_properties->m_maxImpulse;
                hkVector4 Impulse;

                hkReal BreakImpulseModifier = -BreakImpulse * 0.2f;

                Impulse.setMul4( BreakImpulseModifier, pPointInfo->m_contactPoint->getNormal() );
                pNewBody->applyPointImpulse( Impulse, pPointInfo->m_contactPoint->getPosition() );
            }

            //
            // Add to the list of broken off body parts.  Will be used later to extend a UObject.
            //
            ExtensionData ed;
            ed.pszName = reinterpret_cast<const char*>(pBrokenOffShape->getUserData());
            ed.pBody = pNewBody;

            {
                //
                // Lock PhysicsSystem, KeysBrokenOff, and m_aBrokenOffParts.
                //
                SCOPED_SPIN_LOCK( m_BrokenOffPartsSpinWait );

                //
                // Remove the shape from the break-off-parts utility.
                //
                hkpBreakOffPartsUtil::removeKeysFromListShape( pBreakingBody, &BrokenPieceKey, 1 );
                KeysBrokenOff.pushBack( BrokenPieceKey );

                //
                // Update the broken parts list.
                //
                m_aExtensions[ (void*)pBrokenOffShape ] = ed;
            }

            //
            // Inform the universal scene we want to extend an object.
            //
            PostChanges( System::Changes::Generic::ExtendObject );

            Result = HK_SUCCESS;
        }
    }

    return Result;
}*/


///////////////////////////////////////////////////////////////////////////////
// AddTrackCollision - Adds a collision to track and report
void
HavokPhysicsScene::AddTrackCollision(
    HavokPhysicsObject* pSubject,
    HavokPhysicsObject* pObserver
) {
}


///////////////////////////////////////////////////////////////////////////////
// RemoveTrackCollision - Removes a collision to track and report
void
HavokPhysicsScene::RemoveTrackCollision(
    HavokPhysicsObject* pSubject,
    HavokPhysicsObject* pObserver
) {
}
