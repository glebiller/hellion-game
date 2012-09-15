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


#pragma once

#include "SpinMutex.h"

// Temporarily switching warning level to 0 to ignore warnings in extern/Ogre
#pragma warning( push, 0 )
#include "PagedGeometry.h"
#include "GrassLoader.h"
#pragma warning( pop )


class GraphicSystem;
class OGREGraphicsTask;
class GraphicObject;

// Macro to convert from Math::Vector3 to Ogre::Vector3
#define TOOGREVEC(Vec)  (Ogre::Vector3((Vec).x, (Vec).y, (Vec).z) )
// Macro to convert from Math::Quaternion to Ogre::Quaternion
#define TOOGREQUAT(Vec) (Ogre::Quaternion((Vec).w, (Vec).x, (Vec).y, (Vec).z) )

#define MAX_NUM_JOBS 32

struct ProcessData {
    std::list<GraphicObject*>   m_Objects;    // Objects to process
    f32                         m_DeltaTime;  // Delta time (needed for Update calls)
};


/**
 * Implementation of the ISystemScene interface. See Interfaces\System.h for a definition of the
 * class and its functions.
 * 
 * @sa  ISystemScene
 */
class OGREGraphicsScene : public ISystemScene {
        friend GraphicSystem;
        friend OGREGraphicsTask;

    public:

    public:
        
        /**
         * Updates the given DeltaTime.
         * This function must be called every frame.  It updates the graphics scene.
         *
         * @param   DeltaTime   Elapsed time since the last frame.
         *
         * @sa   ISystemTask::Update .
         */
        virtual void Update(f32 DeltaTime);

        /// <summary cref="OGREGraphicsScene::Update">
        ///   This function returns a pointer to the interanl Ogre scene manager.
        /// </summary>
        /// <returns>Ogre::SceneManager* - A pointer to the Ogre scene manager.</returns>
        Ogre::SceneManager* GetOGRESceneManager(void) {
            return m_pSceneManager;
        }

        /// <summary cref="OGREGraphicsScene::GetOGRERootSceneNode">
        ///   This function returns a pointer to the interanl Ogre scene root node.
        /// </summary>
        /// <returns>Ogre::SceneNode* - A pointer to the Ogre scene root node.</returns>
        Ogre::SceneNode* GetOGRERootSceneNode(void) {
            return m_pRootNode;
        }

        /// <summary cref="OGREGraphicsScene::GetOGREOverlayManager">
        ///   This function returns a pointer to the interanl Ogre overlay manager.
        /// </summary>
        /// <returns>Ogre::OverlayManager* - A pointer to the Ogre overlay manager.</returns
        Ogre::OverlayManager* GetOGREOverlayManager(void) {
            return m_pOverlayManager;
        }

        /// <summary cref="OGREGraphicsScene::GetDefaultOverlay">
        ///   This function returns a pointer to the default overlay.
        /// </summary>
        /// <returns>Ogre::Overlay* - A pointer to the default overlay.</returns
        Ogre::Overlay* GetDefaultOverlay(void) {
            return m_pOverlay;
        }

        /// <summary cref="OGREGraphicsScene::GetGrassLoader">
        ///   This function returns a pointer to the interanl paged geometry grass loader.
        /// </summary>
        /// <returns>Forests::GrassLoader* - A pointer to the interal grass loader.</returns
        Forests::GrassLoader* GetGrassLoader(void) {
            return m_pGrassLoader;
        }

        /// <summary cref="OGREGraphicsScene::SetCamera">
        ///   This function sets all the internal data for the given camera.
        /// </summary>
        /// <param name="pCamera">Ogre::Camera* - A pointer to the new camera.</param>
        void SetCamera(Ogre::Camera* pCamera) {
            m_pPagedGeometry->setCamera(pCamera);
        }

