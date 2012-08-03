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

#include "ObjectMesh.h"

struct Animation {
    Interface::Behavior   m_Behavior;    // Behavior associated with this animation
    Ogre::AnimationState* m_State;       // Pointer to OGRE animation state

    f32                   m_Transition;  // Run transition while there is time left
    bool                  m_Disable;     // Animation should be disabled?
};


///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   A class that extends GraphicObjectMesh for meshes that animate.
/// </summary>
///////////////////////////////////////////////////////////////////////////////

class GraphicObjectMeshAnimated : public GraphicObjectMesh {
    public:
        GraphicObjectMeshAnimated(ISystemScene* pSystemScene, pcstr pszName);
        ~GraphicObjectMeshAnimated(void);

        /// <summary cref="GraphicObjectMeshAnimated::ChangeOccurred">
        /// Implementation of the <c>ISystemTask::Update</c> method.
        /// </summary>
        /// <param name="DeltaTime">Elapsed time since the last frame.</param>
        /// <seealso cref="GraphicObject::Update"/>
        virtual void Update(f32 DeltaTime);

    protected:

        /// <summary cref="GraphicObjectMeshAnimated::Initialize">
        ///   Implementation of the <c>ISystem::Initialize</c> function.
        /// </summary>
        /// <param name="Properties">Initializes the object with the properties specified by <paramref name="Properties"/>.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObjectMesh::Initialize"/>
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /// <summary cref="GraphicObjectMeshAnimated::GetProperties">
        ///   Implementation of the <c>ISystem::GetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Gets the properties of the object</param>
        /// <seealso cref="GraphicObjectMesh::GetProperties"/>
        virtual void GetProperties(Properties::Array& Properties);

        /// <summary cref="GraphicObjectMeshAnimated::SetProperties">
        ///   Implementation of the <c>ISystem::SetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Sets the properties of the object</param>
        /// <seealso cref="GraphicObjectMesh::SetProperties"/>
        virtual void SetProperties(Properties::Array Properties);

        /// <summary cref="GraphicObjectMeshAnimated::GetDesiredSystemChanges">
        ///   Implementation of the <c>IGeometryObject::GetDesiredSystemChanges</c> function.
        /// </summary>
        /// <returns>System::Types::BitMask - System changes desired by the object.</returns>
        /// <seealso cref="GraphicObjectMesh::GetSystemType"/>
        virtual System::Types::BitMask GetDesiredSystemChanges(void);

        /// <summary cref="GraphicObjectMeshAnimated::ChangeOccurred">
        ///   Implementation of the <c>IObserver::ChangeOccurred</c> function.
        /// </summary>
        /// <param name="pSubject">Subject of this notification.</param>
        /// <param name="ChangeType">Type of notification for this object.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObjectMesh::ChangeOccurred"/>
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        /// <summary cref="GraphicObjectMeshAnimated::GetPotentialSystemChanges">
        ///   Implementation of the <c>ISubject::GetPotentialSystemChanges</c> function.
        /// </summary>
        /// <returns>System::Changes::BitMask - Returns systems changes possible for this object.</returns>
        /// <seealso cref="GraphicObjectMesh::GetPotentialSystemChanges"/>
        virtual System::Changes::BitMask GetPotentialSystemChanges(void);

    protected:
        // Properties
        enum PropertyTypes {
            Property_Skeleton,
            Property_Animation,
            Property_Count
        };

        static pcstr                        sm_kapszAnimatedPropertyNames[];
        static const Properties::Property   sm_kaAnimatedDefaultProperties[];

        // Member variables
        std::list<Animation*> m_Animations;   // Collection of animations
        Math::Vector3         m_Velocity;     // Current velocity
        f32                   m_MaxVelocity;  // Max velocity
};

