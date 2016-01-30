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

#include <map>
#include <string>
#include <vector>
#include <boost/system/error_code.hpp>
#include <boost/dll.hpp>

#include "System/Types.h"
#include "Manager/IServiceManager.h"
#include "Service/RuntimeService.h"

namespace Schema {
    class Environment;
}
class DefinitionService;
class ServiceManager;
class Scheduler;
class ChangeManager;
class UScene;
class UObject;
class Gdf;
class ISystem;

///
/// Framework. Responsible for tying in all the different managers and systems.  Also handles
/// loading and parsing of the global definition file (gdf).
///
class Framework {
public:

    ///
    /// Default constructor.
    ///
    Framework();

    ///
    /// Destructor.
    ///
    ~Framework();

    ///
    /// Initializes this :.
    ///
    /// @return Error status.
    ///
    boost::system::errc::errc_t Initialize();

    ///
    /// Shuts down this : and frees any resources it is using.
    ///
    void Shutdown();

    ///
    /// Executes the framework..
    ///
    /// @return An error code.
    ///
    Error Execute();

protected:

    ///
    /// Process the messages.
    ///
    void processMessages();

    ///
    /// Sets next scene.
    ///
    void setNextScene(std::string nextSceneName);

    /**
     * Issues all the pending property changes.  This only occurs after the scheduler has
     *  executed all the waiting items.
     *
     * @param   SystemTypes The system types of the system that can accept property changes.
     */
    void IssuePendingSystemPropertyChanges(System::Types::BitMask SystemTypes = System::Types::All);
    
private:
    ServiceManager*                         m_serviceManager;
    DefinitionService*                      m_definitionService;

    Scheduler*                              m_pScheduler;
    ChangeManager*                          m_pSceneCCM;
    ChangeManager*                          m_pObjectCCM;


    UScene*                                 m_pScene;
    const Schema::Environment*              m_environment;
    std::map<Schema::SystemType, ISystem*>  m_systems;
    std::vector<boost::dll::shared_library>  m_systemLibraries;

};
