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

#include <boost/lexical_cast.hpp>

#pragma warning( push, 0 )
#pragma warning( disable : 6326 6385 )

#include <Ogre.h>
#include <Compositor/OgreCompositorManager2.h>

#pragma warning( pop )

#include "GraphicSystem.h"
#include "GraphicScene.h"
#include "Object/Object.h"
#include "Object/CameraGraphicObject.h"

/**
 * @inheritDoc
 */
CameraGraphicObject::CameraGraphicObject(ISystemScene& pSystemScene, UObject& entity,
                                         const Schema::SystemComponent& component)
        : GraphicObject(&pSystemScene, &entity, component)
        , m_pViewport(nullptr), m_vLookAt(Ogre::Vector3::ZERO),
          m_pCamera(GetSystemScene<GraphicScene>()->getSceneManager()->createCamera(entity.getName())) {
    m_pCameraNode = m_pNode->createChildSceneNode();
    m_pCameraNode->setName(m_entity->getId() + "Camera_SceneNode");
    m_pCamera->setPolygonMode(Ogre::PM_SOLID);

    //
    // Set the far clip distance
    //
    m_pCamera->setFarClipDistance(50000);
    if (m_pSystemScene->GetSystem<GraphicSystem>()->getRoot()->getRenderSystem()->getCapabilities()->hasCapability(
            Ogre::RSC_INFINITE_FAR_PLANE)) {
        m_pCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }
    m_pCamera->setNearClipDistance(1);
    //m_pNode->setPosition(500, 500, 500);


    const Ogre::String workspaceName = "SampleBrowserWorkspace";
    Ogre::CompositorManager2* compositorManager = m_pSystemScene->GetSystem<GraphicSystem>()->getRoot()->getCompositorManager2();
    if (!compositorManager->hasWorkspaceDefinition(workspaceName)) {
        compositorManager->createBasicWorkspaceDef(workspaceName, Ogre::ColourValue(0.6f, 0.0f, 0.6f));
    }
    compositorManager->addWorkspace(
            GetSystemScene<GraphicScene>()->getSceneManager(),
            m_pSystemScene->GetSystem<GraphicSystem>()->getRenderWindow(),
            m_pCamera, workspaceName, true);

}

/**
 * @inheritDoc
 */
CameraGraphicObject::~CameraGraphicObject() {
    //m_pCameraNode->detachObject(m_pCamera);

    GetSystemScene<GraphicScene>()->getSceneManager()->destroyCamera(m_pCamera);
}

/**
 * @inheritDoc
 */
void CameraGraphicObject::Update(float DeltaTime) {
    m_pCamera->lookAt(m_vLookAt);
}

/**
 * @inheritDoc
 */
Error CameraGraphicObject::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {
    if (ChangeType & Schema::EntityChange::PhysicPosition) {
        if (systemObject->getEntity()->getId() == "player") {
            auto position = systemObject->getComponent<Schema::Components::PhysicPosition>();
            m_pCamera->setPosition(position->x() - 50, position->y() + 50, position->z());
            m_vLookAt = Ogre::Vector3(position->x(), position->y(), position->z());
        }
    }
    /*if (ChangeType & System::Changes::Physic::Orientation) {
        IGeometryObject* pGeometryObject = dynamic_cast<IGeometryObject*>(pSubject);
        const Math::Quaternion& Orientation = *pGeometryObject->GetOrientation();
        m_pNode->setOrientation(Orientation.w, Orientation.x, Orientation.y, Orientation.z);
    }*/

    return Errors::Success;
}
