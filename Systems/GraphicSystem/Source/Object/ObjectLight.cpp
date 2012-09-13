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
#include "Ogre.h"
#pragma warning( pop )

#include "BaseTypes.h"
#include "Interface.h"

#include "Scene.h"
#include "System.h"
#include "Object.h"
#include "ObjectLight.h"


#define POGRESCENEMGR (reinterpret_cast<OGREGraphicsScene*>(m_pSystemScene)->GetOGRESceneManager())

/**
 * @inheritDoc
 */
GraphicObjectLight::GraphicObjectLight(ISystemScene* pSystemScene, const char* pszName) : GraphicObject(pSystemScene, pszName)
    , m_pLight(NULL) {
    m_Type = GraphicObject::Type_Light;
}

/**
 * @inheritDoc
 */
GraphicObjectLight::~GraphicObjectLight(void) {
    if (m_pLight != NULL) {
        m_pNode->detachObject(m_pLight);
        POGRESCENEMGR->destroyLight(m_pLight);
    }
}

/**
 * @inheritDoc
 */
Error GraphicObjectLight::initialize(void) {
    ASSERT(!m_bInitialized);

    GraphicObject::initialize();
    m_pLight = POGRESCENEMGR->createLight(m_sName);

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void GraphicObjectLight::Update(f32 DeltaTime) {

}

/**
 * @inheritDoc
 */
Error GraphicObjectLight::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    return Errors::Success;
}

/*
void
GraphicObjectLight::SetProperties(
    Properties::Array Properties
) {
    ASSERT(m_bInitialized);

    //
    // Read in the properties.
    //
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();

            if (sName == sm_kapszPropertyNames[ Property_Position ]) {
                ASSERT(m_pLight != NULL);

                if (m_pLight != NULL) {
                    m_pLight->setPosition(it->GetVector3().x,
                                          it->GetVector3().y,
                                          it->GetVector3().z);
                }
            } else if (sName == sm_kapszPropertyNames[ Property_Diffuse ]) {
                ASSERT(m_pLight != NULL);

                if (m_pLight != NULL) {
                    m_pLight->setDiffuseColour(it->GetColor3().r,
                                               it->GetColor3().g,
                                               it->GetColor3().b);
                }
            } else if (sName == sm_kapszPropertyNames[ Property_Specular ]) {
                ASSERT(m_pLight != NULL);

                if (m_pLight != NULL) {
                    m_pLight->setSpecularColour(it->GetColor3().r,
                                                it->GetColor3().g,
                                                it->GetColor3().b);
                }
            } else if (sName == sm_kapszPropertyNames[ Property_Direction ]) {
                ASSERT(m_pLight != NULL);

                if (m_pLight != NULL) {
                    m_pLight->setDirection(it->GetVector3().x,
                                           it->GetVector3().y,
                                           it->GetVector3().z);
                }
            } else if (sName == sm_kapszPropertyNames[ Property_Range ]) {
                ASSERT(m_pLight != NULL);

                if (m_pLight != NULL) {
                    m_pLight->setSpotlightRange(Ogre::Radian(it->GetFloat32(0)),
                                                Ogre::Radian(it->GetFloat32(1)),
                                                it->GetFloat32(2));
                }
            } else if (sName == sm_kapszPropertyNames[ Property_Attenuation ]) {
                ASSERT(m_pLight != NULL);

                if (m_pLight != NULL) {
                    m_pLight->setAttenuation(it->GetFloat32(0),
                                             it->GetFloat32(1),
                                             it->GetFloat32(2),
                                             it->GetFloat32(3));
                }
            } else {
                ASSERT(false);
            }

            //
            // Set this property to invalid since it's already been read.
            //
            it->ClearFlag(Properties::Flags::Valid);
        }
    }
}
*/
