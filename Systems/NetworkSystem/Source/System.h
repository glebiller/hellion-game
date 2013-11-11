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

#include "Errors.h"
#include "Proto.h"
#include "System.h"
#include "System/ISystem.h"
#include "Service/NetworkService.h"

class NetworkScene;
class NetworkTask;

/**
 * Implementation of the ISystem interface for graphics. See Interfaces\System.h for a
 * definition of the class and its functions.
 * 
 * @sa  ISystem
 */
class NetworkSystem : public ISystem {

public:
    
    /**
     * @inheritDoc
     */
    NetworkSystem(void);
        
    /**
     * @inheritDoc
     */
    ~NetworkSystem(void);
        
    /**
     * @inheritDoc
     */
    Error initialize(void);

    /**
     * @inheritDoc
     */
    inline Proto::SystemType GetSystemType(void) {
        return Proto::SystemType::Network;
    }

    /**
     * @inheritDoc
     */
    inline NetworkService* getNetworkService(void) {
        return m_networkService;
    }

private:

    NetworkService*     m_networkService;

};

