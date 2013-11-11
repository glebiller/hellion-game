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
#include "System.h"
#include "Object.h"
#include "Object/MeshGraphicObject.h"
#include "Object/IGeometryObject.h"

#define POGRESCENEMGR (reinterpret_cast<GraphicScene*>(m_pSystemScene)->getSceneManager())

#include "Generic/IttNotify.h"

__ITT_DEFINE_STATIC_EVENT(g_tpeChangeOccurred, "Graphics Mesh: ChangeOccurred", 29);

u32 MeshGraphicObject::sm_EntityId = 0;

/**
 * @inheritDoc
 */
MeshGraphicObject::MeshGraphicObject(ISystemScene* pSystemScene, IEntity* entity) 
    : GraphicObject(pSystemScene, entity)
    , m_pEntity(NULL)
    , isProcedural(false)
    , m_strStaticGrpName("")
    , m_Position(Math::Vector3::Zero)
    , m_Orientation(Math::Quaternion::Zero)
    , m_Scale(Math::Vector3::One)
    , m_Dirty(true) {
    m_propertySetters["Mesh"] = boost::bind(&MeshGraphicObject::setMeshName, this, _1);
}


/**
 * @inheritDoc
 */
MeshGraphicObject::~MeshGraphicObject(void) {
    if (m_pEntity != NULL) {
        POGRESCENEMGR->destroyEntity(m_pEntity);
    }

    if (isProcedural && !pMesh.isNull()) {
        Ogre::MeshManager::getSingleton().remove(pMesh->getName());
    }
}

/**
 * @inheritDoc
 */
Error MeshGraphicObject::initialize(void) {
    ASSERT(!m_bInitialized);

    m_pNode->attachObject(m_pEntity);

    m_bInitialized = true;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
void MeshGraphicObject::Update(f32 DeltaTime) {

}

/**
 * @inheritDoc
 */
Error MeshGraphicObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    if (ChangeType & System::Changes::Physic::Position) {
        const Math::Vector3& Position = *dynamic_cast<IGeometryObject*>(pSubject)->GetPosition();
        m_pNode->setPosition(Position.x, Position.y, Position.z);
    }
    if (ChangeType & System::Changes::Physic::Orientation) {
        const Math::Quaternion& Orientation = *dynamic_cast<IGeometryObject*>(pSubject)->GetOrientation();
        m_pNode->setOrientation(Orientation.w, Orientation.x, Orientation.y, Orientation.z);
    }

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void MeshGraphicObject::setMeshName(Proto::RepeatedString* values) {
    if (!m_bInitialized) {
        auto value = values->begin();
        m_pEntity = POGRESCENEMGR->createEntity(m_entity->getName(), *value);
        ASSERT(m_pEntity != NULL);
    }
}

#pragma warning( pop )
