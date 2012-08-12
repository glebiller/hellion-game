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

#include "OgreWindowEventUtilities.h"

#include "Errors.h"
#include "System.h"
#include "System/ISystem.h"

class OGREGraphicsScene;
class OGREGraphicsTask;

///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   Implementation of the ISystem interface for graphics.
///   See Interfaces\System.h for a definition of the class and its functions.
/// </summary>
///////////////////////////////////////////////////////////////////////////////

class GraphicSystem : public ISystem, public Ogre::WindowEventListener {
        friend OGREGraphicsScene;

    public:

        GraphicSystem(void);
        virtual ~GraphicSystem(void);

        /**
         * Gets the ogre root.
         * This function returns a pointer to the interanl Ogre root.
         *
         * @return  Ogre::Root* - A pointer to the Ogre root.
         */
        Ogre::Root* GetOGRERoot(void) {
            return m_pRoot;
        }

        /**
         * Gets the ogre render window.
         * This function returns a pointer to the interanl Ogre render window.
         *
         * @return  Ogre::RenderWindow* - A pointer to the Ogre render window.
         */
        Ogre::RenderWindow* GetOGRERenderWindow(void) {
            return m_pRenderWindow;
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
         * Gets the system type.
         * Implementation of the <c>ISystem::GetSystemType</c> function. Gets the system type for this
         * system.
         *
         * @return  System::Type - The type of the system.
         *
         * @sa   ISystem::GetSystemType  .
         */
        virtual System::Type GetSystemType(void);

        /**
         * Initializes this :.
         * Implementation of the <c>ISystem::Initialize</c> function. One time initialization function
         * for the system.
         *
         * @param   Properties  Property structure array to initialize.
         * @return  Error - Any error codes.
         *
         * @sa   ISystem::Initialize .
         */
        virtual Error Initialize(Properties::Array Properties);

        /**
         * Gets the properties.
         * Implementation of the <c>ISystem::GetProperties</c> function. Gets the properties of this
         * system.
         *
         * @param   Properties  Property structure array to fill.
         *
         * @sa   ISystem::GetProperties  .
         */
        virtual void GetProperties(Properties::Array& Properties);

        /**
         * Sets the properties.
         * Implementation of the <c>ISystem::SetProperties</c> function. Sets the properties for this
         * system.
         *
         * @param   Properties  Properties to set in the system.
         *
         * @sa   ISystem::SetProperties  .
         */
        virtual void SetProperties(Properties::Array Properties);

        /**
         * Creates the scene.
         * Implementation of the <c>ISystem::CreateScene</c> function. Creates a system scene for
         * containing system objects.
         *
         * @return  ISystemScene* - The newly create system scene.
         *
         * @sa   ISystem::CreateScene    .
         */
        virtual ISystemScene* CreateScene(void);

        /**
         * Destroys the scene described by pSystemScene.
         * Implementation of the <c>ISystem::DestroyScene</c> function. Destroys a system scene.
         *
         * @param   pSystemScene    The scene to destroy. Any objects within are destroyed.
         * @return  Error - Any error codes.
         *
         * @sa   ISystem::DestroyScene   .
         */
        virtual Error DestroyScene(ISystemScene* pSystemScene);


    protected:

        enum PropertyTypes {
            Property_ResourceLocation,
            Property_WindowName,
            Property_Resolution,
            Property_ShadowTexture,
            Property_FullScreen,
            Property_VerticalSync,
            Property_FSAntiAliasing,
            Property_Count
        };

        static const char*                  sm_kapszPropertyNames[];
        static const Properties::Property   sm_kaDefaultProperties[];

        Ogre::Root*                         m_pRoot;

        Ogre::ResourceGroupManager*         m_pResourceGroupManager;
        Ogre::MaterialManager*              m_pMaterialManager;

        Ogre::RenderSystem*                 m_pRenderSystem;
        Ogre::RenderWindow*                 m_pRenderWindow;

        u16                                 m_uShadowTextureCount;
        u16                                 m_uShadowTextureSize;
};

