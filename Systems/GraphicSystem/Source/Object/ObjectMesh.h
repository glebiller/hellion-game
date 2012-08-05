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


class GraphicSystem;
class OGREGraphicsScene;
class OGREGraphicsTask;
class DynamicLines;
class NormalizedLines;

#include "MovableText/MovableText.h"


///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   A class that extends GraphicObject for mesh objects.
/// </summary>
///////////////////////////////////////////////////////////////////////////////

class GraphicObjectMesh : public GraphicObject, public IGraphicsObject {
        friend GraphicSystem;
        friend OGREGraphicsScene;
        friend OGREGraphicsTask;

    protected:

        GraphicObjectMesh(ISystemScene* pSystemScene, const char* pszName);
        ~GraphicObjectMesh(void);

        /// <summary cref="GraphicObjectMesh::Initialize">
        ///   Implementation of the <c>ISystem::Initialize</c> function.
        /// </summary>
        /// <param name="Properties">Initializes the object with the properties specified by <paramref name="Properties"/>.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObject::Initialize"/>
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /// <summary cref="GraphicObjectMesh::GetProperties">
        ///   Implementation of the <c>ISystem::GetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Gets the properties of the object</param>
        /// <seealso cref="GraphicObject::GetProperties"/>
        virtual void GetProperties(Properties::Array& Properties);

        /// <summary cref="GraphicObjectMesh::SetProperties">
        ///   Implementation of the <c>ISystem::SetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Sets the properties of the object</param>
        /// <seealso cref="GraphicObject::SetProperties"/>
        virtual void SetProperties(Properties::Array Properties);

        /// <summary cref="GraphicObjectMesh::GetDesiredSystemChanges">
        ///   Implementation of the <c>IGeometryObject::GetDesiredSystemChanges</c> function.
        /// </summary>
        /// <returns>System::Types::BitMask - System changes desired by the object.</returns>
        /// <seealso cref="GraphicObject::GetSystemType"/>
        virtual System::Types::BitMask GetDesiredSystemChanges(void);

        /// <summary cref="GraphicObjectMesh::ChangeOccurred">
        ///   Implementation of the <c>IObserver::ChangeOccurred</c> function.
        /// </summary>
        /// <param name="pSubject">Subject of this notification.</param>
        /// <param name="ChangeType">Type of notification for this object.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObject::ChangeOccurred"/>
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        /// <summary cref="GraphicObjectMesh::GetPotentialSystemChanges">
        ///   Implementation of the <c>ISubject::GetPotentialSystemChanges</c> function.
        /// </summary>
        /// <returns>System::Changes::BitMask - Returns systems changes possible for this object.</returns>
        /// <seealso cref="GraphicObject::GetPotentialSystemChanges"/>
        virtual System::Changes::BitMask GetPotentialSystemChanges(void);

        /// <summary cref="GraphicObjectMesh::GetSubMeshCount">
        ///   Implementation of the IGraphicsObject GetSubMeshCount function.
        /// </summary>
        /// <seealso cref="IGraphicsObject::GetSubMeshCount"/>
        virtual u32 GetSubMeshCount(void);

        /// <summary cref="GraphicObjectMesh::GetIndexDeclaration">
        ///   Implementation of the IGraphicsObject GetIndexDeclaration function.
        /// </summary>
        /// <seealso cref="IGraphicsObject::GetIndexDeclaration"/>
        virtual u32 GetIndexDeclaration(In  u16 nSubMeshIndex = 0);

        /// <summary cref="GraphicObjectMesh::GetVertexDeclarationCount">
        ///   Implementation of the IGraphicsObject GetVertexDeclarationCount function.
        /// </summary>
        /// <seealso cref="IGraphicsObject::GetVertexDeclarationCount"/>
        virtual u32 GetVertexDeclarationCount(In  u16 nSubMeshIndex = 0);

