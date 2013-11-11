// Copyright © 2008-2009 Intel Corporation
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

#include "Errors.h"
#include "System/ISystem.h"
#include "System/Types.h"

class ISystem;

/**
 * Provides a single location for access to the different systems that are available for use.
 * @sa  Singleton
 */
class SystemService {
public:
    /**
     * Constructor.
     */
    SystemService(void);

    /**
     * Destructor.
     */
    ~SystemService(void);

    /**
     * @inheritDoc
     */
    Error add(ISystem* pSystem);
    
    /**
     * @inheritDoc
     */
    Error remove(const Proto::SystemType SystemType);
    
    /**
     * @inheritDoc
     */
    ISystem* get(const Proto::SystemType SystemType);
    
    /**
     * @inheritDoc
     */
    std::map<Proto::SystemType, ISystem*> get(void);

protected:
    std::map<Proto::SystemType, ISystem*>       m_systems;

};
