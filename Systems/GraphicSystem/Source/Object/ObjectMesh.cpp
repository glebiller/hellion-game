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

#pragma warning( push, 0 )
#pragma warning( disable : 4244 6211 6386 )

#include "BaseTypes.h"
#include "Interface.h"

#include "Scene.h"
#include "System.h"
#include "Object.h"
#include "ObjectMesh.h"
#include "MovableText/DynamicLines.h"
#include "NormalizedLines.h"
#include "MovableText/MovableText.h"

#define POGRESCENEMGR (reinterpret_cast<OGREGraphicsScene*>(m_pSystemScene)->GetOGRESceneManager())

#include <IttNotify.h>

__ITT_DEFINE_STATIC_EVENT(g_tpeChangeOccurred, "Graphics Mesh: ChangeOccurred", 29);

u32 GraphicObjectMesh::sm_EntityId = 0;

/**
 * @inheritDoc
 */
GraphicObjectMesh::GraphicObjectMesh(ISystemScene* pSystemScene, const char* pszName) : GraphicObject(pSystemScene, pszName)
    , m_pEntity(NULL)
    , isProcedural(false)
    , m_strStaticGrpName("")
    , m_Position(Math::Vector3::Zero)
    , m_Orientation(Math::Quaternion::Zero)
    , m_Scale(Math::Vector3::One)
    , m_pCaption(NULL)
    , m_Dirty(true) {
    m_Type = GraphicObject::Type_Mesh;
}


/**
 * @inheritDoc
 */
GraphicObjectMesh::~GraphicObjectMesh(void) {
    if (m_pEntity != NULL) {
        m_pNode->detachObject(m_pEntity);
        POGRESCENEMGR->destroyEntity(m_pEntity);
    }

    if (isProcedural && !pMesh.isNull()) {
        Ogre::MeshManager::getSingleton().remove(pMesh->getName());
    }

    std::vector< NormalizedLines* >::iterator  it;

    for (it = m_pNormals.begin(); it != m_pNormals.end(); it++) {
        SAFE_DELETE(*it);
    }

    for (it = m_pTangents.begin(); it != m_pTangents.end(); it++) {
        SAFE_DELETE(*it);
    }

    SAFE_DELETE(m_pCaption);
}

/**
 * @inheritDoc
 */
Error GraphicObjectMesh::initialize(void) {
    ASSERT(!m_bInitialized);

    GraphicObject::initialize();

    return Errors::Success;
}

/*
void
GraphicObjectMesh::SetProperties(
    Properties::Array Properties
) {
    ASSERT(m_bInitialized);

    //
    // Read in the properties.
    //
    for (Properties::Iterator it = Properties.begin(); it != Properties.end(); it++) {
        if (it->GetFlags() & Properties::Flags::Valid) {
            std::string sName = it->GetName();

            if (sName == sm_kapszPropertyNames[ Property_Material ]) {
                if (m_pEntity != NULL) {
                    m_pEntity->setMaterialName(it->GetStringPtr(0));
                } else if (isProcedural) {
                    sMaterialName = it->GetStringPtr(0);
                } else {
                    ASSERT(false);
                }
            } else if (sName == sm_kapszPropertyNames[ Property_CastShadows ]) {
                if (m_pEntity != NULL) {
                    m_pEntity->setCastShadows(it->GetBool(0) != false);
                } else if (isProcedural) {
                    bCastShadows = it->GetBool(0) != false;
                } else {
                    ASSERT(false);
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

/**
 * @inheritDoc
 */
Error GraphicObjectMesh::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void GraphicObjectMesh::setMeshName(ProtoStringList values) {
    if (!m_bInitialized) {
        ProtoStringList::const_iterator value = values.begin();
        m_pEntity = POGRESCENEMGR->createEntity(m_sName, *value);
        m_pNode->attachObject(m_pEntity);
        ASSERT(m_pEntity != NULL);
    }
}

#pragma warning( pop )
