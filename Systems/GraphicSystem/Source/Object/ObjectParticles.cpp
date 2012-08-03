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
// Temporarily switching warning level to 0 to ignore warnings in extern/Ogre
#include "Ogre.h"
#pragma warning( pop )

#ifdef SAFE_DELETE
#undef SAFE_DELETE
#endif
#ifdef SAFE_DELETE_ARRAY
#undef SAFE_DELETE_ARRAY
#endif

//
// core includes
//
#include <BaseTypes.h>
#include <Interface.h>
#include "DynamicBuffer.h"
#include "windows.h"

//
// Ogre system includes
//
#include "Scene.h"
#include "System.h"
#include "Object.h"
#include "ObjectParticles.h"

#include <IttNotify.h>

//
// Different sets of events to log in Thread Profiler
//
#if 0
__ITT_DEFINE_STATIC_EVENT(g_tpeChangeOccurred, NULL, 0);
__ITT_DEFINE_STATIC_EVENT(g_tpeGetVertices, "Particles: Get Vertices", 23);
__ITT_DEFINE_STATIC_EVENT(g_tpeSetVertexBufferParams, "Particles: Set Vertex Buf Params", 32);
__ITT_DEFINE_STATIC_EVENT(g_tpeSetVertexDecl, "Particles: Set Vertex Decl", 26);
#else
__ITT_DEFINE_STATIC_EVENT(g_tpeChangeOccurred, "Graphics Particles: ChangeOccurred", 34);
__ITT_DEFINE_STATIC_EVENT(g_tpeGetVertices, NULL, 0);
__ITT_DEFINE_STATIC_EVENT(g_tpeSetVertexBufferParams, NULL, 0);
__ITT_DEFINE_STATIC_EVENT(g_tpeSetVertexDecl, NULL, 0);
#endif

#define POGRESCENEMGR   (reinterpret_cast<OGREGraphicsScene*>(m_pSystemScene)->GetOGRESceneManager())
#define PSYSTEM         (reinterpret_cast<GraphicSystem*>(m_pSystemScene->GetSystem()))

#define WATER_PARTICLE 7

const f32 GraphicObjectParticles::m_fFireBBHeight = 500.0f;

u32 GraphicObjectParticles::sm_EntityId = 0;

pcstr GraphicObjectParticles::sm_kapszPropertyNames[] = {
    "PointList", "ProceduralPointList", "Material", "CastShadows",
};

const Properties::Property GraphicObjectParticles::sm_kaDefaultProperties[] = {
    Properties::Property(sm_kapszPropertyNames[ Property_PointList ],
    VALUE1(Properties::Values::String),
    Properties::Flags::Valid | Properties::Flags::InitOnly,
    ""),
    Properties::Property(sm_kapszPropertyNames[ Property_ProceduralPointList ],
    VALUE1(Properties::Values::Boolean),
    Properties::Flags::Valid | Properties::Flags::InitOnly,
    False),
    Properties::Property(sm_kapszPropertyNames[ Property_Material ],
    VALUE1(Properties::Values::String),
    Properties::Flags::Valid | Properties::Flags::WriteOnly,
    ""),
    Properties::Property(sm_kapszPropertyNames[ Property_CastShadows ],
    VALUE1(Properties::Values::Int32),
    Properties::Flags::Valid,
    0),
};


GraphicObjectParticles::GraphicObjectParticles(
    ISystemScene* pSystemScene,
    pcstr pszName
)
    : GraphicObject(pSystemScene, pszName)
    , m_pDynamicObject(NULL) {
    ASSERT(Property_Count == sizeof sm_kapszPropertyNames / sizeof sm_kapszPropertyNames[ 0 ]);
    ASSERT(Property_Count == sizeof sm_kaDefaultProperties / sizeof sm_kaDefaultProperties[ 0 ]);
    m_Type = GraphicObject::Type_PointList;
}


GraphicObjectParticles::~GraphicObjectParticles(
    void
) {
    if (m_pDynamicObject) {
        m_pNode->detachObject(m_pDynamicObject);
    }

    SAFE_DELETE(m_pDynamicObject);
}


