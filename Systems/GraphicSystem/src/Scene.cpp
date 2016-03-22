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

#include <boost/functional/factory.hpp>
#include <boost/lexical_cast.hpp>
#pragma warning( push, 0 )
#include <Ogre.h>
#include <Compositor/OgreCompositorManager2.h>
#pragma warning( pop )

#include <schema/component_type_generated.h>

#include "Manager/ServiceManager.h"
#include "GraphicSystemSystem.h"
#include "Scene.h"
#include "Task.h"
#include "Object/Object.h"
#include "Object/CameraGraphicObject.h"
#include "Object/LightGraphicObject.h"
#include "Object/MeshGraphicObject.h"
#include "Object/ParticleGraphicObject.h"
#include "Object/SkyGraphicObject.h"
#include "Object/TerrainGraphicObject.h"

extern IServiceManager* g_serviceManager;

// We use SM2.0 instancing. Since we do normal mapping on the instanced geometry, we
// need to pass both the world matrix and the inverse world matrix for each instance.
// Given our limit of 256 float4 constants, we set the Objects per batch to 40.
// 40 float3x3 world matrices           = 120 float3/4 constants
// 40 float3x3 world inverse matrices   = 120 float3/4 constants
// we have used 240 float4 constants. The rest 16 are saved to pass other information
// to the instancing shader.
static const unsigned int maxObjectsPerBatch = 40;
static const unsigned int UpdateGrainSize = 120;

// Prototypes
void ProcessObjects(void* Data);

/**
 * @inheritDoc
 */
GraphicScene::GraphicScene(ISystem* pSystem, const Schema::SystemScene* systemScene)
    : ISystemScene(pSystem, systemScene)
    , m_pSceneManager(nullptr)
    , m_FogMode(Ogre::FOG_NONE) {
    m_pSceneManager = GetSystem<GraphicSystem>()->getRoot()->createSceneManager(Ogre::ST_GENERIC, 4, Ogre::INSTANCING_CULLING_THREADED);
    m_pRootNode = m_pSceneManager->getRootSceneNode();
    // TODO ambient light in scene config
    m_ambientLight = Ogre::ColourValue(1, 1, 1, 0.3);
    m_pSceneManager->setAmbientLight(m_ambientLight);
    m_pSceneManager->setSkyBox(true, "nebula");
    m_pSceneManager->addRenderQueueListener(GetSystem<GraphicSystem>()->getOverlaySystem());

    //m_propertySetters["AmbientLight"] = boost::bind(&GraphicScene::setAmbientLight, this, _1);

    m_ObjectFactories[Schema::ComponentType::GraphicCamera] = boost::factory<CameraGraphicObject*>();
    m_ObjectFactories[Schema::ComponentType::GraphicMesh] = boost::factory<MeshGraphicObject*>();
    /*m_ObjectFactories["Gui"] = boost::factory<GuiGraphicObject*>();
    m_ObjectFactories["Light"] = boost::factory<LightGraphicObject*>();
    m_ObjectFactories["Particle"] = boost::factory<ParticleGraphicObject*>();
    m_ObjectFactories["Sky"] = boost::factory<SkyGraphicObject*>();
    m_ObjectFactories["Terrain"] = boost::factory<TerrainGraphicObject*>();*/

    Ogre::Vector3 lightdir(0, 0, 0);

    // TODO light in scene config
    Ogre::SceneNode* node = m_pSceneManager->createSceneNode();
    Ogre::Light* light = m_pSceneManager->createLight();
    node->setName("TestLight");
    node->attachObject(light);
    node->setPosition(15, 15, 0);
    light->setType(Ogre::Light::LT_SPOTLIGHT);
    light->setDirection(lightdir);
    light->setDiffuseColour(Ogre::ColourValue::Red);
    light->setSpecularColour(Ogre::ColourValue(0.4, 0.4, 0.4));
}

/**
 * @inheritDoc
 */
GraphicScene::~GraphicScene() {
    GetSystem<GraphicSystem>()->getRoot()->destroySceneManager(m_pSceneManager);
}

/**
 * @inheritDoc
 */
void GraphicScene::Update(float DeltaTime) {
    m_fDeltaTime = DeltaTime;
    unsigned int size = (unsigned int)m_pObjects.size();
    if (g_serviceManager->getTaskManager() != NULL && UpdateGrainSize < size) {
        // TODO Use a struct for params with DeltaTime, GraphicScene
        g_serviceManager->getTaskManager()->ParallelFor(m_pSystemTask, UpdateCallback, this, 0, size, UpdateGrainSize);
    } else {
        ProcessRange(0, size);
    }
}

/**
 * @inheritDoc
 */
void GraphicScene::UpdateCallback(void* param, unsigned int begin, unsigned int end) {
    GraphicScene* pThis = static_cast<GraphicScene*>(param);
    pThis->ProcessRange(begin, end);
}

/**
 * @inheritDoc
 */
void GraphicScene::ProcessRange(unsigned int begin, unsigned int end) {
    auto start = m_pObjects.begin() + begin;
    for (auto iterator = start; iterator < start + end; iterator++) {
        GraphicObject* pObject = static_cast<GraphicObject*>(iterator->second);
        pObject->Update(m_fDeltaTime);
    }
}

void GraphicScene::createTask() {
    m_pSystemTask = new GraphicTask(this);
}
