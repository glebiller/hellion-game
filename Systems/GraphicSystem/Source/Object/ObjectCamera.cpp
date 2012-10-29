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

#pragma warning( push, 0 )
#pragma warning( disable : 6326 6385 )
#include "Ogre.h"
#include "PagedGeometry.h"
#include "GrassLoader.h"
#pragma warning( pop )

#include "boost/lexical_cast.hpp"

#include "BaseTypes.h"
#include "Interface.h"

#include "System.h"
#include "Scene.h"
#include "Object.h"
#include "ObjectCamera.h"

#define PSCENE          (reinterpret_cast<OGREGraphicsScene*>(m_pSystemScene))
#define PSYSTEM         (reinterpret_cast<GraphicSystem*>(m_pSystemScene->GetSystem()))
#define POGRESCENEMGR   (PSCENE->GetOGRESceneManager())

/**
 * @inheritDoc
 */
GraphicObjectCamera::GraphicObjectCamera(ISystemScene* pSystemScene, const char* pszName) : GraphicObject(pSystemScene, pszName)
    , m_pCamera(NULL)
    , m_pViewport(NULL)
    , m_vLookAt(Ogre::Vector3::ZERO) {
    m_Type = GraphicObject::Type_Camera;
}

/**
 * @inheritDoc
 */
GraphicObjectCamera::~GraphicObjectCamera(void) {
    ASSERT(m_pCamera != NULL);
    ASSERT(m_pViewport != NULL);

    if (m_pViewport != NULL) {
        Ogre::RenderWindow* pRenderWindow = PSYSTEM->GetOGRERenderWindow();
        pRenderWindow->removeViewport(m_pViewport->getZOrder());
    }

    if (m_pCamera != NULL) {
        m_pCameraNode->detachObject(m_pCamera);
        POGRESCENEMGR->destroyCamera(m_pCamera);
    }
}

/**
 * @inheritDoc
 */
Error GraphicObjectCamera::initialize(void) {
    ASSERT(!m_bInitialized);

    //
    // Custom init function for the camera
    //
    Ogre::SceneNode* parentNode;
    // TODO parameter this
    if (POGRESCENEMGR->hasSceneNode("Player_SceneNode")) {
        parentNode = POGRESCENEMGR->getSceneNode("Player_SceneNode");
    } else {
        parentNode = reinterpret_cast<OGREGraphicsScene*>(m_pSystemScene)->GetOGRERootSceneNode();
    }
    m_pNode = parentNode->createChildSceneNode(m_sName + "_SceneNode");
    m_pNode->setPosition(0, 1.0f, 0);
    ASSERT(m_pNode != NULL);
    m_pCameraNode = m_pNode->createChildSceneNode(m_sName + "Camera_SceneNode");
    ASSERT(m_pCameraNode != NULL);

    //
    // Create the camera.
    //
    m_pCamera = POGRESCENEMGR->createCamera(m_sName);
    ASSERT(m_pCamera != NULL);

    if (m_pCamera != NULL) {
        //
        // Create the viewport.
        //
        Ogre::RenderWindow* pRenderWindow = PSYSTEM->GetOGRERenderWindow();
        m_pViewport = pRenderWindow->addViewport(m_pCamera);
        ASSERT(m_pViewport != NULL);
        
        m_pCamera->setPolygonMode(Ogre::PM_SOLID);

        if (m_pViewport != NULL) {
            m_pViewport->setBackgroundColour(Ogre::ColourValue(0, 0, 0));
            //
            // Set the camera's aspect ratio to the dimensions of the viewport.
            //
            m_pCamera->setAspectRatio(Ogre::Real(m_pViewport->getActualWidth()) /
                                      Ogre::Real(m_pViewport->getActualHeight()));
            //
            // Set auto tracking
            //
            m_pCamera->setAutoTracking(true, m_pNode);
            m_pCamera->setFixedYawAxis(true);
            //
            // Attach the camera to the Ogre scene node.
            //
            m_pCameraNode->attachObject(m_pCamera);
            m_pCamera->setPosition(Ogre::Vector3::ZERO);
            m_pCameraNode->hideBoundingBox(true);

            //
            // Set the far clip distance
            //
            if (PSYSTEM->GetOGRERoot()->getRenderSystem()->getCapabilities()->hasCapability(Ogre::RSC_INFINITE_FAR_PLANE)) {
                m_pCamera->setFarClipDistance(0);   // enable infinite far clip distance if we can
            }

            return Errors::Success;
        }
    }
    
    return Errors::Failure;
}

/**
 * @inheritDoc
 */
void GraphicObjectCamera::Update(f32 DeltaTime) {
    UNREFERENCED_PARAM(DeltaTime);
    m_pCamera->lookAt(m_vLookAt);
}

/**
 * @inheritDoc
 */
Error GraphicObjectCamera::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    if (ChangeType & System::Changes::Geometry::Orientation) {
        const Math::Quaternion& orientation = *dynamic_cast<IGeometryObject*>(pSubject)->GetOrientation();
        m_pNode->resetOrientation();
        m_pNode->yaw(Ogre::Radian(orientation.x));
        m_pNode->roll(Ogre::Radian(orientation.y));
    }

    if (ChangeType & System::Changes::Geometry::Position) {
        Math::Vector3 Position = *dynamic_cast<IGeometryObject*>(pSubject)->GetPosition();
        m_pCameraNode->setPosition(TOOGREVEC(Position));
    }

    if (ChangeType & System::Changes::Physics::Position) {
        Math::Vector3 Position = *dynamic_cast<IGeometryObject*>(pSubject)->GetPosition();
        m_vLookAt.x = Position.x;
        m_vLookAt.y = Position.y;
        m_vLookAt.z = Position.z;
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void GraphicObjectCamera::setFOVy(ProtoStringList values) {
    ProtoStringList::const_iterator value = values.begin();
    m_pCamera->setFOVy(Ogre::Radian(boost::lexical_cast<f32>(*value)));
}

/**
 * @inheritDoc
 */
void GraphicObjectCamera::setClipDistances(ProtoStringList values) {
    ProtoStringList::const_iterator value = values.begin();
    m_pCamera->setNearClipDistance(boost::lexical_cast<f32>(*(value++)));
    m_pCamera->setFarClipDistance(boost::lexical_cast<f32>(*value));
}

/**
 * @inheritDoc
 */
void GraphicObjectCamera::setPolygonMode(ProtoStringList values) {
    ProtoStringList::const_iterator value = values.begin();

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
