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

//
// extern includes
//
#pragma warning( push, 0 )
// Temporarily switching warning level to 0 to ignore warnings in extern/Ogre
#include "Ogre.h"
#pragma warning( pop )

#ifdef SAFE_DELETE
#undef SAFE_DELETE
#endif
#ifdef SAFE_DELETE_ARRAY
#undef SAFE_DELETE_ARRAY
#endif

//
// core includes
//
#include "BaseTypes.h"
#include "Interface.h"

#include "System.h"
#include "Scene.h"
#include "Object/Object.h"
#include "Object/GUI/ObjectWindow.h"
#include "Object/GUI/ObjectLoginWindow.h"


extern ManagerInterfaces       g_Managers;

// Define update constants
const f32 GraphicObjectLoginWindow::m_secondsPerUpdate = 1.0f;


//-----------------------------------------------------------------------------
//
// GraphicObjectLoginWindow::GraphicObjectLoginWindow
//
// Stat window constructor.
//
//-----------------------------------------------------------------------------
GraphicObjectLoginWindow::GraphicObjectLoginWindow(ISystemScene* pSystemScene, const char* pszName) :
    GraphicObjectWindow(pSystemScene, pszName),
    m_secondsSinceLastUpdate(0.0f),
    m_framesSinceLastUpdate(0) {
    m_Type = GraphicObject::Type_StatWindow;
}


//-----------------------------------------------------------------------------
//
// GraphicObjectLoginWindow::~GraphicObjectLoginWindow
//
// Stat window destructor.
//
//-----------------------------------------------------------------------------
GraphicObjectLoginWindow::~GraphicObjectLoginWindow() {
    // DONOTHING
}


//-----------------------------------------------------------------------------
//
// GraphicObjectLoginWindow::Update
//
// Get updates for this window, and apply them to the window.  Called every time there is an update.
//
//-----------------------------------------------------------------------------
void GraphicObjectLoginWindow::Update(f32 DeltaTime) {
    
}
