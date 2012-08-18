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
#include "boost/functional/factory.hpp"

//
// Core includes
//
#include "BaseTypes.h"
#include "Interface.h"

//
// Graphics system includes
//
#include "GuiSystem.h"
#include "GuiScene.h"
#include "GuiTask.h"
#include "Object/GuiObject.h"
#include "Object/GuiMouseObject.h"

#define POGREROOT       (reinterpret_cast<GraphicSystem*>(m_pSystem)->GetOGRERoot())

extern ManagerInterfaces       g_Managers;

// We use SM2.0 instancing. Since we do normal mapping on the instanced geometry, we
// need to pass both the world matrix and the inverse world matrix for each instance.
// Given our limit of 256 float4 constants, we set the Objects per batch to 40.
// 40 float3x3 world matrices           = 120 float3/4 constants
// 40 float3x3 world inverse matrices   = 120 float3/4 constants
// we have used 240 float4 constants. The rest 16 are saved to pass other information
// to the instancing shader.
static const u32 maxObjectsPerBatch = 40;

// Prototypes
void ProcessObjects(void* Data);

static const u32   UpdateGrainSize = 120;

const char* GuiScene::sm_kapszPropertyNames[] = {
    "ResourceLocation", "DelResourceLocation",
    "AmbientLight", "Shadows", "ShadowColor",
    "DrawBoundingBox", "ShowNormals", "ShowTangents",
    "UseStaticGeom", "UseInstancedGeom", "FogColor", "Fog", "Font",
    "PagedGeometry", "PagedGeometryTerrain", "PagedGeometryTerrainOffset"
};

GuiScene::GuiScene(ISystem* pSystem) : ISystemScene(pSystem)
    , m_pTask(NULL) {
    m_ObjectFactories["Mouse"] = boost::factory<GuiMouseObject*>();
}


GuiScene::~GuiScene(void) {
    SAFE_DELETE(m_pTask);
}


/**
 * @inheritDoc
 */
void GuiScene::Update(f32 DeltaTime) {
    ObjectsList Objects = m_Objects;

    //
    // Cycle through all of our objects and apply the changes.
    // Also post our change notifications to the CCM.
    //
    for (ObjectsList::iterator it = Objects.begin(); it != Objects.end(); it++) {
        GuiObject* pObject = *it;
        pObject->Update(DeltaTime);
    }
}


void GuiScene::GlobalSceneStatusChanged(GlobalSceneStatus Status) {
    
}


Error GuiScene::Initialize(std::vector<Properties::Property> Properties) {
    ASSERT(!m_bInitialized);

    m_pTask = new GuiTask(this);
    ASSERT(m_pTask != NULL);

    if (m_pTask == NULL) {
        return Errors::Failure;
    }

    CEGUI::Window *newWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout("LoginForm.layout");
    CEGUI::System::getSingleton().getGUISheet()->addChildWindow(newWindow);

    m_bInitialized = true;
    //
    // Set the properties for this scene.
    //
    SetProperties(Properties);

    return Errors::Success;
}


void GuiScene::GetProperties(Properties::Array& Properties) {

}


void GuiScene::SetProperties(Properties::Array Properties) {
    ASSERT(m_bInitialized);
}


const char** GuiScene::GetObjectTypes(void) {
    return GuiObject::sm_kapszTypeNames;
}


ISystemObject* GuiScene::CreateObject(const char* pszName, const char* pszType) {
    ASSERT(m_bInitialized);
    GuiObject* pObject = NULL;

    pObject = m_ObjectFactories[pszType](this, pszName);

    //
    //  Store the newly created object for future access
    //
    if (pObject != NULL) {
        m_Objects.push_back(pObject);
    } else {
        ASSERT(false);
    }

    return pObject;
}


Error GuiScene::DestroyObject(ISystemObject* pSystemObject) {
    ASSERT(m_bInitialized);
    ASSERT(pSystemObject != NULL);
    //
    // Cast to a GraphicObject so that the correct destructor will be called.
    //
    GuiObject* pObject = reinterpret_cast<GuiObject*>(pSystemObject);

    if (pObject != NULL) {
        //
        // Remove the object from the list and delete it.
        //
        m_Objects.erase(std::find(m_Objects.begin(), m_Objects.end(), pObject));
        delete pObject;
    }

    return Errors::Success;
}


ISystemTask* GuiScene::GetSystemTask(void) {
    return m_pTask;
}


System::Changes::BitMask GuiScene::GetPotentialSystemChanges(void) {
    return System::Changes::None;
}


const void* GuiScene::GetSystemChangeData(System::Change SystemChange) {
    UNREFERENCED_PARAM(SystemChange);
    return NULL;
}
