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
#include <vector>

#include "Assert.h"
#include "Proto.h"
#include "System/ISystem.h"
#include "System/ISystemObject.h"

class UScene;

/**
 * Gdf parser.
 */
class DefinitionService {
public:
    /**
     * Constructor.
     *
     * @param [in,out]  pScene  If non-null, the scene.
     * @param   sGDF            The gdf.
     */
    DefinitionService(UScene* pScene);

    /**
     * Destructor.
     */
    ~DefinitionService();

    /**
     * Parse environment.
     */
    void parseEnvironment();

    /**
     * Parse systems.
     */
    void parseSystems();

    /**
     * Parse scene.
     *
     * @param   sScene  The scene.
     */
    void parseScene(std::string sScene);

    /**
     * Gets the startup scene.
     *
     * @return The startup scene.
     */
    std::string getStartupScene(void);

private:
    /**
     * Loads a prototype.
     *
     * @param   pszFile         The file.
     * @param [in,out]  proto   If non-null, the prototype.
     *
     * @return  The prototype.
     */
    Error loadProto(std::string file, google::protobuf::Message* proto);

    /**
     * Loads system library.
     *
     * @param   type                The type.
     * @param [in,out]  ppSystem    If non-null, the system.
     */
    Error loadSystemLibrary(Proto::SystemType type, ISystem** ppSystem);
    
    struct SystemLib {
        Handle      hLib;
        ISystem*    pSystem;
    };

    std::vector<SystemLib>                          m_systemLibs;
    Proto::Application                              m_gdProto;
    UScene*                                         m_pScene;
    ISystem*                                        m_pSystem;
    ISystemScene*                                   m_pSystemScene;

};