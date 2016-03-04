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


#pragma warning( push, 0 )
// Temporarily switching warning level to 0 to ignore warnings in extern/Ogre
#include <Ogre.h>
#pragma warning( pop )

#pragma warning( push, 0 )
#pragma warning( disable : 4244 6211 6386 )

#include "Manager/ServiceManager.h"
#include "Scene.h"
#include "Object/Object.h"
#include "Object/CursorGraphicObject.h"

#define POGRESCENEMGR (reinterpret_cast<GraphicScene*>(m_pSystemScene)->getSceneManager())

extern IServiceManager* g_serviceManager;

/**
 * @inheritDoc
 */
CursorGraphicObject::CursorGraphicObject(ISystemScene* pSystemScene, UObject* entity) 
    : GraphicObject(pSystemScene, entity) {
    //m_propertySetters["Mesh"] = boost::bind(&MeshGraphicObject::setMeshName, this, _1);
}

/**
 * @inheritDoc
 */
CursorGraphicObject::~CursorGraphicObject() {
}

/**
 * @inheritDoc
 */
Error CursorGraphicObject::initialize() {
    ASSERT(!m_bInitialized);
    
    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void CursorGraphicObject::Update(f32 DeltaTime) {

}

/**
 * @inheritDoc
 */
Error CursorGraphicObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    
    return Errors::Success;
}

#pragma warning( pop )
