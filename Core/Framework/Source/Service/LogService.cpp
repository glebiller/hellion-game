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

#include <boost/filesystem/operations.hpp>

#include "Service/LogService.h"

/**
 * @inheritDoc
 */
LogService::LogService() {
    logog::Initialize();
};

/**
 * @inheritDoc
 */
LogService::~LogService() {
    logog::Shutdown();
};

/**
 * Initialises the system.
 *
 * @param   type    The type.
 */
void LogService::initSystem(Proto::SystemType type) {
    logog::Initialize();
    std::string fileName = "logs/" + Proto::SystemType_Name(type) + ".log";
    boost::filesystem::remove(boost::filesystem::path(fileName));
    m_logFile[type] = new logog::LogFile(fileName.c_str());
};

/**
 * Closes a system.
 *
 * @param   type    The type.
 */
void LogService::closeSystem(Proto::SystemType type) {
    delete m_logFile[type];
    m_logFile.erase(type);
    logog::Shutdown();
};

/**
 * @inheritDoc
 */
void LogService::log(const LOGOG_LEVEL_TYPE level, const char* message, ...) {
    va_list args;
    va_start(args, message);
    log(level, nullptr, nullptr, message, args);
    va_end(args);
};

/**
 * @inheritDoc
 */
void LogService::log(const LOGOG_LEVEL_TYPE level, const char* group, const char* message, ...) {
    va_list args;
    va_start(args, message);
    log(level, group, nullptr, message, args);
    va_end(args);
};

/**
 * @inheritDoc
 */
void LogService::log(const LOGOG_LEVEL_TYPE level, const char* group, const char* category, const char* message, ...) {
    va_list args;
    va_start(args, message);
    LOGOG_LEVEL_GROUP_CATEGORY_MESSAGE(level, group, category, _LG(message), args);
    va_end(args);
};

