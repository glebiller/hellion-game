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


#pragma warning( push, 0 )
// Temporarily switching warning level to 0 to ignore warnings in extern/Ogre
#include "Ogre.h"
#include "OgreFontManager.h"
#pragma warning( pop )

#include "boost/functional/factory.hpp"

#include "BaseTypes.h"
#include "Interface.h"

#include "System.h"
#include "Scene.h"
#include "Task.h"
#include "Object/Object.h"
#include "Object/ObjectCamera.h"
#include "Object/ObjectLight.h"
#include "Object/ObjectMesh.h"
#include "Object/ObjectTerrain.h"
#include "Object/ObjectSky.h"

#include "SkyX.h"

#define POGREROOT       (reinterpret_cast<GraphicSystem*>(m_pSystem)->GetOGRERoot())

extern ManagerInterfaces       g_Managers;

// We use SM2.0 instancing. Since we do normal mapping on the instanced geometry, we
// need to pass both the world matrix and the inverse world matrix for each instance.
// Given our limit of 256 float4 constants, we set the Objects per batch to 40.
// 40 float3x3 world matrices           = 120 float3/4 constants
// 40 float3x3 world inverse matrices   = 120 float3/4 constants
// we have used 240 float4 constants. The rest 16 are saved to pass other information
// to the instancing shader.
static const u32 maxObjectsPerBatch = 40;
static const u32 UpdateGrainSize = 120;

DEFINE_SPIN_MUTEX(OGREGraphicsScene::m_mutex);

// Prototypes
void ProcessObjects(void* Data);



/**
 * @inheritDoc
 */
OGREGraphicsScene::OGREGraphicsScene(ISystem* pSystem) : ISystemScene(pSystem)
    , m_pTask(NULL)
    , m_pSceneManager(NULL)
    , m_pRootNode(NULL)
    , m_FogMode(Ogre::FOG_NONE)
    , m_ExpDensity(.001f)
    , m_LinearStart(0.0f)
    , m_LinearEnd(1.0f)
    , m_FogColor(1.0f, 1.0f, 1.0f, 1.0f) {

    m_ObjectFactories["Camera"] = boost::factory<GraphicObjectCamera*>();
    m_ObjectFactories["Light"] = boost::factory<GraphicObjectLight*>();
    m_ObjectFactories["Mesh"] = boost::factory<GraphicObjectMesh*>();
    //m_ObjectFactories["Sky"] = boost::factory<GraphicObjectSky*>();
    //m_ObjectFactories["Terrain"] = boost::factory<GraphicObjectTerrain*>();

}

/**
 * @inheritDoc
 */
OGREGraphicsScene::~OGREGraphicsScene(void) {
    SAFE_DELETE(m_pTask);
    ASSERT(m_pSceneManager != NULL);
    POGREROOT->destroySceneManager(m_pSceneManager);
}

/**
 * @inheritDoc
 */
Error OGREGraphicsScene::initialize(void) {
    ASSERT(!m_bInitialized);

    m_pSceneManager = POGREROOT->createSceneManager(Ogre::ST_EXTERIOR_FAR);
    ASSERT(m_pSceneManager != NULL);
    if (!m_pSceneManager) {
        return Errors::Failure;
    }

    m_pRootNode = m_pSceneManager->getRootSceneNode();
    ASSERT(m_pRootNode != NULL);
    m_pRootNode->hideBoundingBox(true);
    
    m_pTask = new OGREGraphicsTask(this, POGREROOT);
    ASSERT(m_pTask != NULL);
    if (m_pTask == NULL) {
        return Errors::Failure;
    }

    return Errors::Success;
}

