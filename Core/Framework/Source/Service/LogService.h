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

#include <vector>
#include <logog.hpp>

#include "Defines.h"
#include "Proto/Common.pb.h"

/**
 * LogManager class.
 */
class LogService {
public:

    /**
     * Constructor.
     */
    LogService();

    /**
     * Destructor.
     */
    ~LogService();

    /**
     * Initialises the system.
     *
     * @param   type    The type.
     */
    void initSystem(Proto::SystemType type);

    /**
     * Closes a system.
     *
     * @param   type    The type.
     */
    void closeSystem(Proto::SystemType type);

    /**
     * Logs.
     *
     * @param   type    The type.
     * @param   message The message.
     */
    void log(const LOGOG_LEVEL_TYPE level, const char* message, ...);

    /**
     * Logs.
     *
     * @param   level   The level.
     * @param   group   The group.
     * @param   message The message.
     */
    void log(const LOGOG_LEVEL_TYPE level, const char* group, const char* message, ...);

    /**
     * Logs.
     *
     * @param   level       The level.
     * @param   group       The group.
     * @param   category    The category.
     * @param   message     The message.
     */
    void log(const LOGOG_LEVEL_TYPE level, const char* group, const char* category, const char* message, ...);

private:
    std::map<Proto::SystemType, logog::LogFile*>    m_logFile;

};

