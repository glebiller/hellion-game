//
// extern includes
//
#pragma warning( push, 0 )
#pragma warning( disable : 6326 6385 )
#include "Ogre.h"
#pragma warning( pop )

//
// Core includes
//
#include "BaseTypes.h"
#include "Interface.h"

//
// Graphic system includes
//
#include "System.h"
#include "Scene.h"
#include "Object.h"
#include "ObjectTerrain.h"

#define POGRESCENEMGR (reinterpret_cast<OGREGraphicsScene*>(m_pSystemScene)->GetOGRESceneManager())


GraphicObjectTerrain::GraphicObjectTerrain(
    ISystemScene* pSystemScene,
    const char* pszName
)
    : GraphicObject(pSystemScene, pszName),
      mTerrainGlobals(0),
      mTerrainGroup(0),
      mTerrainsImported(false) {
    m_Type = GraphicObject::Type_Terrain;
}

GraphicObjectTerrain::~GraphicObjectTerrain(
    void
) {
    OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainGlobals;
}

Error
GraphicObjectTerrain::Initialize(
    std::vector<Properties::Property> Properties
) {
    Error Err = Errors::Success;
    ASSERT(!m_bInitialized);
    //
    // Call the base class.
    //
    GraphicObject::Initialize(Properties);
    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);
    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(POGRESCENEMGR, Ogre::Terrain::ALIGN_X_Z, 257, 1024.0f);
    mTerrainGroup->setFilenameConvention(Ogre::String("MainTerrain"), Ogre::String("dat"));
    mTerrainGroup->setOrigin(Ogre::Vector3::ZERO);
    configureTerrainDefaults();

    for (long x = 0; x <= 0; ++x) {
        for (long y = 0; y <= 0; ++y) {
            defineTerrain(x, y);
        }
    }

    // sync load since we want everything in place when we start
    mTerrainGroup->loadAllTerrains(true);

    if (mTerrainsImported) {
        Ogre::TerrainGroup::TerrainIterator ti = mTerrainGroup->getTerrainIterator();

        while (ti.hasMoreElements()) {
            Ogre::Terrain* t = ti.getNext()->instance;
            initBlendMaps(t);
        }
    }

    mTerrainGroup->freeTemporaryResources();
    return Err;
}

void
GraphicObjectTerrain::SetProperties(
    Properties::Array Properties
) {
    ASSERT(m_bInitialized);
}

void
GraphicObjectTerrain::GetProperties(
    Properties::Array& Properties
) {
}

void
GraphicObjectTerrain::Update(f32 DeltaTime) {
    UNREFERENCED_PARAM(DeltaTime);
    //
    // Save the terrain to binary data if needed
    //
    /*if (!mTerrainGroup->isDerivedDataUpdateInProgress())
    {
        if (mTerrainsImported)
        {
            mTerrainGroup->saveAllTerrains(true);
            mTerrainsImported = false;
        }
    }*/
}


void
GraphicObjectTerrain::configureTerrainDefaults() {
    Ogre::Light* light = POGRESCENEMGR->getLight("Sun");
    // Configure global
    mTerrainGlobals->setMaxPixelError(8);
    // testing composite map
    mTerrainGlobals->setCompositeMapDistance(3000);
    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
    mTerrainGlobals->setCompositeMapAmbient(POGRESCENEMGR->getAmbientLight());
    // Configure default import settings for if we use imported image
    Ogre::Terrain::ImportData& defaultimp = mTerrainGroup->getDefaultImportSettings();
    defaultimp.terrainSize = 257;
    defaultimp.worldSize = 1024.0f;
    defaultimp.inputScale = 100;
    defaultimp.minBatchSize = 33;
    defaultimp.maxBatchSize = 65;
    // textures
    defaultimp.layerList.resize(3);
    defaultimp.layerList[0].worldSize = 10;
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 5;
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 20;
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("growth_weirdfungus-03_normalheight.dds");
}

void GraphicObjectTerrain::defineTerrain(long x, long y) {
    Ogre::String filename = mTerrainGroup->generateFilename(x, y);

    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(mTerrainGroup->getResourceGroup(), filename)) {
        mTerrainGroup->defineTerrain(x, y);
    } else {
        Ogre::Image img;
        img.load("terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

        if (x % 2 != 0) {
            img.flipAroundY();
        }

        if (y % 2 != 0) {
            img.flipAroundX();
        }

        mTerrainGroup->defineTerrain(x, y, &img);
        mTerrainsImported = true;
    }
}

void GraphicObjectTerrain::initBlendMaps(Ogre::Terrain* terrain) {
    Ogre::TerrainLayerBlendMap* blendMap0 = terrain->getLayerBlendMap(1);
    Ogre::TerrainLayerBlendMap* blendMap1 = terrain->getLayerBlendMap(2);
    Ogre::Real minHeight0 = 50;
    Ogre::Real fadeDist0 = 15;
    Ogre::Real minHeight1 = 10;
    Ogre::Real fadeDist1 = 5;
    float* pBlend0 = blendMap0->getBlendPointer();
    float* pBlend1 = blendMap1->getBlendPointer();

    for (Ogre::uint16 y = 0; y < terrain->getLayerBlendMapSize(); ++y) {
        for (Ogre::uint16 x = 0; x < terrain->getLayerBlendMapSize(); ++x) {
            Ogre::Real tx, ty;
            blendMap0->convertImageToTerrainSpace(x, y, &tx, &ty);
            Ogre::Real height = terrain->getHeightAtTerrainPosition(tx, ty);
            Ogre::Real val = (height - minHeight0) / fadeDist0;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend0++ = val;
            val = (height - minHeight1) / fadeDist1;
            val = Ogre::Math::Clamp(val, (Ogre::Real)0, (Ogre::Real)1);
            *pBlend1++ = val;
        }
    }

    blendMap0->dirty();
    blendMap1->dirty();
    blendMap0->update();
    blendMap1->update();
}
