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

#include "Object/Object.h"
#include "Object/IGeometryObject.h"

class GraphicSystem;
class OGREGraphicsScene;
class OGREGraphicTask;

/**
 * Implementation of the IGraphicsObject interface. See Interfaces\Graphics.h and Interfaces\
 * System.h for a definition of the class and its functions.
 * 
 * @sa  GraphicObject
 * @sa  IGeometryObject
 */
class GraphicObjectCamera : public GraphicObject, public IGeometryObject {

        friend GraphicSystem;
        friend OGREGraphicsScene;
        friend OGREGraphicsTask;

    protected:

        /**
         * Constructor.
         *
         * @param [in,out]  pSystemScene    If non-null, the system scene.
         * @param   pszName                 The name.
         */
        GraphicObjectCamera(ISystemScene* pSystemScene, const char* pszName);

        /**
         * Destructor.
         */
        ~GraphicObjectCamera(void);

        /**
         * Initializes this GraphicObjectCamera.
         * Implementation of the <c>ISystem::Initialize</c> function.
         *
         * @param   Properties  Initializes the object with the properties specified by
         *                      <paramref name="Properties"/>.
         * @return  Error.
         *
         * @sa   GraphicObject::Initialize   .
         */
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /**
         * Gets the properties.
         * Implementation of the <c>ISystem::GetProperties</c> function.
         *
         * @param   Properties  Gets the properties of the object.
         *
         * @sa   ISystem::GetProperties  .
         */
        virtual void GetProperties(Properties::Array& Properties);

        /**
         * Sets the properties.
         * Implementation of the <c>ISystem::SetProperties</c> function.
         *
         * @param   Properties  Sets the properties of the object.
         *
         * @sa   ISystem::SetProperties  .
         */
        virtual void SetProperties(Properties::Array Properties);

        /**
         * Gets the desired system changes.
         * Implementation of the <c>IGeometryObject::GetDesiredSystemChanges</c> function.
         *
         * @return  System::Types::BitMask - System changes desired by the object.
         *
         * @sa   GraphicObject::GetSystemType    .
         */
        virtual System::Types::BitMask GetDesiredSystemChanges(void);

        /**
         * Change occurred.
         * Implementation of the <c>IObserver::ChangeOccurred</c> function.
         *
         * @param   pSubject    Subject of this notification.
         * @param   ChangeType  Type of notification for this object.
         * @return  Error.
         *
         * @sa   GraphicObject::ChangeOccurred   .
         */
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        /**
         * Updates the polygon mode based on m_PolygonMode.
         */
        void UpdatePolygonMode(void);

        /**
         * Updates the given DeltaTime.
         * Implementation of the <c>ISystemTask::Update</c> method.
         *
         * @param   DeltaTime   Elapsed time since the last frame.
         *
         * @sa   GraphicObject::Update   .
         */
        void Update(f32 DeltaTime);

    public:

        /**
         * Gets the position.
         * Implementation of the IGeometryObject GetPosition function.
         *
         * @return  null if it fails, else the position.
         *
         * @sa   IGeometryObject::GetPosition    .
         */
        virtual const Math::Vector3* GetPosition(void);

        /**
         * Gets the orientation.
         * Implementation of the IGeometryObject GetOrientation function.
         *
         * @return  null if it fails, else the orientation.
         *
         * @sa   IGeometryObject::GetOrientation .
         */
        virtual const Math::Quaternion* GetOrientation(void);

        /**
         * Gets the scale.
         * Implementation of the IGeometryObject GetScale function.
         *
         * @return  null if it fails, else the scale.
         *
         * @sa   IGeometryObject::GetScale   .
         */
        virtual const Math::Vector3* GetScale(void);

        /**
         * Gets the potential system changes.
         * Implementation of the <c>ISubject::GetPotentialSystemChanges</c> function.
         *
         * @return  System::Changes::BitMask - Returns systems changes possible for this object.
         *
         * @sa   GraphicObject::GetPotentialSystemChanges    .
         */
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

        static const const char*            sm_kapszPropertyNames[];
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

        static const const char*            sm_kapszPolygonModeEnumOptions[];
        PolygonModes                        m_PolygonMode;

        Math::Vector3                       m_Position;     // Position of AI object
        Math::Quaternion                    m_Orientation;  // Orientation of AI object
        Math::Vector3                       m_Scale;        // Scale of AI object

};

