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

//
// core includes
//
#include "BaseTypes.h"
#include "Interface.h"

//
// Ogre system includes
//
#include "GuiScene.h"
#include "Object/GuiObject.h"

#define POGREROOTNODE (reinterpret_cast<OGREGraphicsScene*>(m_pSystemScene)->GetOGRERootSceneNode())


const char* GuiObject::sm_kapszTypeNames[] = {
    "Light", "LightFire", "Camera", "Mesh", "MeshAnimated", "PointList", "Window", "StatWindow", "Chart", "CPUChart", "WorkloadWindow",
    "ParticleSystem", "PagedGeometryLayer", "Terrain", "Sky",
    NULL
};


GuiObject::GuiObject(ISystemScene* pSystemScene, const char* pszName)
    : ISystemObject(pSystemScene, pszName) {
    m_pszName = pszName;
}


GuiObject::~GuiObject(void) {

}

void GuiObject::Update(f32 DeltaTime) {
    UNREFERENCED_PARAM(DeltaTime);
    return;
}


Error GuiObject::Initialize(std::vector<Properties::Property> Properties) {
    ASSERT(!m_bInitialized);
    return Errors::Success;
}


Error GuiObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    return Errors::Success;
}

