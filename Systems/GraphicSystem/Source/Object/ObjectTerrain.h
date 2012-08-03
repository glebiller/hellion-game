#pragma once

//
// Ogre includes
//
#include "Terrain/OgreTerrain.h"
#include "Terrain/OgreTerrainGroup.h"

class GraphicSystem;
class OGREGraphicsScene;

class GraphicObjectTerrain : public GraphicObject {
        friend GraphicSystem;
        friend OGREGraphicsScene;
        friend OGREGraphicsTask;

    protected:
        GraphicObjectTerrain(ISystemScene* pSystemScene, pcstr pszName);
        ~GraphicObjectTerrain(void);

        /// <summary cref="GraphicObjectCamera::Initialize">
        ///   Implementation of the <c>ISystem::Initialize</c> function.
        /// </summary>
        /// <param name="Properties">Initializes the object with the properties specified by <paramref name="Properties"/>.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObject::Initialize"/>
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /// <summary cref="GraphicObjectCamera::GetProperties">
        ///   Implementation of the <c>ISystem::GetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Gets the properties of the object</param>
        /// <seealso cref="ISystem::GetProperties"/>
        virtual void GetProperties(Properties::Array& Properties);

        /// <summary cref="GraphicObjectCamera::SetProperties">
        ///   Implementation of the <c>ISystem::SetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Sets the properties of the object</param>
        /// <seealso cref="ISystem::SetProperties"/>
        virtual void SetProperties(Properties::Array Properties);

        /// <summary cref="GraphicObjectCamera::Update">
        ///   Implementation of the <c>ISystemTask::Update</c> method.
        /// </summary>
        /// <param name="DeltaTime">Elapsed time since the last frame.</param>
        /// <seealso cref="GraphicObject::Update"/>
        void Update(f32 DeltaTime);

    private:

        void defineTerrain(long x, long y);
        void initBlendMaps(Ogre::Terrain* terrain);
        void configureTerrainDefaults();

    private:
        Ogre::TerrainGlobalOptions* mTerrainGlobals;
        Ogre::TerrainGroup* mTerrainGroup;

        bool mTerrainsImported;

};