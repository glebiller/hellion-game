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

#include "ObjectLight.h"

///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   A class that extends GraphicObjectLight to simulate a flicking light.
/// </summary>
///////////////////////////////////////////////////////////////////////////////
class GraphicObjectLightFire : public GraphicObjectLight {

    public:

        GraphicObjectLightFire(ISystemScene* pSystemScene, pcstr pszName);
        ~GraphicObjectLightFire(void);

        /// <summary cref="GraphicObjectLightFire::Update">
        ///   Implementation of the <c>ISystemTask::Update</c> method.
        /// </summary>
        /// <param name="DeltaTime">Elapsed time since the last frame.</param>
        /// <seealso cref="GraphicObject::Update"/>
        virtual void Update(f32 DeltaTime);

    protected:

        /// <summary cref="GraphicObjectLightFire::Initialize">
        ///   Implementation of the <c>ISystem::Initialize</c> function.
        /// </summary>
        /// <param name="Properties">Initializes the object with the properties specified by <paramref name="Properties"/>.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObjectLight::Initialize"/>
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /// <summary cref="GraphicObjectLightFire::GetProperties">
        ///   Implementation of the <c>ISystem::GetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Gets the properties of the object</param>
        /// <seealso cref="GraphicObjectLight::GetProperties"/>
        virtual void GetProperties(Properties::Array& Properties);

        /// <summary cref="GraphicObjectLightFire::SetProperties">
        ///   Implementation of the <c>ISystem::SetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Sets the properties of the object</param>
        /// <seealso cref="GraphicObjectLight::SetProperties"/>
        virtual void SetProperties(Properties::Array Properties);

        /// <summary cref="GraphicObjectLightFire::GetDesiredSystemChanges">
        ///   Implementation of the <c>IGeometryObject::GetDesiredSystemChanges</c> function.
        /// </summary>
        /// <returns>System::Types::BitMask - System changes desired by the object.</returns>
        /// <seealso cref="GraphicObject::GetSystemType"/>
        virtual System::Types::BitMask GetDesiredSystemChanges(void);

        /// <summary cref="GraphicObjectLightFire::ChangeOccurred">
        ///   Implementation of the <c>IObserver::ChangeOccurred</c> function.
        /// </summary>
        /// <param name="pSubject">Subject of this notification.</param>
        /// <param name="ChangeType">Type of notification for this object.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObject::ChangeOccurred"/>
        virtual Error ChangeOccurred(ISubject* pSubject, System::Changes::BitMask ChangeType);

    protected:

        enum PropertyTypes {
            Property_Flare,
            Property_Flicker,
            Property_Count
        };

        static pcstr                        sm_kapszPropertyNames[];
        static const Properties::Property   sm_kaDefaultProperties[];

        Ogre::ColourValue m_BaseColor;
        Ogre::ColourValue m_BaseFlare;
        Ogre::ColourValue m_Flare;
        f32               m_Flicker;
        f32               m_TotalTime;
};

