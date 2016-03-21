// Copyright 2008-2009 Intel Corporation
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

#include "Object/TerrainGraphicObject.h"

#pragma warning( push, 0 )
#pragma warning( disable : 6326 6385 )
#include <OgreTerrain.h>
#include <OgreTerrainGroup.h>
#pragma warning( pop )

#include "Scene.h"
#include "Object/Object.h"

///
/// @inheritDoc.
///
TerrainGraphicObject::TerrainGraphicObject(ISystemScene* pSystemScene, UObject* entity,
                                           const Schema::SystemComponent& component)
    : GraphicObject(pSystemScene, entity, component),
      mTerrainGlobals(0),
      mTerrainGroup(0),
      mTerrainsImported(false) {
    //m_propertySetters["Terrain"] = boost::bind(&TerrainGraphicObject::setTerrain, this, _1);
}

///
/// @inheritDoc.
///
TerrainGraphicObject::~TerrainGraphicObject() {
    OGRE_DELETE mTerrainGroup;
    OGRE_DELETE mTerrainGlobals;
}

///
/// @inheritDoc.
///
Error TerrainGraphicObject::initialize() {
    Error Err = Errors::Success;


    Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_ANISOTROPIC);
    Ogre::MaterialManager::getSingleton().setDefaultAnisotropy(7);

    mTerrainGlobals = OGRE_NEW Ogre::TerrainGlobalOptions();
    mTerrainGroup = OGRE_NEW Ogre::TerrainGroup(GetSystemScene<GraphicScene>()->getSceneManager(), Ogre::Terrain::ALIGN_X_Z, 257, 1024.0f);
    mTerrainGroup->setFilenameConvention(Ogre::String("terrain/MainTerrain"), Ogre::String("dat"));
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

///
/// @inheritDoc.
///
void TerrainGraphicObject::Update(float DeltaTime) {
    if (!mTerrainGroup->isDerivedDataUpdateInProgress()) {
        if (mTerrainsImported) {
            mTerrainGroup->saveAllTerrains(false);
            mTerrainsImported = false;
        }
    }
}

///
/// @inheritDoc.
///
Error TerrainGraphicObject::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {


    return Errors::Success;
}


void TerrainGraphicObject::configureTerrainDefaults() {
    //Ogre::Light* light = GetSystemScene<GraphicScene>()->getSceneManager()->getAmbientLight();
    // Configure global
    mTerrainGlobals->setMaxPixelError(8);
    // testing composite map
    mTerrainGlobals->setCompositeMapDistance(3000);
    // Important to set these so that the terrain knows what to use for derived (non-realtime) data
    //mTerrainGlobals->setLightMapDirection(light->getDerivedDirection());
    //mTerrainGlobals->setCompositeMapDiffuse(light->getDiffuseColour());
    mTerrainGlobals->setCompositeMapAmbient(GetSystemScene<GraphicScene>()->getSceneManager()->getAmbientLight());
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
    defaultimp.layerList[0].textureNames.push_back("terrain/dirt_grayrocky_diffusespecular.dds");
    defaultimp.layerList[0].textureNames.push_back("terrain/dirt_grayrocky_normalheight.dds");
    defaultimp.layerList[1].worldSize = 5;
    defaultimp.layerList[1].textureNames.push_back("terrain/grass_green-01_diffusespecular.dds");
    defaultimp.layerList[1].textureNames.push_back("terrain/grass_green-01_normalheight.dds");
    defaultimp.layerList[2].worldSize = 20;
    defaultimp.layerList[2].textureNames.push_back("terrain/growth_weirdfungus-03_diffusespecular.dds");
    defaultimp.layerList[2].textureNames.push_back("terrain/growth_weirdfungus-03_normalheight.dds");
}

void TerrainGraphicObject::defineTerrain(long x, long y) {
    Ogre::String filename = mTerrainGroup->generateFilename(x, y);
    Ogre::String group = mTerrainGroup->getResourceGroup();
    if (Ogre::ResourceGroupManager::getSingleton().resourceExists(group, filename)) {
        mTerrainGroup->defineTerrain(x, y);
    } else {
        Ogre::Image img;
        img.load("terrain/terrain.png", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

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

void TerrainGraphicObject::initBlendMaps(Ogre::Terrain* terrain) {
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
