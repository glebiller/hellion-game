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
/// This describes a layer of the paged geometry system. The paged geometry system can
/// support multiple instances of itself with multiple layers per instance. We limit
/// ourselves to one instance tied to the grass loader. Multiple layers can be added
/// which would allow for a layer of grass and a layer of flowers for example.
/// </summary>
///////////////////////////////////////////////////////////////////////////////

class GraphicObjectLayer : public GraphicObject {
        friend GraphicSystem;
        friend OGREGraphicsScene;


    protected:

        GraphicObjectLayer(ISystemScene* pSystemScene, pcstr pszName);
        ~GraphicObjectLayer(void);

        /// <summary cref="GraphicObjectLayer::Initialize">
        ///   Implementation of the <c>ISystem::Initialize</c> function.
        /// </summary>
        /// <param name="Properties">Initializes the object with the properties specified by <paramref name="Properties"/>.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="GraphicObject::Initialize"/>
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /// <summary cref="GraphicObjectLayer::GetProperties">
        ///   Implementation of the <c>ISystem::GetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Gets the properties of the object</param>
        /// <seealso cref="GraphicObject::GetProperties"/>
        virtual void GetProperties(Properties::Array& Properties);

        /// <summary cref="GraphicObjectLayer::SetProperties">
        ///   Implementation of the <c>ISystem::SetProperties</c> function.
        /// </summary>
        /// <param name="Properties">Sets the properties of the object</param>
        /// <seealso cref="GraphicObject::SetProperties"/>
        virtual void SetProperties(Properties::Array Properties);


    protected:

        enum PropertyTypes {
            Property_MinimumSize, Property_MaximumSize,
            Property_Animation, Property_Density,
            Property_MapBounds, Property_MaterialName,
            Property_Color, Property_RenderTechnique,
            Property_Count
        };

        static pcstr                        sm_kapszPropertyNames[];
        static const Properties::Property   sm_kaDefaultProperties[];

        Forests::GrassLayer*                m_pLayer;
};