        /// <summary cref="OGREGraphicsScene::SetDetailLevel">
        ///   Updates the internal setting for detail level.
        ///   Note: The detail level must be set after the camera and other settings
        /// </summary>
        void SetDetailLevel(void) {
            m_pPagedGeometry->addDetailLevel<Forests::GrassPage>(m_fDrawDistance);
        }

        /// <summary cref="OGREGraphicsScene::getTerrainHeightScene">
        ///   Gets the height from the height map at the given a,b offset.
        /// </summary>
        /// <param name="a">const float - Horizontal offset into height map.</param>
        /// <param name="b">const float - Vertical offset into height map.</param>
        /// <param name="userData">void* - Pointer to user data used for the height lookup.</param>
        /// <returns>float - Height for the give params in scene units.</returns
        float getTerrainHeightScene(const float a, const float b, void* userData);

    public:
        
        /**
         * @inheritDoc
         */
        OGREGraphicsScene(ISystem* pSystem);
        
        /**
         * @inheritDoc
         */
        ~OGREGraphicsScene(void);
                
        /**
         * @inheritDoc
         */
        Error initialize(void);
        
        /**
         * @inheritDoc
         */
        void Update(f32 DeltaTime);
        
        /**
         * @inheritDoc
         */
        System::Changes::BitMask GetPotentialSystemChanges(void) {
            return System::Changes::None;
        };

        /**
         * @inheritDoc
         */
        System::Changes::BitMask GetDesiredSystemChanges(void) {
            return System::Changes::None;
        };
        
        /**
         * @inheritDoc
         */
        System::Type GetSystemType(void) {
            return System::Types::Graphic;
        };
        
        /**
         * @inheritDoc
         */
        ISystemTask* GetSystemTask(void) {
            return m_pTask;
        };

        typedef std::vector<GraphicObject*>    ObjectsList;

        DECLARE_STATIC_SPIN_MUTEX(m_mutex);

    protected:

        ObjectsList                         m_Objects;
        std::map<std::string, Ogre::StaticGeometry*>    m_StaticGeoms;
        std::map<std::string, std::vector<Ogre::InstancedGeometry*> > m_InstancedGeoms;

        OGREGraphicsTask*                   m_pTask;

        Ogre::SceneManager*                 m_pSceneManager;
        Ogre::SceneNode*                    m_pRootNode;

        Ogre::OverlayManager*               m_pOverlayManager;
        Ogre::Overlay*                      m_pOverlay;

        bool                                m_bUseStaticGeom;
        bool                                m_bUseInstancedGeom;

        ProcessData                         m_ProcessData[ MAX_NUM_JOBS ];

        Ogre::ColourValue                   m_FogColor;
        Ogre::FogMode                       m_FogMode;
        float                               m_ExpDensity;
        float                               m_LinearStart;
        float                               m_LinearEnd;

        //
        // PagedGeometry
        //
        Forests::PagedGeometry*             m_pPagedGeometry;
        Forests::GrassLoader*               m_pGrassLoader;
        Ogre::Image                         m_HeightMapImage;
        std::string                         m_sHeightmap;
        std::string                         m_sResourceGroup;
        f32                                 m_fPageSize;
        f32                                 m_fDrawDistance;
        f32                                 m_fTerrainWidth;
        f32                                 m_fTerrainLength;
        f32                                 m_fTerrainHeight;
        f32                                 m_fTerrainWidthOffset;
        f32                                 m_fTerrainLengthOffset;
        f32                                 m_fTerrainHeightOffset;

        bool    m_bPause;
        f32     m_fDeltaTime;

        /// <summary cref="OGREGraphicsScene::UpdateCallback">
        ///   Invoked by ParalellFor algorithm to update a range of objects.
        /// </summary>
        static void UpdateCallback(void* param, u32 begin, u32 end);

        /// <summary cref="OGREGraphicsScene::UpdateCallback">
        ///   Updates the given range of fire objects.
        /// </summary>
        void ProcessRange(u32 begin, u32 end);

};

