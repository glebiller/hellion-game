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

#include "GraphicScene.h"
#include "GraphicSystem.h"
#include "Object/Object.h"
#include "Object/LightGraphicObject.h"


#define POGRESCENEMGR (reinterpret_cast<GraphicScene*>(m_pSystemScene)->getSceneManager())

/**
 * @inheritDoc
 */
LightGraphicObject::LightGraphicObject(ISystemScene* pSystemScene, UObject* entity,
                                       const Schema::SystemComponent& component)
    : GraphicObject(pSystemScene, entity, component)
    , m_pLight(NULL) {
    m_pLight = POGRESCENEMGR->createLight();
    m_pNode->attachObject(m_pLight);
    m_pNode->setPosition(0, 0, 0);
    m_pNode->setPosition(Ogre::Vector3(200, 200, 200));
    m_pLight->setType(Ogre::Light::LightTypes::LT_POINT);
    m_pLight->setDiffuseColour(Ogre::ColourValue(0.862745f, 0.0f, 0.0f));
    m_pLight->setSpecularColour(Ogre::ColourValue(0.862745f, 0.0f, 0.0f));
}

/**
 * @inheritDoc
 */
LightGraphicObject::~LightGraphicObject() {
    if (m_pLight != NULL) {
        m_pNode->detachObject(m_pLight);
        POGRESCENEMGR->destroyLight(m_pLight);
    }
}

/**
 * @inheritDoc
 */
void LightGraphicObject::Update(float DeltaTime) {

}

/**
 * @inheritDoc
 */
Error LightGraphicObject::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {

    return Errors::Success;
}
