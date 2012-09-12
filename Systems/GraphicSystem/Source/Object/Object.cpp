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
#include "Object.h"

#define POGREROOTNODE (reinterpret_cast<OGREGraphicsScene*>(m_pSystemScene)->GetOGRERootSceneNode())


/**
 * @inheritDoc
 */
GraphicObject::GraphicObject(ISystemScene* pSystemScene, const char* pszName) : ISystemObject(pSystemScene, pszName) {
    
}

/**
 * @inheritDoc
 */
GraphicObject::~GraphicObject(void) {
    POGREROOTNODE->removeChild(m_pNode);
}

/**
 * @inheritDoc
 */
Error GraphicObject::initialize(void) {
    ASSERT(!m_bInitialized);

    m_pNode = POGREROOTNODE->createChildSceneNode(m_sName + "_SceneNode");
    ASSERT(m_pNode != NULL);

    return Errors::Success;
}

namespace {

    // Provides access to protected members of the Ogre::SceneNode class
    // in order to check if the locking is necessary
    class AccessOgreSceneNode : public Ogre::SceneNode {
            // Never use this class
            AccessOgreSceneNode(const AccessOgreSceneNode&);
            AccessOgreSceneNode();
            const AccessOgreSceneNode& operator= (const AccessOgreSceneNode&);
        public:

            bool NeedsLocking() {
                return mParent && !mParentNotified;
            }
    };

    inline
    bool NeedsLocking(Ogre::SceneNode* pNode) {
        return reinterpret_cast<AccessOgreSceneNode*>(pNode)->NeedsLocking();
    }

} // anonymous namespace


inline void UpdateGeometry(Ogre::SceneNode* pNode, System::Changes::BitMask ChangeType, IGeometryObject* pGeometryObject) {
    if (ChangeType & System::Changes::Geometry::Position) {
        const Math::Vector3& Position = *pGeometryObject->GetPosition();
        pNode->setPosition(Position.x, Position.y, Position.z);
    }

    if (ChangeType & System::Changes::Geometry::Orientation) {
        const Math::Quaternion& Orientation = *pGeometryObject->GetOrientation();
        pNode->setOrientation(Orientation.w, Orientation.x, Orientation.y, Orientation.z);
    }

    if (ChangeType & System::Changes::Geometry::Scale) {
        const Math::Vector3& Scale = *pGeometryObject->GetScale();
        pNode->setScale(Scale.x, Scale.y, Scale.z);
    }
}

/**
 * @inheritDoc
 */
Error GraphicObject::ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType) {
    ASSERT(m_bInitialized);

    if (!m_pNode) {
        return Errors::Success;
    }

    if (ChangeType & System::Changes::Geometry::All) {
        IGeometryObject* pGeometryObject = dynamic_cast<IGeometryObject*>(pSubject);

        if (NeedsLocking(m_pNode)) {
            SCOPED_SPIN_LOCK(OGREGraphicsScene::m_mutex);
            UpdateGeometry(m_pNode, ChangeType, pGeometryObject);
        } else {
            UpdateGeometry(m_pNode, ChangeType, pGeometryObject);
        }
    }

    return Errors::Success;
}
