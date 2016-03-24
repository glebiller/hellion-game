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

#include "Scene.h"
#include "Object/Object.h"
#include "Object/CursorGraphicObject.h"

#define POGRESCENEMGR (reinterpret_cast<GraphicScene*>(m_pSystemScene)->getSceneManager())

/**
 * @inheritDoc
 */
CursorGraphicObject::CursorGraphicObject(ISystemScene* pSystemScene, UObject* entity,
                                         const Schema::SystemComponent& component)
    : GraphicObject(pSystemScene, entity, component) {
}

/**
 * @inheritDoc
 */
CursorGraphicObject::~CursorGraphicObject() {
}

/**
 * @inheritDoc
 */
void CursorGraphicObject::Update(float DeltaTime) {

}

/**
 * @inheritDoc
 */
Error CursorGraphicObject::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {

    
    return Errors::Success;
}

#pragma warning( pop )
