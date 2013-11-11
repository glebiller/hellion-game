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

#include "Manager/ServiceManager.h"
#include "Listener/CustomLogListener.h"

extern IServiceManager* g_serviceManager;
    
/**
 * @inheritDoc
 */
CustomLogListener::CustomLogListener(void) {
};

/**
 * @inheritDoc
 */
CustomLogListener::~CustomLogListener(void) {
};
 
/**
 * @inheritDoc
 */
void CustomLogListener::messageLogged(const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String& logName, bool& skipThisMessage) {
    if (skipThisMessage) {
        return;
    }

    LOGOG_LEVEL_TYPE logLevel;
    switch (lml) {
    case Ogre::LogMessageLevel::LML_TRIVIAL:
        logLevel = LOGOG_LEVEL_DEBUG;
        break;
    case Ogre::LogMessageLevel::LML_NORMAL:
        logLevel = LOGOG_LEVEL_INFO;
        break;
    case Ogre::LogMessageLevel::LML_CRITICAL:
        logLevel = LOGOG_LEVEL_CRITICAL;
        break;
    }
    
    g_serviceManager->getLogService()->log(logLevel, "Ogre", message.c_str());
};