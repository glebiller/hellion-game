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

//
// core includes
//
#include "BaseTypes.h"
#include "Interface.h"

// UPDATE: provides access to environment variables
extern ManagerInterfaces   g_Managers;

//
// Ogre system includes
//
#include "Scene.h"
#include "Task.h"


#include "Object/Object.h"
#include "Object/ObjectCamera.h"
#include "Object/ObjectMesh.h"
#include "MovableText/DynamicLines.h"
#include "NormalizedLines.h"

#include "IttNotify.h"

__ITT_DEFINE_STATIC_EVENT(g_tpeRendering, "Graphics: Rendering", 19);

OGREGraphicsTask::OGREGraphicsTask(
    OGREGraphicsScene* pScene,
    Ogre::Root* pRoot
)
    : ISystemTask((ISystemScene*)pScene)
    , m_pScene(pScene)
    , m_pRoot(pRoot) {
    ASSERT(m_pRoot != NULL);
}


OGREGraphicsTask::~OGREGraphicsTask(void) {
}


System::Type OGREGraphicsTask::GetSystemType(void) {
    return System::Types::Graphic;
}


enum PolygonModes {
    PolygonMode_Invalid = -1,
    PolygonMode_Solid,
    PolygonMode_WireFrame,
    PolygonMode_WireFrame_NoTex,
    PolygonMode_Points,
    PolygonMode_Points_NoTex,
    PolygonMode_Solid_NoTex,
    PolygonMode_Count
};


