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

//
// core includes
//
#include <BaseTypes.h>
#include <Interface.h>

#pragma warning( push, 0 )
// Temporarily switching warning level to 0 to ignore warnings in extern/Ogre
#include "Ogre.h"
#include "OgreParticleSystem.h"
#pragma warning( pop )

class GraphicSystem;
class OGREGraphicsScene;


////////////////////////////////////////////////////////////////////////////////////////////////////
/// <summary>
///   <c>GraphicObjectParticleSystem</c>Graphics system object wrapping the Ogre ParticleFX particle system.
/// </summary>
////////////////////////////////////////////////////////////////////////////////////////////////////

class GraphicObjectParticleSystem : public GraphicObject, public IGeometryObject {
        friend GraphicSystem;
        friend OGREGraphicsScene;
        friend OGREGraphicsTask;

    protected:

        /// <summary cref="GraphicObjectParticleSystem::GraphicObjectParticleSystem">
        /// ctor
        /// </summary>
        /// <param name="pSystemScene">Pointer to the <c>ISystemScene</c> object.</param>
        /// <param name="pszName">Name of the GraphicObjectParticleSystem object.</param>
        GraphicObjectParticleSystem(ISystemScene* pSystemScene, const char* pszName);

        /// <summary cref="GraphicObjectParticleSystem::~GraphicObjectParticleSystem">
        /// dtor
        /// </summary>
        ~GraphicObjectParticleSystem(void);

        /// <summary cref="GraphicObjectParticleSystem::Initialize">
        /// Implementation of the <c>ISystem::Initialize</c> method.
        /// </summary>
        /// <param name="Properties">Property structure array to fill in</param>
        /// <seealso cref="ISystem::Initialize"/>
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /// <summary cref="GraphicObjectParticleSystem::GetProperties">
        /// Implementation of the <c>ISystem::GetProperties</c> method.
        /// </summary>
        /// <param name="Properties">Get properties for this GraphicObjectParticleSystem object.</param>
        /// <seealso cref="ISystem::GetProperties"/>
        virtual void GetProperties(Properties::Array& Properties);

        /// <summary cref="GraphicObjectParticleSystem::SetProperties">
        /// Implementation of the <c>ISystem::SetProperties</c> method.
        /// </summary>
        /// <param name="Properties">Set properties for this GraphicObjectParticleSystem object.</param>
        /// <seealso cref="ISystem::SetProperties"/>
        virtual void SetProperties(Properties::Array Properties);

        /// <summary cref="GraphicObjectParticleSystem::GetDesiredSystemChanges">
        /// Implementation of the <c>ISubject::GetDesiredSystemChanges</c> method.
        /// </summary>
        /// <returns>Returns systems changes requested by this GraphicObjectParticleSystem object.
        /// </returns>
        /// <seealso cref="ISubject::GetDesiredSystemChanges"/>
        virtual System::Types::BitMask GetDesiredSystemChanges(void);

        /// <summary cref="GraphicObjectParticleSystem::GetPotentialSystemChanges">
        /// Implementation of the <c>ISubject::GetPotentialSystemChanges</c> method.
        /// </summary>
        /// <returns>Returns systems changes possible for this GraphicObjectParticleSystem object.
        /// </returns>
        /// <seealso cref="ISubject::GetPotentialSystemChanges"/>
        virtual System::Changes::BitMask GetPotentialSystemChanges(void);

        /// <summary cref="GraphicObjectParticleSystem::ChangeOccurred">
        /// Implementation of the <c>IObserver::ChangeOccurred</c> method.
        /// </summary>
        /// <param name="pSubject">Subject of this notification.</param>
        /// <param name="ChangeType">Type of notification for this object.</param>
        /// <returns>Error
        /// </returns>
        /// <seealso cref="IObserver::ChangeOccurred"/>
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

        /// <summary cref="GraphicObjectParticleSystem::ChangeOccurred">
        /// Implementation of the <c>ISystemTask::Update</c> method.
        /// </summary>
        /// <param name="DeltaTime">Elapsed time since the last frame.</param>
        /// <seealso cref="ISystemTask::Update"/>
        void Update(f32 DeltaTime);

