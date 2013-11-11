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

#include "Scene.h"
#include "System.h"
#include "Object.h"
#include "Object/LightGraphicObject.h"


#define POGRESCENEMGR (reinterpret_cast<GraphicScene*>(m_pSystemScene)->getSceneManager())

/**
 * @inheritDoc
 */
LightGraphicObject::LightGraphicObject(ISystemScene* pSystemScene, IEntity* entity)
    : GraphicObject(pSystemScene, entity)
    , m_pLight(NULL) {
}

/**
 * @inheritDoc
 */
LightGraphicObject::~LightGraphicObject(void) {
    if (m_pLight != NULL) {
        //m_pNode->detachObject(m_pLight);
        POGRESCENEMGR->destroyLight(m_pLight);
    }
}

/**
 * @inheritDoc
 */
Error LightGraphicObject::initialize(void) {
    ASSERT(!m_bInitialized);

    m_pLight = POGRESCENEMGR->createLight(m_entity->getName());
    
    // TODO make it properties
    m_pLight->setType(Ogre::Light::LightTypes::LT_POINT);
    m_pLight->setPosition(Ogre::Vector3(200, 200, 200));
    m_pLight->setDiffuseColour(Ogre::ColourValue(0.862745f, 0.0f, 0.0f));
    m_pLight->setSpecularColour(Ogre::ColourValue(0.862745f, 0.0f, 0.0f));

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void LightGraphicObject::Update(f32 DeltaTime) {

}

/**
 * @inheritDoc
 */
Error LightGraphicObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    return Errors::Success;
}
