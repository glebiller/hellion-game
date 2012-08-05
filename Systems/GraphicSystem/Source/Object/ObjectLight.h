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
///   A class to wrap Ogre light functionality.
/// </summary>
///////////////////////////////////////////////////////////////////////////////
class GraphicObjectLight : public GraphicObject {
        friend GraphicSystem;
        friend OGREGraphicsScene;


    protected:

        GraphicObjectLight(ISystemScene* pSystemScene, const char* pszName);
        ~GraphicObjectLight(void);

        /// <summary cref="GraphicObjectLight::Initialize">
        ///   Implementation of the <c>ISystem::Initialize</c> function.
        /// </summary>
        /// <param name="Properties">Initializes the object with the properties specified by <paramref name="Properties"/>.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObject::Initialize"/>
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /// <summary cref="GraphicObjectLight::GetProperties">
        ///   Implementation of the <c>ISystem::GetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Gets the properties of the object</param>
        /// <seealso cref="ISystem::GetProperties"/>
        virtual void GetProperties(Properties::Array& Properties);

        /// <summary cref="GraphicObjectLight::SetProperties">
        ///   Implementation of the <c>ISystem::SetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Sets the properties of the object</param>
        /// <seealso cref="ISystem::SetProperties"/>
        virtual void SetProperties(Properties::Array Properties);


    protected:

        enum LightTypes {
            LightType_Invalid = -1,
            LightType_Point,
            LightType_Directional,
            LightType_Spot,
            LightType_Count,
        };

        static const const char*                  sm_kapszLightTypeEnumOptions[];

        LightTypes                          m_LightType;

        enum PropertyTypes {
            Property_Type, Property_Position,
            Property_Diffuse, Property_Specular,
            Property_Direction, Property_Range, Property_Attenuation,
            Property_Count
        };

        static const char*                        sm_kapszPropertyNames[];
        static const Properties::Property   sm_kaDefaultProperties[];

        Ogre::Light*                        m_pLight;
};

