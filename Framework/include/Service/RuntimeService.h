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
#include <boost/noncopyable.hpp>

/**
 * An interface for environment runtime functionality.
 */
class RuntimeService : public boost::noncopyable {
public:
    enum Status {
        Unknown, Running, Paused, NextScene, Quit
    };

    ///
    /// Default constructor.
    ///
    RuntimeService();

    ///
    /// Gets the status.
    ///
    /// @return The status.
    ///
    Status getStatus();

    ///
    /// Sets the status.
    ///
    /// @param  Status  The status.
    ///
    void setStatus(Status Status);

    ///
    /// Sets next scene.
    ///
    /// @param  sceneName   Name of the scene.
    ///
    void setNextScene(std::string sceneName);

    ///
    /// Query if this object is paused.
    ///
    /// @return true if paused, false if not.
    ///
    bool isPaused();

    ///
    /// Query if this object is quit.
    ///
    /// @return true if quit, false if not.
    ///
    bool isQuit();

    ///
    /// Query if this object is next scene.
    ///
    /// @return true if next scene, false if not.
    ///
    bool isNextScene();

    ///
    /// Gets scene name.
    ///
    /// @return The scene name.
    ///
    std::string getSceneName();

private:
    Status          m_runtimeStatus;
    std::string     m_sceneName;

};