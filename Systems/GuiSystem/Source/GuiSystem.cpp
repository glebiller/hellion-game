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

//
// extern includes
//
#include "CEGUI.h"
#include "RendererModules/Ogre/CEGUIOgreRenderer.h"
#include "boost/functional/factory.hpp"

//
// Core includes
//
#include "Defines.h"
#include "BaseTypes.h"
#include "Interface.h"

//
// Graphic system includes
//
#include "GuiSystem.h"
#include "GuiScene.h"


//
// Global variables
//
extern ManagerInterfaces    g_Managers;

//
// Static member variables
//
const char* GuiSystem::sm_kapszPropertyNames[] = {
    "ResourceLocation",
    "WindowName",
    "Resolution",
    "ShadowTexture",
    "FullScreen",
    "VerticalSync",
    "FSAntiAliasing",
};

const Properties::Property GuiSystem::sm_kaDefaultProperties[] = {
    Properties::Property(sm_kapszPropertyNames[ Property_ResourceLocation ],
    VALUE4(Properties::Values::String,
    Properties::Values::String,
    Properties::Values::String,
    Properties::Values::Boolean),
    Properties::Flags::Valid | Properties::Flags::InitOnly |
    Properties::Flags::Multiple,
    "", "", "", 0),

    Properties::Property(sm_kapszPropertyNames[ Property_WindowName ],
    VALUE1(Properties::Values::String),
    Properties::Flags::Valid | Properties::Flags::InitOnly,
    ""),
    Properties::Property(sm_kapszPropertyNames[ Property_Resolution ],
    VALUE1x2(Properties::Values::Int32),
    Properties::Flags::Valid,
    1024, 768),
    Properties::Property(sm_kapszPropertyNames[ Property_ShadowTexture ],
    VALUE1x2(Properties::Values::Int32),
    Properties::Flags::Valid,
    1, 1024),
    Properties::Property(sm_kapszPropertyNames[ Property_FullScreen ],
    VALUE1(Properties::Values::Int32),
    Properties::Flags::Valid,
    0),
    Properties::Property(sm_kapszPropertyNames[ Property_VerticalSync ],
    VALUE1(Properties::Values::Int32),
    Properties::Flags::Valid,
    0),
    Properties::Property(sm_kapszPropertyNames[ Property_FSAntiAliasing ],
    VALUE1x2(Properties::Values::String),
    Properties::Flags::Valid,
    "0" /* D3DMULTISAMPLE_NONE */,
    "0" /* Default Quality level offered by driver */),
};


GuiSystem::GuiSystem(void) : ISystem() {
    ASSERT(Property_Count == (sizeof sm_kapszPropertyNames / sizeof sm_kapszPropertyNames[ 0 ]));
    ASSERT(Property_Count == (sizeof sm_kaDefaultProperties / sizeof sm_kaDefaultProperties[ 0 ]));

    //
    // Init the Scene factory
    //
    m_SceneFactory = boost::factory<GuiScene*>();
}


GuiSystem::~GuiSystem(void) {

}


Error GuiSystem::Initialize(Properties::Array Properties) {
    ASSERT(!m_bInitialized);

    /*
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        //
        // Make sure this property is valid.
        //
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();

            if (sName == sm_kapszPropertyNames[ Property_ResourceLocation ]) {
                const char* pszName = it->GetStringPtr(0);
                const char* pszLocationType = it->GetStringPtr(1);
                const char* pszResourceGroup = it->GetStringPtr(2);
                bool  bRecursive = it->GetBool(3);
                m_pResourceGroupManager->addResourceLocation(pszName, pszLocationType, pszResourceGroup, (bRecursive == true));
                m_pResourceGroupManager->initialiseResourceGroup(pszResourceGroup);
                m_pResourceGroupManager->loadResourceGroup(pszResourceGroup);
            } else if (sName == sm_kapszPropertyNames[ Property_WindowName ]) {
                strcpy_s(szWindowName, sizeof szWindowName, it->GetStringPtr(0));
            } else if (sName == sm_kapszPropertyNames[ Property_Resolution ]) {
                Width  = static_cast<u32>(it->GetInt32(0));
                Height  = static_cast<u32>(it->GetInt32(1));
            } else if (sName == sm_kapszPropertyNames[ Property_ShadowTexture ]) {
                m_uShadowTextureCount  = static_cast<u16>(it->GetInt32(0));
                m_uShadowTextureSize  = static_cast<u16>(it->GetInt32(1));
            } else if (sName == sm_kapszPropertyNames[ Property_FullScreen ]) {
                bFullScreen = it->GetBool(0);
            } else if (sName == sm_kapszPropertyNames[ Property_VerticalSync ]) {
                bVerticalSync = it->GetBool(0);
            } else if (sName == sm_kapszPropertyNames[ Property_FSAntiAliasing ]) {
                dFSAAType    = it->GetString(0);
                dFSAAQuality = it->GetString(1);
            }

            //
            // Set this property to invalid since it's already been read.
            //
            it->ClearFlag(Properties::Flags::Valid);
        }
    }*/
    
    Ogre::RenderTarget* renderWindow = (Ogre::RenderTarget*) g_Managers.pPlatform->Window().GetRenderWindow();
    m_pRenderer = &CEGUI::OgreRenderer::bootstrapSystem(*renderWindow);
    // TODO properties
    CEGUI::Imageset::setDefaultResourceGroup("Imagesets");
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

    //
    // Set as initialized.
    //
    m_bInitialized = true;

    //
    // Set the remaining properties.
    //
    SetProperties(Properties);

    return Errors::Success;
}


void GuiSystem::GetProperties(Properties::Array& Properties) {
    //
    // Get the index of our first item.
    //
    i32 iProperty = static_cast<i32>(Properties.size());
    //
    // Add all the properties.
    //
    Properties.reserve(Properties.size() + Property_Count);

    for (i32 i = 0; i < Property_Count; i++) {
        Properties.push_back(sm_kaDefaultProperties[ i ]);
    }
}


void GuiSystem::SetProperties(Properties::Array Properties) {
    ASSERT(m_bInitialized);

    //
    // Read in the properties.
    //
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();

            if (sName == sm_kapszPropertyNames[ Property_Resolution ]) {
                
            } else {
                ASSERT(false);
            }

            //
            // Set this property to invalid since it's already been read.
            //
            it->ClearFlag(Properties::Flags::Valid);
        }
    }
}
