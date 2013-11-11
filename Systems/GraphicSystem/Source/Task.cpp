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

#include <berkelium/Berkelium.hpp>
#pragma warning( push, 0 )
#include <Ogre.h>
#pragma warning( pop )

#include "Scene.h"
#include "Task.h"
#include "Object/Object.h"
#include "Generic/IttNotify.h"


__ITT_DEFINE_STATIC_EVENT(g_tpeRendering, "Graphics: Rendering", 19);


/**
 * @inheritDoc
 */
GraphicTask::GraphicTask(ISystemScene* pScene) 
    : ISystemTask((ISystemScene*)pScene) {
    m_pRoot = pScene->GetSystem<GraphicSystem>()->getRoot(); 
    ASSERT(m_pRoot != NULL);
}

/**
 * @inheritDoc
 */
GraphicTask::~GraphicTask(void) {

}

/**
 * @inheritDoc
 */
void GraphicTask::Update(f32 DeltaTime) {
    // Since rendering is a limiting serial stage in some (if not most) of the frames,
    // we do not want it to be preempted. So temporarily boost up its thread priority.
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
    //
    // Render the scene
    //
    __ITT_EVENT_START(g_tpeRendering, PROFILE_RENDER);
    m_pRoot->renderOneFrame();
    __ITT_EVENT_END(g_tpeRendering, PROFILE_RENDER);
    // Since it's the pool thread, we know that normally it runs at normal priority.
    // In more general case we would have needed to remember the initial priority
    // before bringing it up.
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
    //
    // Update objects for next frame
    //
    m_pSystemScene->Update(DeltaTime);

    // Update GUI
    Berkelium::update();
}
