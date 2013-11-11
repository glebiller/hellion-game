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

#include "Defines.h"
#include <windows.h>

#include "System/ISystem.h"
#include "Manager/ServiceManager.h"
#include "Generic/SystemInterface.h"
#include "System.h"

IServiceManager*        g_serviceManager;
HINSTANCE               g_hInstance;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD Reason, LPVOID pReserved) {
    switch (Reason) {
        case DLL_PROCESS_ATTACH:
            g_hInstance = hModule;
            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}

extern "C" void __stdcall InitializeNetworkSystem(IServiceManager* serviceManager) {
    g_serviceManager = serviceManager;
    g_serviceManager->getLogService()->initSystem(Proto::SystemType::Network);
}

extern "C" ISystem* __stdcall CreateNetworkSystem() {
    return new NetworkSystem();
}

extern "C" void __stdcall DestroyNetworkSystem(ISystem* pSystem) {
    delete reinterpret_cast<NetworkSystem*>(pSystem);
    g_serviceManager->getLogService()->closeSystem(Proto::SystemType::Network);
}
