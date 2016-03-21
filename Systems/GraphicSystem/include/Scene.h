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

#pragma warning( push, 0 )

#include <OgreOverlaySystem.h>

#pragma warning( pop )

#include "DataTypes.h"
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
class GraphicScene : public ISystemScene {

public:

    /**
     * @inheritDoc
     */
    GraphicScene(ISystem* pSystem, const Schema::SystemScene* systemScene);

    /**
     * @inheritDoc
     */
    ~GraphicScene();

    /**
     * @inheritDoc
     */
    void Update(float DeltaTime) override;

    /**
     * @inheritDoc
     */
    System::Changes::BitMask GetDesiredSystemChanges() override {
        return System::Changes::None;
    };

    Ogre::SceneManager* getSceneManager() {
        return m_pSceneManager;
    };

    Ogre::SceneNode* getRootNode() {
        return m_pRootNode;
    };


    virtual void createTask() override;

protected:
    Ogre::SceneManager* m_pSceneManager;
    Ogre::SceneNode* m_pRootNode;

    Ogre::ColourValue m_ambientLight;
    Ogre::ColourValue m_FogColor;
    Ogre::FogMode m_FogMode;
    float m_ExpDensity;
    float m_LinearStart;
    float m_LinearEnd;

    bool m_bPause;
    float m_fDeltaTime;

private:

    /**
     * Invoked by ParalellFor algorithm to update a range of objects.
     */
    static void UpdateCallback(void* param, unsigned int begin, unsigned int end);

    /**
     * Updates the given range of fire objects.
     */
    void ProcessRange(unsigned int begin, unsigned int end);

};

