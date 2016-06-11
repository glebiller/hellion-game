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

#include "GraphicTask.h"

#pragma warning( push, 0 )
#include <Ogre.h>
#pragma warning( pop )

#include "GraphicScene.h"
#include "Object/Object.h"

///
/// @inheritDoc
///
GraphicTask::GraphicTask(ISystemScene* pScene) 
    : ISystemTask(pScene) {
    m_pRoot = pScene->GetSystem<GraphicSystem>()->getRoot();
    BOOST_ASSERT(m_pRoot != NULL);
}

///
/// @inheritDoc
///
GraphicTask::~GraphicTask() {

}

///
/// @inheritDoc
///
void GraphicTask::Update(float DeltaTime) {
    // Since rendering is a limiting serial stage in some (if not most) of the frames,
    // we do not want it to be preempted. So temporarily boost up its thread priority.
    // TODO
    //SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);
    //
    // Render the scene
    //
    m_pRoot->renderOneFrame();
    // Since it's the pool thread, we know that normally it runs at normal priority.
    // In more general case we would have needed to remember the initial priority
    // before bringing it up.
    // TODO
    //SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
    //
    // Update objects for next frame
    //
    m_pSystemScene->Update(DeltaTime);
}
