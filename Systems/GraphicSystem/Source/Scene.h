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
#include "Task.h"

class GraphicSystem;
class GraphicObject;

// Macro to convert from Math::Vector3 to Ogre::Vector3
#define TOOGREVEC(Vec)  (Ogre::Vector3((Vec).x, (Vec).y, (Vec).z) )
// Macro to convert from Math::Quaternion to Ogre::Quaternion
#define TOOGREQUAT(Vec) (Ogre::Quaternion((Vec).w, (Vec).x, (Vec).y, (Vec).z) )

#define MAX_NUM_JOBS 32


/**
 * Implementation of the ISystemScene interface. See Interfaces\System.h for a definition of the
 * class and its functions.
 * 
 * @sa  ISystemScene
 */
class OGREGraphicsScene : public ISystemScene {

    public:
        
        typedef std::vector<GraphicObject*>    ObjectsList;

        DECLARE_STATIC_SPIN_MUTEX(m_mutex);
        
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
        
        /// <summary cref="OGREGraphicsScene::Update">
        ///   This function returns a pointer to the interanl Ogre scene manager.
        /// </summary>
        /// <returns>Ogre::SceneManager* - A pointer to the Ogre scene manager.</returns>
        Ogre::SceneManager* GetOGRESceneManager(void) {
            return m_pSceneManager;
        };

        /// <summary cref="OGREGraphicsScene::GetOGRERootSceneNode">
        ///   This function returns a pointer to the interanl Ogre scene root node.
        /// </summary>
        /// <returns>Ogre::SceneNode* - A pointer to the Ogre scene root node.</returns>
        Ogre::SceneNode* GetOGRERootSceneNode(void) {
            return m_pRootNode;
        };

        /// <summary cref="OGREGraphicsScene::GetOGREOverlayManager">
        ///   This function returns a pointer to the interanl Ogre overlay manager.
        /// </summary>
        /// <returns>Ogre::OverlayManager* - A pointer to the Ogre overlay manager.</returns
        Ogre::OverlayManager* GetOGREOverlayManager(void) {
            return m_pOverlayManager;
        };

        /// <summary cref="OGREGraphicsScene::GetDefaultOverlay">
        ///   This function returns a pointer to the default overlay.
        /// </summary>
        /// <returns>Ogre::Overlay* - A pointer to the default overlay.</returns
        Ogre::Overlay* GetDefaultOverlay(void) {
            return m_pOverlay;
        };

    protected:

        OGREGraphicsTask*                   m_pTask;
        
        ObjectsList                         m_Objects;

        Ogre::SceneManager*                 m_pSceneManager;
        Ogre::SceneNode*                    m_pRootNode;

        Ogre::OverlayManager*               m_pOverlayManager;
        Ogre::Overlay*                      m_pOverlay;

        Ogre::ColourValue                   m_FogColor;
        Ogre::FogMode                       m_FogMode;
        float                               m_ExpDensity;
        float                               m_LinearStart;
        float                               m_LinearEnd;

        bool                                m_bPause;
        f32                                 m_fDeltaTime;

    private:

        /**
         * Invoked by ParalellFor algorithm to update a range of objects.
         */
        static void UpdateCallback(void* param, u32 begin, u32 end);

        /**
         * Updates the given range of fire objects.
         */
        void ProcessRange(u32 begin, u32 end);

};