void OGREGraphicsTask::Update(f32 DeltaTime) {
    static const std::string sCaption = "Caption";
    static bool bMaterialsSaved     = false;
    static i32  iPolygonModeOld     = 0;
    static bool bShowNormalsOld     = false;
    static bool bShowTangentsOld    = false;
    static bool bShowCaptionsOld    = false;
    static bool bShowBoundingBoxOld = false;
    // Get current state as updated by the input system.
    i32  iPolygonMode     = g_Managers.pEnvironment->Variables().GetAsInt("Graphics::PolygonMode");
    bool bShowNormals     = g_Managers.pEnvironment->Variables().GetAsBool("Graphics::ShowNormals");
    bool bShowTangents    = g_Managers.pEnvironment->Variables().GetAsBool("Graphics::ShowTangents");
    bool bShowBoundingBox = g_Managers.pEnvironment->Variables().GetAsBool("Graphics::ShowBoundingBox");
    bool bShowCaptions    = g_Managers.pEnvironment->Variables().GetAsBool("Graphics::ShowCaptions");

    if (bShowBoundingBox != bShowBoundingBoxOld) {
        m_pScene->m_pSceneManager->showBoundingBoxes(bShowBoundingBox == true);
        bShowBoundingBoxOld = bShowBoundingBox;
    }

    OGREGraphicsScene::ObjectsList* pObjects = &(static_cast<OGREGraphicsScene*>(m_pScene)->m_Objects);
    OGREGraphicsScene::ObjectsList::iterator it;
    ASSERT(pObjects);

    //
    // Only parse the object list if we have to make any Graphics state change
    //
    if (iPolygonMode  != iPolygonModeOld) {
        iPolygonModeOld = iPolygonMode;
        PolygonModes polygonMode = (PolygonModes)(iPolygonMode % (int)PolygonMode_Count);

        for (it = pObjects->begin(); it != pObjects->end(); it++) {
            GraphicObject* pObject = *it;

            if (GraphicObject::Type_Camera == pObject->GetType()) {
                GraphicObjectCamera* pCamera = dynamic_cast<GraphicObjectCamera*>(pObject);
                ASSERT(pCamera != NULL);
            } else if ((GraphicObject::Type_Mesh == pObject->GetType()) ||
                       (GraphicObject::Type_MeshAnimated == pObject->GetType())) {
                GraphicObjectMesh* pMesh = dynamic_cast<GraphicObjectMesh*>(pObject);
                ASSERT(pMesh != NULL);

                switch (polygonMode) {
                    case PolygonMode_Points_NoTex:
                    case PolygonMode_WireFrame_NoTex:
                    case PolygonMode_Solid_NoTex:
                        if (!bMaterialsSaved) {
                            // NoTexture mode is not supported for Instanced or Static Geom objects.
                            // Only for individual entities
                            if (pMesh->m_pEntity) {
                                // Save off original Material names..
                                int nSubs = pMesh->m_pEntity->getNumSubEntities();

                                for (int i = 0; i < nSubs; i++) {
                                    pMesh->m_MaterialNames.push_back(pMesh->m_pEntity->getSubEntity(i)->getMaterialName());
                                    pMesh->m_pEntity->getSubEntity(i)->setMaterialName("BaseWhiteNoLighting");
                                }
                            }
                        }

                        break;

                    case PolygonMode_Points:
                    case PolygonMode_WireFrame:
                    case PolygonMode_Solid:
                        if (bMaterialsSaved) {
                            // NoTexture mode is not supported for Instanced or Static Geom objects.
                            // Only for individual entities
                            if (pMesh->m_pEntity) {
                                // Restore original Material names..
                                u32 nSubs = pMesh->m_pEntity->getNumSubEntities();
                                // Make sure we have enough saved materials as there are subentities
                                ASSERT(nSubs <= pMesh->m_MaterialNames.size());

                                for (u32 i = 0; i < nSubs; i++) {
                                    pMesh->m_pEntity->getSubEntity(i)->setMaterialName(pMesh->m_MaterialNames[i]);
                                }

                                pMesh->m_MaterialNames.clear();
                            }
                        }

                        break;

                    default:
                        ASSERT(false);
                }
            }
        }

        if ((polygonMode == PolygonMode_Points) || (polygonMode == PolygonMode_Solid) || (polygonMode == PolygonMode_WireFrame)) {
            bMaterialsSaved = false;
        } else {
            bMaterialsSaved = true;
        }
    }

    std::vector<NormalizedLines*>::iterator itn;

    //
    // Here we check to see if the user has toggled "Display Normals" on/off. We cycle through
    // all of the mesh objects and enable/disable normal display using Ogre's existing functionality.
    //
    if (bShowNormals != bShowNormalsOld) {
        bShowNormalsOld = bShowNormals;

        for (it = pObjects->begin(); it != pObjects->end(); it++) {
            GraphicObject* pObject = *it;

            if ((GraphicObject::Type_Mesh == pObject->GetType()) ||
                    (GraphicObject::Type_MeshAnimated == pObject->GetType())) {
                GraphicObjectMesh* pMesh = dynamic_cast<GraphicObjectMesh*>(pObject);
                ASSERT(pMesh != NULL);

                if (bShowNormals) {
                    if (pMesh->m_pNormals.empty()) {
                        pMesh->SetupNormalsAndTangentsDisplay();
                    }

                    for (itn = pMesh->m_pNormals.begin(); itn != pMesh->m_pNormals.end(); itn++) {
                        if (!(*itn)->isAttached()) {
                            pMesh->m_pNode->attachObject(*itn);
                        }
                    }
                } else {
                    if (!(pMesh->m_pNormals.empty())) {
                        for (itn = pMesh->m_pNormals.begin(); itn != pMesh->m_pNormals.end(); itn++) {
                            pMesh->m_pNode->detachObject(*itn);
                        }
                    }
                }
            }
        }
    }

    //
    // Here we check to see if the user has toggled "Display Tangents" on/off. We cycle through
    // all of the mesh objects and enable/disable tangent display using Ogre's existing functionality.
    //
    if (bShowTangents != bShowTangentsOld) {
        bShowTangentsOld = bShowTangents;

        for (it = pObjects->begin(); it != pObjects->end(); it++) {
            GraphicObject* pObject = *it;

            if ((GraphicObject::Type_Mesh == pObject->GetType()) ||
                    (GraphicObject::Type_MeshAnimated == pObject->GetType())) {
                GraphicObjectMesh* pMesh = dynamic_cast<GraphicObjectMesh*>(pObject);
                ASSERT(pMesh != NULL);

                if (bShowTangents) {
                    if (pMesh->m_pTangents.empty()) {
                        pMesh->SetupNormalsAndTangentsDisplay();
                    }

                    for (itn = pMesh->m_pTangents.begin(); itn != pMesh->m_pTangents.end(); itn++) {
                        if (!(*itn)->isAttached()) {
                            pMesh->m_pNode->attachObject(*itn);
                        }
                    }
                } else {
                    if (!(pMesh->m_pTangents.empty())) {
                        for (itn = pMesh->m_pTangents.begin(); itn != pMesh->m_pTangents.end(); itn++) {
                            pMesh->m_pNode->detachObject(*itn);
                        }
                    }
                }
            }
        }
    }

    //
    // Here we check to see if the user has toggled "Display Captions (Object Names)" on/off. Each mesh
    // object will get a child node attached to it that will display the name of the object. These
    // nodes will be removed when the captions are disabled.
    //
    if (bShowCaptions != bShowCaptionsOld) {
        bShowCaptionsOld = bShowCaptions;

        for (it = pObjects->begin(); it != pObjects->end(); it++) {
            GraphicObject* pObject = *it;

            if ((GraphicObject::Type_Mesh == pObject->GetType()) ||
                    (GraphicObject::Type_MeshAnimated == pObject->GetType())) {
                GraphicObjectMesh* pMesh = dynamic_cast<GraphicObjectMesh*>(pObject);
                ASSERT(pMesh != NULL);

                if (bShowCaptions) {
                    if (pMesh->m_pCaption == NULL) {
                        pMesh->SetupCaptions();

                        if (pMesh->m_pCaption != NULL) {
                            Ogre::SceneNode* capNode;

                            if (pMesh->m_pNode) {
                                capNode = pMesh->m_pNode->createChildSceneNode(std::string(pMesh->m_pszName).append(sCaption));
                            } else {
                                capNode = m_pScene->m_pSceneManager->getRootSceneNode()->createChildSceneNode(
                                              std::string(pMesh->m_pszName).append(sCaption),
                                              TOOGREVEC(pMesh->m_Position),
                                              TOOGREQUAT(pMesh->m_Orientation));
                            }

                            capNode->attachObject(pMesh->m_pCaption);
                            capNode->setInheritScale(false);
                            capNode->hideBoundingBox(true);
                        }
                    }
                } else {
                    if (pMesh->m_pCaption != NULL) {
                        if (pMesh->m_pNode) {
                            pMesh->m_pNode->removeAndDestroyChild(std::string(pMesh->m_pszName).append(sCaption));
                        } else {
                            m_pScene->m_pSceneManager->getRootSceneNode()->removeAndDestroyChild(
                                std::string(pMesh->m_pszName).append(sCaption));
                        }

                        pMesh->m_pCaption = NULL;
                    }
                }
            }
        }
    }

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
    m_pScene->Update(DeltaTime);
}
