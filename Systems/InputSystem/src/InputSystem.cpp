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

#include <boost/dll.hpp>

#include "System/ISystem.h"
#include "Manager/ServiceManager.h"
#include "Generic/SystemInterface.h"
#include "System.h"

IServiceManager*        g_serviceManager;

extern "C" void BOOST_SYMBOL_EXPORT InitializeSystemLib(IServiceManager* serviceManager) {
    g_serviceManager = serviceManager;
}

extern "C" ISystem* BOOST_SYMBOL_EXPORT CreateSystem() {
    return new InputSystem();
}

extern "C" void BOOST_SYMBOL_EXPORT DestroySystem(ISystem* pSystem) {
    delete reinterpret_cast<InputSystem*>(pSystem);
}
