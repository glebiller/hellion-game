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

#include "Environment_generated.h"
#include "System/ISystem.h"
#include "System/ISystemObject.h"

class UScene;

class DefinitionService {
public:
    DefinitionService();

    ~DefinitionService();

    void parseEnvironment();

    void parseSystems();

    void parseScene(UScene* scene, std::string sceneName);

    std::string getStartupScene();

private:
    Error loadProto(std::string file, std::string* proto);

    Error loadSystemLibrary(const std::string type);
    
    struct SystemLib {
        void*      hLib;
        ISystem*    pSystem;
    };

    std::vector<SystemLib>                          m_systemLibs;
    const Schema::Environment*                      m_gdProto;
    ISystem*                                        m_pSystem;
    ISystemScene*                                   m_pSystemScene;

};