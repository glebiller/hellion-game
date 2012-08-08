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

#include <string>

#include "Assert.h"
#include "Proto.h"
#include "Universal.h"
#include "System\ISystem.h"
#include "System\ISystemObject.h"

/**
 * Gdf parser.
 */
class DefinitionParser {
    public:

        /**
         * Constructor.
         *
         * @param [in,out]  pScene  If non-null, the scene.
         * @param   sGDF            The gdf.
         */
        DefinitionParser(UScene* pScene, std::string sGDF);

        /**
         * Parse environment.
         */
        void ParseEnvironment(void);

        /**
         * Parse systems.
         */
        void ParseSystems(void);

        /**
         * Parse scene.
         *
         * @param   sScene  The scene.
         */
        void ParseScene(std::string sScene);

        /**
         * Gets the startup scene.
         *
         * @return The startup scene.
         */
        std::string StartupScene(void);

        /**
         * Assignment operator.
         *
         * @param   parameter1  The first parameter.
         * @return  A shallow copy of this DefinitionParser.
         */
        DefinitionParser& operator=(const DefinitionParser&) {
            ASSERT(false);
        }

    protected:

        void ApplyProperties(
            ProtoPropertyList& PropertyList,
            Properties::Array& GetProperties,
            Properties::Array& SetProperties
        ) const;


    protected:

        GlobalDefinitionProto                           m_gdProto;

        UScene*                                         m_pScene;
        ISystem*                                        m_pSystem;
        ISystemScene*                                   m_pSystemScene;

        UObject*                                        m_pUObject;
        ISystemObject*                                  m_pSystemObject;

        Properties::Array                               m_GetProperties;
        Properties::Array                               m_SetProperties;

        std::map<u32, Properties::Array>                 m_SetPropertiesMap;
        std::map<UObject*, std::map<u32, Properties::Array>>  m_AllObjectProperties;

};