    public:
        /// <summary cref="GraphicObjectParticleSystem::GetPosition">
        /// Implementation of the <c>IGeometryObject::GetPosition</c> method.
        /// </summary>
        /// <returns>Position of the object.
        /// </returns>
        /// <seealso cref="IGeometryObject::GetPosition"/>
        virtual const Math::Vector3* GetPosition(void);
        /// <summary cref="GraphicObjectParticleSystem::GetOrientation">
        /// Implementation of the <c>IGeometryObject::GetOrientation</c> method.
        /// </summary>
        /// <returns>Orientation of the object.
        /// </returns>
        /// <seealso cref="IGeometryObject::GetOrientation"/>
        virtual const Math::Quaternion* GetOrientation(void);
        /// <summary cref="GraphicObjectParticleSystem::GetScale">
        /// Implementation of the <c>IGeometryObject::GetScale</c> method.
        /// </summary>
        /// <returns>Scale of the object.
        /// </returns>
        /// <seealso cref="IGeometryObject::GetScale"/>
        virtual const Math::Vector3* GetScale(void);

    protected:

        static u32                          sm_EntityId;

        /// <summary >
        /// GraphicObjectParticleSystem object PropertyTypes type enum supported in this class.
        /// </summary >
        enum PropertyTypes {
            Property_PSystemType,
            Property_PSystemScripts,
            Property_Count
        };

        /// <summary >
        /// GraphicObjectParticleSystem object ParticleSystemType type enum denotes special handling for particle systems.
        /// </summary >
        enum ParticleSystemType {
            Type_FireHose,
            Type_Default,
            Type_Count
        };

        /// <summary >
        /// GraphicObjectParticleSystem object Property name array
        /// </summary >
        static const char*                  sm_kapszPropertyNames[];

        /// <summary >
        /// GraphicObjectParticleSystem object Property array of default values
        /// </summary >
        static const Properties::Property   sm_kaDefaultProperties[];

        /// <summary >
        /// GraphicObjectParticleSystem object Ogre3D particle system from the ParticleFX plugin.
        /// </summary >
        Ogre::ParticleSystem*               m_pParticleSystem;

        /// <summary >
        /// GraphicObjectParticleSystem object position.
        /// </summary >
        Math::Vector3                       m_Position;

        /// <summary >
        /// GraphicObjectParticleSystem object last position recorded for this object used to calculate a direction (velocity) vector.
        /// </summary >
        Math::Vector3                       m_LastPosition;

        /// <summary >
        /// GraphicObjectParticleSystem object orientation quaternion.
        /// </summary >
        Math::Quaternion                    m_Orientation;

        /// <summary >
        /// GraphicObjectParticleSystem object scale.
        /// </summary >
        Math::Vector3                       m_Scale;

        /// <summary >
        /// GraphicObjectParticleSystem object Ogre3D particle system material string ID name.
        /// (This value must be unique from all other particle system objects in the scene).
        /// </summary >
        std::string                         m_strPSystemName;

        /// <summary >
        /// GraphicObjectParticleSystem object Ogre3D particle system .particle source script
        /// (This is the script name not the file name).
        /// </summary >
        std::string                         m_strPSystemSource;

        /// <summary >
        /// GraphicObjectParticleSystem object denotes special handling for the particle system.
        /// E.g. Type_FireHose receives special updates from the Input system, and camera positioning and
        /// orientation updates whereas Type_Default receives position updates only from whatever object it tracks
        /// from the .cdf link section.
        /// </summary >
        ParticleSystemType                  m_PSystemType;

        /// <summary >
        /// GraphicObjectParticleSystem object Ogre3D particle system emission rate as read from the script file.
        /// </summary >
        Ogre::Real                          m_emissionRate;

        /// <summary >
        /// GraphicObjectParticleSystem object specifies whether or not to emit particles.
        /// (This does not destroy the particle system, but rather sets the emission rate to 0.
        /// </summary >
        bool                                m_bEmitParticles;
};

