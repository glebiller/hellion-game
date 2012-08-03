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


#include <windows.h>

//
// extern includes
//
#pragma warning( push, 0 )
#pragma warning( disable : 6031 6201 6323 6386 )
#include "Common\Base\hkBase.h"
#include "Physics\Dynamics\World\hkpWorld.h"
#pragma warning( pop )

//
// core includes
//
#include <BaseTypes.h>
#include <Interface.h>
#include <Manager/ServiceManager.h>

//
// system includes
//
#include "Collision.h"
#include "System.h"

//
// havok includes
//
#include "Common\Base\Memory\Allocator\Malloc\hkMallocAllocator.h"

//
// global variables
//
ManagerInterfaces   g_Managers;
hkMallocAllocator   g_MallocAllocator;


///////////////////////////////////////////////////////////////////////////////
// DllMain - API entry point for SystemPhysicsCollisionHavok DLL
BOOL APIENTRY
DllMain(
    HMODULE hModule,
    DWORD Reason,
    LPVOID pReserved
) {
    UNREFERENCED_PARAM(hModule);
    UNREFERENCED_PARAM(pReserved);

    switch (Reason) {
        case DLL_PROCESS_ATTACH:
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}


///////////////////////////////////////////////////////////////////////////////
// InitializeNewtonPhysicsCollision - Initialize the PhysicsCollisionHavok system
extern "C" void __stdcall
InitializePhysicSystem(
    ManagerInterfaces* pManagers
) {
    g_Managers = *pManagers;
}


///////////////////////////////////////////////////////////////////////////////
// CreateAISystem - Create the PhysicsCollisionHavok system
extern "C" ISystem* __stdcall
CreatePhysicSystem(
    Debug::Debugger* p_Debugger
) {
    // Setup debugger
    Debug::Init(p_Debugger);
    // Create Havok system
    HavokPhysicsSystem* System = new HavokPhysicsSystem();
    // Return reference to new system
    return System;
}


///////////////////////////////////////////////////////////////////////////////
// DestroyHavokPhysicsCollisionSystem - Free the given PhysicsCollisionHavok system
extern "C" void __stdcall
DestroyPhysicSystem(
    ISystem* pSystem
) {
    HavokPhysicsSystem* pHavokSystem = reinterpret_cast<HavokPhysicsSystem*>(pSystem);
    // Delete scene
    delete pHavokSystem;
}
