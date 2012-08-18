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

//
// core includes
//
#include "BaseTypes.h"
#include "Interface.h"

// UPDATE: provides access to environment variables
extern ManagerInterfaces   g_Managers;

//
// Gui system includes
//
#include "GuiScene.h"
#include "GuiTask.h"


#include "Object/GuiObject.h"

#include "IttNotify.h"
#include "CEGUI.h"

__ITT_DEFINE_STATIC_EVENT(g_tpeRendering, "Gui: Rendering", 19);

GuiTask::GuiTask(GuiScene* pScene) : ISystemTask((ISystemScene*)pScene)
    , m_pScene(pScene) {

}


GuiTask::~GuiTask(void) {

}


void GuiTask::Update(f32 DeltaTime) {
    //
    // Update objects for next frame
    //
    CEGUI::System::getSingleton().injectTimePulse(DeltaTime);

    //
    // Update objects for next frame
    //
    m_pScene->Update(DeltaTime);
}