Error
GraphicObjectParticles::Initialize(
    std::vector<Properties::Property> Properties
) {
    Error Err = Errors::Success;
    ASSERT(!m_bInitialized);
    //
    // Call the base class.
    //
    Err = GraphicObject::Initialize(Properties);

    //
    // Read in the initialization only properties.
    //

    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid &&
                it->GetFlags() & Properties::Flags::InitOnly) {
            std::string sName = it->GetName();

            if (sName == sm_kapszPropertyNames[ Property_PointList ]) {
                it->ClearFlag(Properties::Flags::Valid);
            } else if (sName == sm_kapszPropertyNames[ Property_ProceduralPointList ]) {
                it->ClearFlag(Properties::Flags::Valid);
            }
        }
    }

    m_pDynamicObject = new DynamicBuffer();

    if (!m_pDynamicObject) {
        ASSERT(False);
        Err = Errors::Failure;
    }

    m_pDynamicObject->setCustomParameter(0, Ogre::Vector4(0.0f, 0.0f, 0.0f, 0.0f));
    m_pDynamicObject->setCustomParameter(1, Ogre::Vector4(0.0f, 0.0f, 0.0f, 0.0f));
    //
    // Set this set as initialized.
    //
    m_bInitialized = True;
    m_pNode->attachObject(m_pDynamicObject);
    //
    // Set the remaining properties for this object.
    //
    SetProperties(Properties);
    return Err;
}


void
GraphicObjectParticles::GetProperties(
    Properties::Array& Properties
) {
    //
    // Add all the properties.
    //
    Properties.reserve(Properties.size() + Property_Count);

    for (i32 i = 0; i < Property_Count; i++) {
        Properties.push_back(sm_kaDefaultProperties[ i ]);
    }
}


void
GraphicObjectParticles::SetProperties(
    Properties::Array Properties
) {
    ASSERT(m_bInitialized);

    //
    // Read in the properties.
    //
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();

            if (sName == sm_kapszPropertyNames[ Property_Material ]) {
                if (m_pDynamicObject != NULL) {
                    m_pDynamicObject->setMaterial(it->GetStringPtr(0));
                } else {
                    ASSERT(False);
                }
            } else if (sName == sm_kapszPropertyNames[ Property_CastShadows ]) {
                if (m_pDynamicObject != NULL) {
                    m_pDynamicObject->setCastShadows(it->GetBool(0) != False);
                } else {
                    ASSERT(False);
                }
            } else {
                ASSERT(False);
            }

            //
            // Set this property to invalid since it's already been read.
            //
            it->ClearFlag(Properties::Flags::Valid);
        }
    }
}


System::Types::BitMask
GraphicObjectParticles::GetDesiredSystemChanges(
    void
) {
    return System::Changes::Custom | System::Changes::Graphics::All | GraphicObject::GetDesiredSystemChanges();
}


