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

#include "Proto/Common.pb.h"
#include "Defines.h"

class IServiceManager;
class ISystem;

/**
 * Function type definitions for communicating with the system DLL.
 */
extern "C" {

#if defined( MSC_COMPILER )
    /**
     * Function definition for calling into a system library for creating a system.
     * 
     * @return   The newly created system.
     */
    typedef ISystem* (__stdcall* CreateSystemFunction)();

    /**
     * Function definition for calling into a system library for initializing a system.
     */
    typedef void (__stdcall* InitializeSystemLibFunction)(IServiceManager* serviceManager);

    /**
     * Function definition for calling into a system library for destroying a system.
     * 
     * @param    pSystem The system to destroy.
     */
    typedef void (__stdcall* DestroySystemFunction)(ISystem* pSystem);
#elif defined( GCC_COMPILER )
    /**
     * Function definition for calling into a system library for creating a system.
     * 
     * @return   The newly created system.
     */
    ISystem* __attribute__((__stdcall)) CreateSystemFunction(Log::Logger* p_logger);

    /**
     * Function definition for calling into a system library for initializing a system.
     */
    void __attribute__((__stdcall)) InitializeSystemLibFunction(IServiceManager* serviceManager);

    /**
     * Function definition for calling into a system library for destroying a system.
     * 
     * @param    pSystem The system to destroy.
     */
    void __attribute__((__stdcall)) DestroySystemFunction(ISystem* pSystem);    
#endif

}