/*
Error OGREGraphicsScene::initialize(void) {
    Error Err = Errors::Success;
    ASSERT(!m_bInitialized);

    //
    // Read the properties.
    //
    Ogre::ResourceGroupManager* pResourceGroupManager = Ogre::ResourceGroupManager::getSingletonPtr();

    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid &&
                it->GetFlags() & Properties::Flags::InitOnly) {
            std::string sName = it->GetName();

            if (sName == sm_kapszPropertyNames[ Property_ResourceLocation ]) {
                const char* pszName = it->GetStringPtr(0);
                const char* pszLocationType = it->GetStringPtr(1);
                const char* pszResourceGroup = it->GetStringPtr(2);
                bool  bRecursive = it->GetBool(3);
                pResourceGroupManager->addResourceLocation(pszName, pszLocationType, pszResourceGroup, (bRecursive == true));
                pResourceGroupManager->initialiseResourceGroup(pszResourceGroup);
                pResourceGroupManager->loadResourceGroup(pszResourceGroup);
            } else if (sName == sm_kapszPropertyNames[ Property_DelResourceLocation ]) {
                const char* pszName = it->GetStringPtr(0);
                const char* pszResourceGroup = it->GetStringPtr(1);
                pResourceGroupManager->unloadResourceGroup(pszResourceGroup);
                pResourceGroupManager->clearResourceGroup(pszResourceGroup);
                pResourceGroupManager->removeResourceLocation(pszName, pszResourceGroup);
            } else if (sName == sm_kapszPropertyNames[ Property_UseStaticGeom ]) {
                m_bUseStaticGeom = it->GetBool(0);
            } else if (sName == sm_kapszPropertyNames[ Property_UseInstancedGeom ]) {
                m_bUseInstancedGeom = it->GetBool(0);
            } else if (sName == sm_kapszPropertyNames[ Property_FogColor ]) {
                m_FogColor.r = it->GetColor3().r;
                m_FogColor.g = it->GetColor3().g;
                m_FogColor.b = it->GetColor3().b;
            } else if (sName == sm_kapszPropertyNames[ Property_Fog ]) {
                i32 mode = it->GetInt32(0);

                switch (mode) {
                    case 1:
                        m_FogMode = Ogre::FOG_EXP;
                        break;

                    case 2:
                        m_FogMode = Ogre::FOG_EXP2;
                        break;

                    case 3:
                        m_FogMode = Ogre::FOG_LINEAR;
                        break;

                    default:
                        m_FogMode = Ogre::FOG_NONE;
                        break;
                }

                m_ExpDensity  = it->GetFloat32(1);
                m_LinearStart = it->GetFloat32(2);
                m_LinearEnd   = it->GetFloat32(3);
            } else if (sName == sm_kapszPropertyNames[ Property_Font ]) {
                //-------------------------------------------------------------------------
                // Here is where we load the "default" font into the ogre resource system. We don't need
                // to keep any references to the resource around as it will be accessed by name by anyone
                // who needs the resource.
                //
                std::string sPath = it->GetString(0);
                std::string sFile = it->GetString(1);
                Ogre::ResourceGroupManager* pResGroupMgr = Ogre::ResourceGroupManager::getSingletonPtr();
                pResGroupMgr->addResourceLocation(sPath, "FileSystem");
                Ogre::FontManager& fontManager = Ogre::FontManager::getSingleton();
                // The first parameter here is the name of the font resource.
                Ogre::ResourcePtr font = fontManager.create("MyFont", "General");
                font->setParameter("type", "truetype");
                font->setParameter("source", sFile);
                font->setParameter("size", "24");
                font->setParameter("resolution", "96");
                font->load();
            }


            //
            // Set this property to invalid since it's already been read.
            //
            it->ClearFlag(Properties::Flags::Valid);
        }
    }

    //
    // Set some of the shadow properties even though shadows might be disabled.
    //
    m_pSceneManager->setShadowTextureCount(reinterpret_cast<GraphicSystem*>(m_pSystem)->m_uShadowTextureCount);
    m_pSceneManager->setShadowTextureSize(reinterpret_cast<GraphicSystem*>(m_pSystem)->m_uShadowTextureSize);
    m_pSceneManager->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(new Ogre::LiSPSMShadowCameraSetup()));
    m_pSceneManager->setShadowCasterRenderBackFaces(true);
    //-------------------------------------------------------------------------
    // This is where we "create" the overlay manager by retrieving its pointer. We
    // also create one overlay which is currently the only one in the system. All
    // overlay elements are currently attached to it.
    //
    m_pOverlayManager = Ogre::OverlayManager::getSingletonPtr();
    //-------------------------------------------------------------------------
    // Here we create the default overlay.
    //
    m_pOverlay = m_pOverlayManager->create("SmokeOverlay");
    m_pOverlay->show();
    //-------------------------------------------------------------------------
    // Here we set the fog properties for the scene
    //
    m_pSceneManager->setFog(m_FogMode, m_FogColor, m_ExpDensity, m_LinearStart, m_LinearEnd);

    // SKYX TEST
    SkyX::BasicController* mBasicController = new SkyX::BasicController();
    SkyX::SkyX* mSkyX = new SkyX::SkyX(m_pSceneManager, mBasicController);
    mSkyX->create();
    
    POGREROOT->addFrameListener(mSkyX);
    reinterpret_cast<GraphicSystem*>(m_pSystem)->GetOGRERenderWindow()->addListener(mSkyX);

    return Err;
}
*/


