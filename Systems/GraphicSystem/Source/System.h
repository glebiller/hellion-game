// Copyright © 2008-2009 Intel Corporation
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
#include <OgreWindowEventUtilities.h>
#include <OgreRenderWindow.h>
#include <Overlay/OgreOverlaySystem.h>
#pragma warning( pop )

#include "Errors.h"
#include "System.h" 
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
class GraphicSystem : public ISystem, public Ogre::WindowEventListener {
public:

    /**
     * @inheritDoc
     */
    GraphicSystem(void);

    /**
     * @inheritDoc
     */
    ~GraphicSystem(void);

    /**
     * @inheritDoc
     */
    Error initialize(void);

    /**
     * @inheritDoc
     */
    inline Proto::SystemType GetSystemType(void) {
        return Proto::SystemType::Graphic;
    }

    /**
     * Gets the ogre root.
     * This function returns a pointer to the interanl Ogre root.
     *
     * @return  Ogre::Root* - A pointer to the Ogre root.
     */
    inline Ogre::Root* getRoot(void) {
        return m_pRoot;
    }

    /**
     * Gets the ogre render window.
     * This function returns a pointer to the interanl Ogre render window.
     *
     * @return  Ogre::RenderWindow* - A pointer to the Ogre render window.
     */
    inline Ogre::RenderWindow* getRenderWindow(void) {
        return m_pRenderWindow;
    }

    /**
     * Gets overlay system.
     *
     * @return  null if it fails, else the overlay system.
     */
    inline Ogre::OverlaySystem* getOverlaySystem(void) {
        return m_pOverlaySystem;
    }

    /**
     * Window closed.
     * This function is called when the app window is close (to signal that the app is shutting
     * down).
     *
     * @param   pRenderWindow   Ogre::RenderWindow* - A pointer to Ogre render window that closed.
     */
    void windowClosed(Ogre::RenderWindow* pRenderWindow);

protected:
    
    /**
     * Sets the resources location.
     *
     * @param   values  The values.
     */
    void setResourceLocation(Proto::RepeatedString* values);

    /**
     * Sets the window name.
     *
     * @param   values  The values.
     */
    void setWindowName(Proto::RepeatedString* values);

    /**
     * Sets the resolution.
     *
     * @param   values  The values.
     */
    void setResolution(Proto::RepeatedString* values);

    /**
     * Sets the fullscreen.
     *
     * @param   values  The values.
     */
    void setFullScreen(Proto::RepeatedString* values);

    /**
     * Sets the vertical sync.
     *
     * @param   values  The values.
     */
    void setVerticalSync(Proto::RepeatedString* values);

    /**
     * Sets the anti aliasing.
     *
     * @param   values  The values.
     */
    void setAntiAliasing(Proto::RepeatedString* values);

private:
    Ogre::Root*                         m_pRoot;

    Ogre::ResourceGroupManager*         m_pResourceGroupManager;
    Ogre::MaterialManager*              m_pMaterialManager;
    Ogre::RenderSystem*                 m_pRenderSystem;
    Ogre::OverlaySystem*                m_pOverlaySystem;

    Ogre::RenderWindowDescription       m_RenderWindowDescription;
    Ogre::RenderWindow*                 m_pRenderWindow;

};