Error
GraphicObjectParticles::ChangeOccurred(
    ISubject* pSubject,
    System::Changes::BitMask ChangeType
) {
    __ITT_EVENT_START(g_tpeChangeOccurred, PROFILE_PARTICLES);
    IGraphicsObject* pGfxObj = dynamic_cast<IGraphicsObject*>(pSubject);
    SCOPED_SPIN_LOCK(OGREGraphicsScene::m_mutex);

    if (ChangeType & System::Changes::Graphics::VertexBuffer) {
        //
        // Get the vertex buffer from the graphics object.
        //
        __ITT_EVENT_START(g_tpeGetVertices, PROFILE_PARTICLES);
        void* pVB = NULL;
        u32 vertexCount = pGfxObj->GetVertexCount();
        m_pDynamicObject->prepareHardwareBuffers(vertexCount);
        pVB = m_pDynamicObject->lockBuffer();
        {
            pGfxObj->GetVertices(pVB);
        }
        m_pDynamicObject->unlockBuffer();
        __ITT_EVENT_END(g_tpeGetVertices, PROFILE_PARTICLES);
        //
        // Get the AABB information from the graphics object.
        //
        Math::Vector3 AABBMin;
        Math::Vector3 AABBMax;
        pGfxObj->GetAABB(AABBMin, AABBMax);
        AABBMax.y += m_fFireBBHeight;  // make bounding boxes a taller for the embers and smoke
        Ogre::AxisAlignedBox oaabb(Ogre::Vector3(AABBMin.x, AABBMin.y, AABBMin.z), Ogre::Vector3(AABBMax.x, AABBMax.y, AABBMax.z));
        m_pDynamicObject->setBoundingBox(oaabb);
        m_pDynamicObject->setExtents(AABBMin, AABBMax);
        m_pNode->_updateBounds(); // Update the "dirty" flag in Ogre for the node so that the new bounding box is rendered!
        f32 Height = static_cast<f32>(PSYSTEM->GetOGRERenderWindow()->getHeight());
        __ITT_EVENT_START(g_tpeSetVertexBufferParams, PROFILE_PARTICLES);
        m_pDynamicObject->setCustomParameter(1, Ogre::Vector4(Height, 0.0f, 0.0f, 0.0f));
        m_pDynamicObject->setVisible(true);
        m_pNode->setVisible(true);
        __ITT_EVENT_END(g_tpeSetVertexBufferParams, PROFILE_PARTICLES);
    }

    if (ChangeType & System::Changes::Graphics::AABB) {
        //
        // Get the AABB information from the graphics object.
        //
        Math::Vector3 AABBMin;
        Math::Vector3 AABBMax;
        pGfxObj->GetAABB(AABBMin, AABBMax);
        m_pDynamicObject->setExtents(AABBMin, AABBMax);
        Ogre::AxisAlignedBox oaabb(Ogre::Vector3(AABBMin.x, AABBMin.y, AABBMin.z), Ogre::Vector3(AABBMax.x, AABBMax.y, AABBMax.z));
        m_pDynamicObject->setBoundingBox(oaabb);
    }

    if (ChangeType & System::Changes::Graphics::VertexDecl) {
        //
        // Get the vertex information from the graphics object.
        //
        IGraphicsParticleObject* pParticleObject = dynamic_cast<IGraphicsParticleObject*>(pSubject);
        u32 VertexDeclCount = pGfxObj->GetVertexDeclarationCount();
        VertexDecl::Element* pVertexDecl = new VertexDecl::Element[ VertexDeclCount ];
        ASSERT(pVertexDecl != NULL);
        pGfxObj->GetVertexDeclaration(pVertexDecl);
        //
        // Create the vertex declaration using the mappings below to go from those defined in
        //  Interfaces/Graphics.h to those that Ogre uses.
        //
        static const Ogre::VertexElementType aVETs[] = {
            Ogre::VET_COLOUR, Ogre::VET_FLOAT1, Ogre::VET_FLOAT2, Ogre::VET_FLOAT3, Ogre::VET_FLOAT4
        };
        static const Ogre::VertexElementSemantic aVESs[] = {
            Ogre::VES_POSITION,
            Ogre::VES_NORMAL,
            Ogre::VES_DIFFUSE,
            Ogre::VES_SPECULAR,
            Ogre::VES_TANGENT,
            Ogre::VES_TEXTURE_COORDINATES,
            Ogre::VES_BLEND_INDICES,
            Ogre::VES_BLEND_WEIGHTS,
        };
        std::string szTechniqueName;
        size_t Offset = 0;
        m_pOgreVertexDecl = m_pDynamicObject->getVertexDeclaration();
        __ITT_EVENT_START(g_tpeSetVertexDecl, PROFILE_PARTICLES);
        m_pOgreVertexDecl->removeAllElements();
        u16 texUsageCount = 0;

        for (u32 i = 0; i < VertexDeclCount; i++) {
            Offset += m_pOgreVertexDecl->addElement(
                          0, Offset, aVETs[ pVertexDecl[ i ].Type ],
                          aVESs[ pVertexDecl[ i ].Usage ],
                          ((pVertexDecl[ i ].Usage == VertexDecl::Usage::Texture) ? texUsageCount++ : 0)
                      ).getSize();
        }

        //
        // Set the Material type
        //
        szTechniqueName = pParticleObject->GetParticleGroupTechnique();
        m_pDynamicObject->setMaterial(szTechniqueName);
        __ITT_EVENT_END(g_tpeSetVertexDecl, PROFILE_PARTICLES);
        delete[] pVertexDecl;
        //---------------------------------------------------------------------
        // Give the dynamic objects their own rendering queue towards the end.
        // This makes them draw after other transparent objects in the scene,
        // like the house windows
        //
        m_pDynamicObject->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);

        //---------------------------------------------------------------------
        // Here we skew the draw order of the smoke and embers particles. The
        // smoke will draw first, then the fire (at 0), finally the embers
        //
        if (!szTechniqueName.compare("FirePatchSmoke")) {
            m_pDynamicObject->setDistanceFromCamera(50);
        } else if (!szTechniqueName.compare("FirePatchEmbers")) {
            m_pDynamicObject->setDistanceFromCamera(-50);
        }
    }

    __ITT_EVENT_END(g_tpeChangeOccurred, PROFILE_PARTICLES);
    return GraphicObject::ChangeOccurred(pSubject, ChangeType);
}