/**
 * @inheritDoc
 */
void OGREGraphicsScene::Update(f32 DeltaTime) {
    m_bPause = g_Managers.pEnvironment->Runtime().GetStatus() == IEnvironment::IRuntime::Status::Paused;
    m_fDeltaTime = DeltaTime;

    u32 size = (u32)m_Objects.size();
    if (g_Managers.pTask != NULL && UpdateGrainSize < size) {
        g_Managers.pTask->ParallelFor(m_pTask, UpdateCallback, this, 0, size, UpdateGrainSize);
    } else {
        ProcessRange(0, size);
    }
}

void OGREGraphicsScene::UpdateCallback(void* param, u32 begin, u32 end) {
    //ASSERT (dynamic_cast<OGREGraphicsScene*>(param));
    OGREGraphicsScene* pThis = static_cast<OGREGraphicsScene*>(param);
    pThis->ProcessRange(begin, end);
}

void OGREGraphicsScene::ProcessRange(u32 begin, u32 end) {
    static const u32 nonPausable = GraphicObject::Type_Camera;
    for (size_t i = begin; i < end; ++i) {
        GraphicObject* pObject = m_Objects[i];

        // Update objects based on paused state
        if (!m_bPause  || pObject->GetType() & nonPausable) {
            // Process this object
            pObject->Update(m_fDeltaTime);
        }
    }
}

/*
void OGREGraphicsScene::SetProperties(Properties::Array Properties) {
    ASSERT(m_bInitialized);

    //
    // Read in the properties.
    //
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();

            if (sName == sm_kapszPropertyNames[ Property_AmbientLight ]) {
                //
                // Set the scene's ambient light.
                //
                Ogre::ColourValue AmbientLight;
                AmbientLight.a = 1.0f;
                AmbientLight.r = it->GetColor3().r;
                AmbientLight.g = it->GetColor3().g;
                AmbientLight.b = it->GetColor3().b;
                m_pSceneManager->setAmbientLight(AmbientLight);
            } else if (sName == sm_kapszPropertyNames[ Property_Shadows ]) {
                //
                // Enable/disable shadows.
                //
                m_pSceneManager->setShadowTechnique((Ogre::ShadowTechnique)(it->GetInt32(0)));
                m_pSceneManager->setShadowTexturePixelFormat(Ogre::PF_FLOAT16_RGB);
                m_pSceneManager->setShadowTextureSelfShadow(it->GetBool(1) != 0);
            } else if (sName == sm_kapszPropertyNames[ Property_ShadowColor ]) {
                //
                // Set the scene's shadow color.
                //
                Ogre::ColourValue ShadowColor;
                ShadowColor = Ogre::ColourValue(it->GetColor3().r,
                                                it->GetColor3().g,
                                                it->GetColor3().b);
                m_pSceneManager->setShadowColour(ShadowColor);
            } else if (sName == sm_kapszPropertyNames[ Property_DrawBoundingBox ]) {
                //
                // Enable/disable the drawing of Object BoundingBoxes.
                //
                m_pSceneManager->showBoundingBoxes(it->GetBool(0) == true);
            } else {
                ASSERT(false);
            }

            //
            // Set this property to invalid since it's already been read.
            //
            it->ClearFlag(Properties::Flags::Valid);
        }
    }
}
*/