        /// <summary cref="GraphicObjectMesh::GetVertexDeclaration">
        ///   Implementation of the IGraphicsObject GetVertexDeclaration function.
        /// </summary>
        /// <seealso cref="IGraphicsObject::GetVertexDeclaration"/>
        virtual void GetVertexDeclaration(Out VertexDecl::Element* pVertexDecl,
                                          In  u16 nSubMeshIndex = 0);

        /// <summary cref="GraphicObjectMesh::GetIndexCount">
        ///   Implementation of the IGraphicsObject GetIndexCount function.
        /// </summary>
        /// <seealso cref="GraphicObjectMesh::GetIndexCount"/>
        virtual u32 GetIndexCount(In  u16 nSubMeshIndex = 0);

        /// <summary cref="GraphicObjectMesh::GetVertexCount">
        ///   Implementation of the IGraphicsObject GetVertexCount function.
        /// </summary>
        /// <seealso cref="IGraphicsObject::GetVertexCount"/>
        virtual u32 GetVertexCount(In  u16 nSubMeshIndex = 0);

        /// <summary cref="GraphicObjectMesh::GetIndices">
        ///   Implementation of the IGraphicsObject GetIndices function.
        /// </summary>
        /// <seealso cref="IGraphicsObject::GetIndices"/>
        virtual void GetIndices(Out void* pIndices,
                                In  u16 nSubMeshIndex = 0);

        /// <summary cref="GraphicObjectMesh::GetVertices">
        ///   Implementation of the IGraphicsObject GetVertices function.
        /// </summary>
        /// <seealso cref="IGraphicsObject::GetVertices"/>
        virtual void GetVertices(Out void* pVertices,
                                 In  u16 nSubMeshIndex = 0,
                                 In  u16 nStreamIndex = 0,
                                 In  u32 nVertexDeclCount = 0,
                                 In  VertexDecl::Element* pVertexDecl = NULL);

        /// <summary cref="GraphicObjectMesh::GetStreamsChanged">
        ///   Implementation of the IGraphicsObject GetStreamsChanged function.
        /// </summary>
        /// <seealso cref="IGraphicsObject::GetStreamsChanged"/>
        virtual u32 GetStreamsChanged(void);

        /// <summary cref="GraphicObjectMesh::GetAABB">
        ///   Implementation of the IGraphicsObject GetAABB function.
        /// </summary>
        /// <seealso cref="IGraphicsObject::GetAABB"/>
        virtual void GetAABB(Out Math::Vector3& Min, Out Math::Vector3& Max);

        /// <summary cref="GraphicObjectMesh::SetupNormalsAndTangentsDisplay">
        ///   Generate data need to display normals and tangets for this mesh.
        /// </summary>
        void SetupNormalsAndTangentsDisplay(void);

        /// <summary cref="GraphicObjectMesh::SetupCaptions">
        ///   Generate data need to display caption (names) for this mesh.
        /// </summary>
        void SetupCaptions(void);

    private:
        template<class IdxType> void BuildNormalsTemplate(u32 nSubMesh);

        /// <summary cref="GraphicObjectMesh::GeometryChanged">
        ///   Process a geometry (position, scale, orientation) change (called from ChangeOccurred).
        /// </summary>
        /// <seealso cref="GraphicObjectMesh::ChangeOccurred"/>
        void GeometryChanged(System::Changes::BitMask ChangeType, IGeometryObject* pGeometryObject);

    protected:

        static u32                          sm_EntityId;

        enum PropertyTypes {
            Property_Mesh, Property_ProceduralMesh, Property_Material,
            Property_CastShadows, Property_DrawBoundingBox,
            Property_ShowNormals, Property_ShowTangents,
            Property_StaticGeom, Property_Instance,
            Property_Count
        };

        static const char*                        sm_kapszPropertyNames[];
        static const Properties::Property   sm_kaDefaultProperties[];

        Math::Vector3                       m_Position;
        Math::Quaternion                    m_Orientation;
        Math::Vector3                       m_Scale;

        Ogre::Entity*                       m_pEntity;

        std::vector<NormalizedLines*>       m_pNormals;
        std::vector<NormalizedLines*>       m_pTangents;

        Ogre::MovableText*                  m_pCaption;

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


