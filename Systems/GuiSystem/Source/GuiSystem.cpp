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

#include "CEGUI.h"
#include "RendererModules/Ogre/CEGUIOgreRenderer.h"
#include "boost/functional/factory.hpp"
#include "boost/bind.hpp"

#include "Defines.h"
#include "BaseTypes.h"
#include "Interface.h"

#include "GuiSystem.h"
#include "GuiScene.h"


extern ManagerInterfaces    g_Managers;


/**
 * @inheritDoc
 */
GuiSystem::GuiSystem(void) : ISystem() {
    //
    // Init the Scene factory
    //
    m_SceneFactory = boost::factory<GuiScene*>();

    //
    // Fill the properties default values
    //
    m_propertySetters["Imageset"] = boost::bind(&GuiSystem::setImagesetResourceGroup, this, _1);
}

/**
 * @inheritDoc
 */
GuiSystem::~GuiSystem(void) {

}


/**
 * @inheritDoc
 */
Error GuiSystem::initialize(void) {
    ASSERT(!m_bInitialized);
    
    Ogre::RenderTarget* renderWindow = (Ogre::RenderTarget*) g_Managers.pPlatform->Window().GetRenderWindow();
    m_pRenderer = &CEGUI::OgreRenderer::bootstrapSystem(*renderWindow);
    // TODO properties
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
    CEGUI::SchemeManager::getSingleton().create("OgreTray.scheme");
    CEGUI::System::getSingleton().setDefaultMouseCursor("OgreTrayImages", "MouseArrow");
    CEGUI::MouseCursor::getSingleton().setImage( CEGUI::System::getSingleton().getDefaultMouseCursor() );
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window *sheet = wmgr.createWindow("DefaultWindow", "HellionGame/Sheet");
    CEGUI::System::getSingleton().setGUISheet(sheet);
    // END TODO properties
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void GuiSystem::setImagesetResourceGroup(ProtoStringList values) {
    ProtoStringList::const_iterator value = values.begin();
    CEGUI::Imageset::setDefaultResourceGroup(*value);
}