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

#pragma once

#include <OgreMesh.h>
#include <UniversalScene_generated.h>

#include "MathUtils.h"
#include "Object/Object.h"

class GraphicSystem;
class GraphicScene;
class GraphicTask;

/**
 * Implementation of the IGraphicsObject interface. See Interfaces\Graphics.h and Interfaces\
 * System.h for a definition of the class and its functions.
 * 
 * @sa  GuiObject
 */
class MeshGraphicObject : public GraphicObject {
    public:
        
        /**
         * @inheritDoc
         */
        MeshGraphicObject(ISystemScene& pSystemScene, UObject& entity, const Schema::SystemComponent& component);
        
        /**
         * @inheritDoc
         */
        ~MeshGraphicObject();

        /**
         * @inheritDoc
         */
        Error initialize();
        
        /**
         * @inheritDoc
         */
        void Update(f32 DeltaTime);
        
        /**
         * @inheritDoc
         */
        System::Changes::BitMask GetPotentialSystemChanges() {
            return System::Changes::None;
        };

        /**
         * @inheritDoc
         */
        System::Types::BitMask GetDesiredSystemChanges() {
            return System::Changes::Physic::Position | System::Changes::Physic::Orientation;
        };
        
        /**
         * @inheritDoc
         */
        Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

    protected:

        void setMeshName(std::string value);

    private:
        static u32                          sm_EntityId;

        Math::Vector3                       m_Position;
        Math::Quaternion                    m_Orientation;
        Math::Vector3                       m_Scale;

        Ogre::Entity*                       m_pEntity;

        std::vector<Ogre::String>           m_MaterialNames;

        std::string                         m_strStaticGrpName;
        // Index of the InstancedGeometry Object having this entity
        u32                                 m_InstancedGeomIdx;
        // Index of this entity in the list of entities in its InstancedGeom object
        u32                                 m_ObjectIdxinInstGeom;


        // Indicates whether the Instanced Geometry object should update its contents
        // for a change in position, orientation or scale of any of its parts.
        bool                                m_Dirty;

        std::string                         sMaterialName;
        bool                                bCastShadows;
        Ogre::MeshPtr                       pMesh;

        // Bitmask indicating vertex streams in use. Max of 32 streams.
        u32                                 m_StreamMask;

        bool                                isProcedural;
};


