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

#include <Ogre.h>
#include <schema/scene_generated.h>
#include <schema/physic_components_generated.h>
#include <System/ISystemObject.h>

#include "Scene.h"
#include "System.h"
#include "Object/Object.h"
#include "Object/MeshGraphicObject.h"

#define POGRESCENEMGR (reinterpret_cast<GraphicScene*>(m_pSystemScene)->getSceneManager())

#include "Generic/IttNotify.h"

__ITT_DEFINE_STATIC_EVENT(g_tpeChangeOccurred, "Graphics Mesh: ChangeOccurred", 29);

unsigned int MeshGraphicObject::sm_EntityId = 0;

/**
 * @inheritDoc
 */
MeshGraphicObject::MeshGraphicObject(ISystemScene& pSystemScene, UObject& entity,
                                     const Schema::SystemComponent& component)
    : GraphicObject(&pSystemScene, &entity, component)
    , isProcedural(false)
    , m_strStaticGrpName("")
    , m_Dirty(true) {
    auto mesh = reinterpret_cast<const Schema::Components::GraphicMesh *>(component.data());
    m_pEntity = POGRESCENEMGR->createEntity(mesh->name()->c_str());
    m_pNode->setName(m_entity->getName());
    m_pNode->attachObject(m_pEntity);
}


/**
 * @inheritDoc
 */
MeshGraphicObject::~MeshGraphicObject() {
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
void MeshGraphicObject::Update(float DeltaTime) {

}

/**
 * @inheritDoc
 */
Error MeshGraphicObject::ChangeOccurred(ISystemObject* systemObject, System::Changes::BitMask ChangeType) {


    // TODO next step
    if (ChangeType & Schema::EntityChange::PhysicPosition) {
        auto component = systemObject->getComponent();
        auto position = static_cast<const Schema::Components::PhysicPosition *>(component);
        m_pNode->setPosition(position->x(), position->y(), position->z());
    }
    /*if (ChangeType & System::Changes::Physic::Orientation) {
        const Math::Quaternion& Orientation = *dynamic_cast<IGeometryObject*>(pSubject)->GetOrientation();
        m_pNode->setOrientation(Orientation.w, Orientation.x, Orientation.y, Orientation.z);
    }*/

    return Errors::Success;
}

/**
 * @inheritDoc
 */
void MeshGraphicObject::setMeshName(std::string values) {
        /*auto value = values->begin();
        m_pEntity = POGRESCENEMGR->createEntity(m_entity->getName(), *value);
        BOOST_ASSERT(m_pEntity != NULL);*/
}

#pragma warning( pop )