System::Changes::BitMask
GraphicObjectParticles::GetPotentialSystemChanges(
    void
) {
    return System::Changes::Graphics::All | GraphicObject::GetPotentialSystemChanges();
}


u32
GraphicObjectParticles::GetVertexDeclarationCount(
    In  i32 BufferBindingIndex
) {
    UNREFERENCED_PARAM(BufferBindingIndex);
    ASSERTMSG(False, "Need to implement this.");
    return 0;
}


void
GraphicObjectParticles::GetVertexDeclaration(
    Out VertexDecl::Element* pVertexDecl,
    In  i32 BufferBindingIndex
) {
    UNREFERENCED_PARAM(pVertexDecl);
    UNREFERENCED_PARAM(BufferBindingIndex);
    ASSERTMSG(False, "Need to implement this.");
    return;
}


u32
GraphicObjectParticles::GetVertexCount(
    void
) {
    ASSERTMSG(False, "Need to implement this.");
    return 0;
}



void
GraphicObjectParticles::GetVertices(
    Out void* pVertices
) {
    UNREFERENCED_PARAM(pVertices);
    ASSERTMSG(False, "Need to implement this.");
    return;
}


void
GraphicObjectParticles::GetAABB(
    Out Math::Vector3& Min,
    Out Math::Vector3& Max
) {
    UNREFERENCED_PARAM(Min);
    UNREFERENCED_PARAM(Max);
    ASSERTMSG(False, "Need to implement this.");
    return;
}


u32
GraphicObjectParticles::GetParticleGroupCount(
    void
) {
    ASSERTMSG(False, "Need to implement this.");
    return 0;
}


void
GraphicObjectParticles::GetParticleGroup(
    u32 iParticleGroup,
    Out ParticleGroupData& ParticleGroup
) {
    DBG_UNREFERENCED_PARAM(iParticleGroup);
    DBG_UNREFERENCED_PARAM(ParticleGroup);
    ASSERTMSG(False, "Need to implement this.");
}


u32
GraphicObjectParticles::GetParticleCount(
    u32 iParticleGroup
) {
    DBG_UNREFERENCED_PARAM(iParticleGroup);
    ASSERTMSG(False, "Need to implement this.");
    return 0;
}

std::string
GraphicObjectParticles::GetParticleGroupTechnique(
    void
) {
    ASSERTMSG(False, "Need to implement this.");
    return "No_Technique";
}

void
GraphicObjectParticles::GetParticles(
    u32 iParticleGroup,
    Out ParticleData* pParticles
) {
    DBG_UNREFERENCED_PARAM(iParticleGroup);
    DBG_UNREFERENCED_PARAM(pParticles);
    ASSERTMSG(False, "Need to implement this.");
    return;
}

