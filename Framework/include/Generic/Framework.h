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
#include <boost/log/sources/logger.hpp>
#include <boost/system/error_code.hpp>
#include <boost/dll/shared_library.hpp>
#include <schema/system_type_generated.h>

#include "Manager/ChangeManager.h"
#include "Manager/TaskManager.h"
#include "Generic/Scheduler.h"
#include "Universal/UScene.h"
#include "System/ISystem.h"

///
/// Framework. Responsible for tying in all the different managers and systems.
///
class Framework {
public:

    Framework();

    ~Framework();

    boost::system::errc::errc_t Initialize();

    void Shutdown();

    bool Execute();

    void setWindowHandle(std::size_t windowHandle);

    std::size_t getWindowHandle();

    void setRunning(bool running);

private:

    void setNextScene(std::string nextSceneName);

private:
    boost::log::sources::logger logger_;

    TaskManager* taskManager_;
    Scheduler* m_pScheduler;
    ChangeManager* m_pSceneCCM;
    ChangeManager* m_pObjectCCM;

    UScene* m_pScene;
    std::string environmentFile_;
    const Schema::Environment* m_environment;
    std::map<Schema::SystemType, ISystem*> m_systems;
    std::vector<boost::dll::shared_library> m_systemLibraries;
    std::size_t windowHandle_;
    boost::shared_ptr<void> dynamicWorld_;
    bool running_;

};
