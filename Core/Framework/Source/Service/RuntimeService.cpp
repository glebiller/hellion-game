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

#include "Service/RuntimeService.h"

#include "Defines.h"
#if defined(MSC_COMPILER)
#include <Windows.h>
#endif

///
/// @inhertiDoc.
///
RuntimeService::RuntimeService() :
    m_runtimeStatus(Status::Unknown) {
}

///
/// @inhertiDoc.
///
RuntimeService::~RuntimeService() {
    
}

///
/// @inhertiDoc.
///
RuntimeService::Status RuntimeService::getStatus() {
    return m_runtimeStatus;
}

///
/// @inhertiDoc.
///
void RuntimeService::setStatus(Status Status) {
#if defined(MSC_COMPILER)
    InterlockedExchange((LONG*)&m_runtimeStatus, Status);
#endif
}

///
/// @inhertiDoc.
///
void RuntimeService::setNextScene(std::string sceneName) {
    if (m_runtimeStatus == Status::NextScene) {
        return;
    }

    setStatus(Status::NextScene);
    m_sceneName = sceneName;
}

///
/// @inhertiDoc.
///
bool RuntimeService::isPaused() {
    return m_runtimeStatus == Status::Paused;
}

///
/// @inhertiDoc.
///
bool RuntimeService::isQuit() {
    return m_runtimeStatus == Status::Quit;
}

///
/// @inhertiDoc.
///
bool RuntimeService::isNextScene() {
    return m_runtimeStatus == Status::NextScene;
}

///
/// @inhertiDoc.
///
std::string RuntimeService::getSceneName() {
    return m_sceneName;
}

