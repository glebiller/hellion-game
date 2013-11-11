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

#include "Scene.h"
#include "Object.h"

/**
 * @inheritDoc
 */
GraphicObject::GraphicObject(ISystemScene* pSystemScene, IEntity* entity) 
    : ISystemObject(pSystemScene, entity)
    , m_sceneManager(reinterpret_cast<GraphicScene*>(m_pSystemScene)->getSceneManager()) {
    m_pNode = m_sceneManager->createSceneNode(m_entity->getId() + "_SceneNode");
    ASSERT(m_pNode != NULL);
    m_sceneManager->getRootSceneNode()->addChild(m_pNode);
}

/**
 * @inheritDoc
 */
GraphicObject::~GraphicObject(void) {
    m_pNode->getParent()->removeChild(m_pNode);
    m_sceneManager->destroySceneNode(m_pNode);
}
