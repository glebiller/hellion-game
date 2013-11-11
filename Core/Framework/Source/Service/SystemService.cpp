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

#include "SystemService.h"

/**
 * @inheritDoc
 */
SystemService::SystemService(void) {
}

/**
 * @inheritDoc
 */
SystemService::~SystemService(void) {
}

/**
 * @inheritDoc
 */
Error SystemService::add(ISystem* pSystem) {
    Proto::SystemType systemType = pSystem->GetSystemType();
    ASSERT(m_systems.find(systemType) == m_systems.end());
    m_systems[systemType] = pSystem;
    return Errors::Success;
}

/**
 * @inheritDoc
 */
Error SystemService::remove(const Proto::SystemType SystemType) {
    Error Err = Errors::Success;
    auto it = m_systems.find(SystemType);
    if (it != m_systems.end()) {
        m_systems.erase(it);
    }
    return Err;
}

/**
 * @inheritDoc
 */
ISystem* SystemService::get(const Proto::SystemType SystemType) {
    ISystem* pSystem = nullptr;
    auto it = m_systems.find(SystemType);
    if (it != m_systems.end()) {
        pSystem = (*it).second;
    }
    return pSystem;
}

/**
 * @inheritDoc
 */
std::map<Proto::SystemType, ISystem*> SystemService::get(void) {
    return m_systems;
}

