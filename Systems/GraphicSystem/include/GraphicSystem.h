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

#include <boost/log/sources/logger.hpp>
#include <boost/dll.hpp>

#pragma warning( push, 0 )

#include <OgreWindowEventUtilities.h>
#include <OgreRenderWindow.h>
#include <OgreOverlaySystem.h>
#include <OgreLog.h>

#pragma warning( pop )

#include <schema/graphic_system_generated.h>
#include <Generic/Framework.h>
#include "Errors.h"
#include "GraphicSystem.h"
#include "System/ISystem.h"

class GraphicScene;

class GraphicTask;

/**
 * Implementation of the ISystem interface for graphics. See Interfaces\System.h for a
 * definition of the class and its functions.
 *
 * @sa  ISystem
 * @sa  Ogre::WindowEventListener
 */
class GraphicSystem : public ISystem, public Ogre::LogListener, public Ogre::WindowEventListener {
public:

    /**
     * @inheritDoc
     */
    GraphicSystem(Framework* framework);

    /**
     * @inheritDoc
     */
    ~GraphicSystem();

    ISystemScene* createScene(const Schema::SystemScene* pScene) override;

    inline Schema::SystemType GetSystemType() override {
        return Schema::SystemType::Graphic;
    }

    /**
     * Gets the ogre root.
     * This function returns a pointer to the interanl Ogre root.
     *
     * @return  Ogre::Root* - A pointer to the Ogre root.
     */
    inline Ogre::Root* getRoot() {
        return m_pRoot;
    }

    /**
     * Gets the ogre render window.
     * This function returns a pointer to the interanl Ogre render window.
     *
     * @return  Ogre::RenderWindow* - A pointer to the Ogre render window.
     */
    inline Ogre::RenderWindow* getRenderWindow() {
        return m_pRenderWindow;
    }

    /**
     * Gets overlay system.
     *
     * @return  null if it fails, else the overlay system.
     */
    inline Ogre::OverlaySystem* getOverlaySystem() {
        return m_pOverlaySystem;
    }

    void messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug,
                       const Ogre::String& logName, bool& skipThisMessage) override;

    void windowClosed(Ogre::RenderWindow* pRenderWindow) override;

    Framework* getFramework() {
        return framework_;
    }

private:
    boost::log::sources::logger logger_;
    Framework* framework_;

    std::string definitionData_;
    const Schema::Systems::GraphicSystem* definition_;

    Ogre::Root* m_pRoot;
    Ogre::RenderWindow* m_pRenderWindow;
    Ogre::ResourceGroupManager* m_pResourceGroupManager;
    Ogre::MaterialManager* m_pMaterialManager;
    Ogre::OverlaySystem* m_pOverlaySystem;

};
