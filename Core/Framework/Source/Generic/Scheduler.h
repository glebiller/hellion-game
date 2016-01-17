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

#include <boost/timer/timer.hpp>

namespace Schema {
    class Environment;
}
class RuntimeService;
class TaskManager;

///
/// Handles scheduling of all task execution.
///
class Scheduler {
public:

    ///
    /// Constructor.
    ///
    Scheduler();

    ///
    /// Destructor.
    ///
    ~Scheduler();

    ///
    /// Initialises this object.
    ///
    void init(const Schema::Environment* environment);

    ///
    /// Sets the UScene to schedule execution of.
    ///
    /// @param  pScene  A pointer to the universal scene.
    ///
    void setScene(const UScene* pScene);

    ///
    /// Execute the set UScene.
    ///
    void execute();

    ///
    /// Wait for scenes.
    ///
    void waitForScenes();

    ///
    /// Gets task manager.
    ///
    /// @return null if it fails, else the task manager.
    ///
    inline TaskManager* getTaskManager() {
        return m_pTaskManager;
    }

protected:
    static const boost::timer::nanosecond_type          sm_defaultClockFrequency;

    RuntimeService*                                     m_runtimeService;
    TaskManager*                                        m_pTaskManager;
    
    bool                                                m_benchmarkingEnabled;
    boost::timer::cpu_timer                             m_executionTimer;
    std::map<Schema::SystemType, ISystemScene*>          m_SceneExecs;

};
