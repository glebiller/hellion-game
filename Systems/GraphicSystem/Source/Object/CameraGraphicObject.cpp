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

#include <boost/lexical_cast.hpp>
#pragma warning( push, 0 )
#pragma warning( disable : 6326 6385 )
#include <Ogre.h>
#include <OgreSceneManager.h>
#pragma warning( pop )

#include "System.h"
#include "Scene.h"
#include "Object.h"
#include "Object/CameraGraphicObject.h"
#include "Object/IGeometryObject.h"

#define PSCENE          (reinterpret_cast<GraphicScene*>(m_pSystemScene))
#define PSYSTEM         (m_pSystemScene->GetSystem<GraphicSystem>())
#define POGRESCENEMGR   (PSCENE->getSceneManager())

/**
 * @inheritDoc
 */
CameraGraphicObject::CameraGraphicObject(ISystemScene* pSystemScene, IEntity* entity) : GraphicObject(pSystemScene, entity)
    , m_pViewport(nullptr)
    , m_vLookAt(Ogre::Vector3::ZERO)
    , m_pCamera(GetSystemScene<GraphicScene>()->getSceneManager()->createCamera(entity->getName())) {
    m_propertySetters["FOVy"] = boost::bind(&CameraGraphicObject::setFOVy, this, _1);
    m_propertySetters["ClipDistances"] = boost::bind(&CameraGraphicObject::setClipDistances, this, _1);
}

/**
 * @inheritDoc
 */
CameraGraphicObject::~CameraGraphicObject(void) {
    if (m_bInitialized) {
        GetSystemScene<GraphicScene>()->GetSystem<GraphicSystem>()->getRenderWindow()->removeViewport(m_pViewport->getZOrder());
        m_pCameraNode->detachObject(m_pCamera);
    }
    
    GetSystemScene<GraphicScene>()->getSceneManager()->destroyCamera(m_pCamera);
}

/**
 * @inheritDoc
 */
Error CameraGraphicObject::initialize(void) {
    ASSERT(!m_bInitialized);

    if (m_pCamera == NULL) {
        return Errors::Failure;
    }

    //
    // Custom init function for the camera
    //
    m_pCameraNode = m_pNode->createChildSceneNode(m_entity->getId() + "Camera_SceneNode");
    ASSERT(m_pCameraNode != NULL);

    //
    // Create the viewport.
    //
    Ogre::RenderWindow* pRenderWindow = PSYSTEM->getRenderWindow();
    m_pViewport = pRenderWindow->addViewport(m_pCamera);
    ASSERT(m_pViewport != NULL);
        
    m_pCamera->setPolygonMode(Ogre::PM_SOLID);

    if (m_pViewport == NULL) {
        return Errors::Failure;
    }

    m_pViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
    //
    // Set the camera's aspect ratio to the dimensions of the viewport.
    //
    m_pCamera->setAspectRatio(Ogre::Real(m_pViewport->getActualWidth()) /
                                Ogre::Real(m_pViewport->getActualHeight()));
    //
    // Set auto tracking
    //
    //m_pCamera->setAutoTracking(true, m_pNode);
    //m_pCamera->setFixedYawAxis(true);
    //
    // Attach the camera to the Ogre scene node.
    //
    m_pCameraNode->attachObject(m_pCamera);
    //m_pCameraNode->hideBoundingBox(true);

    //
    // Set the far clip distance
    //
    if (PSYSTEM->getRoot()->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE)) {
        m_pCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
    }
            
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void CameraGraphicObject::Update(f32 DeltaTime) {
    m_pCamera->lookAt(m_vLookAt);
}

/**
 * @inheritDoc
 */
Error CameraGraphicObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    if (ChangeType & System::Changes::Physic::Position) {
        IGeometryObject* pGeometryObject = dynamic_cast<IGeometryObject*>(pSubject);
        const Math::Vector3& Position = *pGeometryObject->GetPosition();
        m_pNode->setPosition(Position.x, Position.y, Position.z);
    }
    if (ChangeType & System::Changes::Physic::Orientation) {
        IGeometryObject* pGeometryObject = dynamic_cast<IGeometryObject*>(pSubject);
        const Math::Quaternion& Orientation = *pGeometryObject->GetOrientation();
        m_pNode->setOrientation(Orientation.w, Orientation.x, Orientation.y, Orientation.z);
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void CameraGraphicObject::setFOVy(Proto::RepeatedString* values) {
    auto value = values->begin();
    m_pCamera->setFOVy(Ogre::Radian(boost::lexical_cast<f32>(*value)));
}

/**
 * @inheritDoc
 */
void CameraGraphicObject::setClipDistances(Proto::RepeatedString* values) {
    auto value = values->begin();
    m_pCamera->setNearClipDistance(boost::lexical_cast<f32>(*value));
    m_pCamera->setFarClipDistance(boost::lexical_cast<f32>(*(++value)));
}

/**
 * @inheritDoc
 */
void CameraGraphicObject::setPolygonMode(Proto::RepeatedString* values) {
    auto value = values->begin();

    PolygonModes polygonMode = static_cast<PolygonModes>(boost::lexical_cast<i32>(*value));
    ASSERT(polygonMode > PolygonMode_Invalid);
    ASSERT(polygonMode < PolygonMode_Count);

    switch (polygonMode) {
        case PolygonMode_Points:
            m_pCamera->setPolygonMode(Ogre::PM_POINTS);
            break;

        case PolygonMode_WireFrame:
            m_pCamera->setPolygonMode(Ogre::PM_WIREFRAME);
            break;

        case PolygonMode_Solid:
            m_pCamera->setPolygonMode(Ogre::PM_SOLID);
            break;
    }
}
