// Copyright © 2008-2009 Intel Corporation
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


///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   Implementation of the IGraphicsObject interface.
///   See Interfaces\Graphics.h and Interfaces\System.h for a definition of the
///   class and its functions.
/// </summary>
///////////////////////////////////////////////////////////////////////////////

class GraphicObjectCamera : public GraphicObject, public IGeometryObject {
        friend GraphicSystem;
        friend OGREGraphicsScene;
        friend OGREGraphicsTask;

    protected:

        GraphicObjectCamera(ISystemScene* pSystemScene, const char* pszName);
        ~GraphicObjectCamera(void);

        /// <summary cref="GraphicObjectCamera::Initialize">
        ///   Implementation of the <c>ISystem::Initialize</c> function.
        /// </summary>
        /// <param name="Properties">Initializes the object with the properties specified by <paramref name="Properties"/>.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObject::Initialize"/>
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /// <summary cref="GraphicObjectCamera::GetProperties">
        ///   Implementation of the <c>ISystem::GetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Gets the properties of the object</param>
        /// <seealso cref="ISystem::GetProperties"/>
        virtual void GetProperties(Properties::Array& Properties);

        /// <summary cref="GraphicObjectCamera::SetProperties">
        ///   Implementation of the <c>ISystem::SetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Sets the properties of the object</param>
        /// <seealso cref="ISystem::SetProperties"/>
        virtual void SetProperties(Properties::Array Properties);

        /// <summary cref="GraphicObjectCamera::GetDesiredSystemChanges">
        ///   Implementation of the <c>IGeometryObject::GetDesiredSystemChanges</c> function.
        /// </summary>
        /// <returns>System::Types::BitMask - System changes desired by the object.</returns>
        /// <seealso cref="GraphicObject::GetSystemType"/>
        virtual System::Types::BitMask GetDesiredSystemChanges(void);

        /// <summary cref="GraphicObjectCamera::ChangeOccurred">
        ///   Implementation of the <c>IObserver::ChangeOccurred</c> function.
        /// </summary>
        /// <param name="pSubject">Subject of this notification.</param>
        /// <param name="ChangeType">Type of notification for this object.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObject::ChangeOccurred"/>
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        /// <summary cref="GraphicObjectCamera::UpdatePolygonMode">
        ///   Updates the polygon mode based on m_PolygonMode.
        /// </summary>
        void UpdatePolygonMode(void);

        /// <summary cref="GraphicObjectCamera::Update">
        ///   Implementation of the <c>ISystemTask::Update</c> method.
        /// </summary>
        /// <param name="DeltaTime">Elapsed time since the last frame.</param>
        /// <seealso cref="GraphicObject::Update"/>
        void Update(f32 DeltaTime);

    public:
        /// <summary cref="GraphicObjectCamera::GetPosition">
        ///   Implementation of the IGeometryObject GetPosition function.
        /// </summary>
        /// <seealso cref="IGeometryObject::GetPosition"/>
        virtual const Math::Vector3* GetPosition(void);

        /// <summary cref="GraphicObjectCamera::GetOrientation">
        ///   Implementation of the IGeometryObject GetOrientation function.
        /// </summary>
        /// <seealso cref="IGeometryObject::GetOrientation"/>
        virtual const Math::Quaternion* GetOrientation(void);

        /// <summary cref="GraphicObjectCamera::GetScale">
        ///   Implementation of the IGeometryObject GetScale function.
        /// </summary>
        /// <seealso cref="IGeometryObject::GetScale"/>
        virtual const Math::Vector3* GetScale(void);

        /// <summary cref="GraphicObjectCamera::GetPotentialSystemChanges">
        ///   Implementation of the <c>ISubject::GetPotentialSystemChanges</c> function.
        /// </summary>
        /// <returns>System::Changes::BitMask - Returns systems changes possible for this object.</returns>
        /// <seealso cref="GraphicObject::GetPotentialSystemChanges"/>
        virtual System::Changes::BitMask GetPotentialSystemChanges(void);


    protected:

        //
        // Camera Graphics Object
        //
        enum PropertyTypes {
            Property_FOVy, Property_ClipDistances, Property_LookAt,
            Property_PolygonMode, Property_LockCamera, Property_PagedGeometry,
            Property_Count
        };

        static const const char*                  sm_kapszPropertyNames[];
        static const Properties::Property   sm_kaDefaultProperties[];

        Ogre::Camera*                       m_pCamera;
        Ogre::SceneNode*                    m_pCameraNode;
        Ogre::Viewport*                     m_pViewport;

        bool                                m_bLocked;
        Math::Vector3                       m_vLookAt;

        System::Types::BitMask              m_Modified;

        enum PolygonModes {
            PolygonMode_Invalid = -1,

            PolygonMode_Points, PolygonMode_WireFrame, PolygonMode_Solid,
            PolygonMode_Count
        };

        static const const char*                  sm_kapszPolygonModeEnumOptions[];
        PolygonModes                        m_PolygonMode;

        Math::Vector3                       m_Position;     // Position of AI object
        Math::Quaternion                    m_Orientation;  // Orientation of AI object
        Math::Vector3                       m_Scale;        // Scale of AI object

};

