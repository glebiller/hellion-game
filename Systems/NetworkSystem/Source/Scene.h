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

#define DIRECTNetwork_VERSION 0x0800
#include "dNetwork.h"
#include "System.h"

class NetworkSystem;
class NetworkTask;
class NetworkObject;

///////////////////////////////////////////////////////////////////////////////
/// <summary>
///   <c>NetworkScene</c> Implementation of the ISystemScene interface.
///   The Network scene contains all the Network objects
/// </summary>
///////////////////////////////////////////////////////////////////////////////

class NetworkScene : public ISystemScene {
        friend class NetworkSystem;
        friend class NetworkTask;

    public:
        static BOOL __stdcall EnumSuitableDevicesCB(LPCDIDEVICEINSTANCE lpddi,
                LPDIRECTNetworkDEVICE8 lpdid,
                DWORD dwFlags,
                DWORD dwRemaining,
                LPVOID pvRef);
    protected:

        NetworkScene(ISystem* pSystem);
        ~NetworkScene(void);

        /// <summary cref="NetworkScene::GetSystemType">
        ///   Implementation of the <c>ISystemScene::GetSystemType</c> function.
        /// </summary>
        /// <returns>SystemProto::Type - Type of this system.</returns>
        /// <seealso cref="ISystemScene::GetSystemType"/>
        virtual SystemProto::Type GetSystemType(void);

        /// <summary cref="NetworkScene::NetworkUpdate">
        /// This function must be called every frame.  It updates the Network scene.
        /// </summary>
        /// <param name="DeltaTime">Elapsed time since the last frame.</param>
        /// <seealso cref="ISystemTask::Update"/>
        void NetworkUpdate(f32 DeltaTime);

        /// <summary cref="NetworkScene::Initialize">
        ///   Implementation of the <c>ISystemScene::Initialize</c> function.
        ///   One time initialization function for the scene.
        /// </summary>
        /// <param name="Properties">Initializes the Network scene with the properties specified by <paramref name="Properties"/>.</param>
        /// <returns>Error.</returns>
        /// <seealso cref="ISystemScene::Initialize"/>
        virtual Error Initialize(std::vector<Properties::Property> Properties);

        /// <summary cref="NetworkScene::GetProperties">
        ///   Implementation of the <c>ISystemScene::GetProperties</c> function.
        ///   Gets the properties of this scene.
        /// </summary>
        /// <param name="Properties">Gets the properties of the sound scene</param>
        /// <seealso cref="ISystemScene::GetProperties"/>
        virtual void GetProperties(Properties::Array& Properties);

        /// <summary cref="NetworkScene::SetProperties">
        ///   Implementation of the <c>ISystemScene::SetProperties</c> function.
        ///   Sets the properties for this scene.
        /// </summary>
        /// <param name="Properties">Sets the properties of the Network scene</param>
        /// <seealso cref="ISystem::SetProperties"/>
        virtual void SetProperties(Properties::Array Properties);

        /// <summary cref="NetworkScene::GetObjectTypes">
        ///   Implementation of the <c>ISystemScene::GetObjectTypes</c> function.
        ///   Get all the available object types as names.
        /// </summary>
        /// <returns>const char** - A NULL terminated array of object type names.</returns>
        /// <seealso cref="ISystemScene::GetObjectTypes"/>
        virtual const char** GetObjectTypes(void);

        /// <summary cref="NetworkScene::CreateObject">
        ///   Implementation of the <c>ISystemScene::CreateObject</c> function.
        ///   Creates a system object used to extend a UObject.
        /// </summary>
        /// <param name="pszName">The unique name for this object.</param>
        /// <param name="pszType">The object type to create.</param>
        /// <returns>ISystemObject* - The newly created system object.</returns>
        /// <seealso cref="ISystemScene::CreateObject"/>
        virtual ISystemObject* CreateObject(const char* pszName, const char* pszType);

        /// <summary cref="NetworkScene::DestroyObject">
        ///   Implementation of the <c>ISystemScene::DestroyObject</c> function.
        ///   Destroys a system object.
        /// </summary>
        /// <param name="pSystemObject">The system object to destroy.</param>
        /// <returns>Error - Any error codes.</returns>
        /// <seealso cref="ISystemScene::DestroyObject"/>
        virtual Error DestroyObject(ISystemObject* pSystemObject);

        /// <summary cref="NetworkScene::GetSystemTask">
        ///   Implementation of the <c>ISystemScene::GetSystemTask</c> function.
        ///   Returns a pointer to the task that this scene needs to perform on its objects.
        /// </summary>
        /// <returns>ISystemTask* - The task for this scene.</returns>
        /// <seealso cref="ISystemScene::GetSystemTask"/>
        virtual ISystemTask* GetSystemTask(void);

        /// <summary cref="NetworkScene::GetPotentialSystemChanges">
        ///   Implementation of the <c>ISubject::GetPotentialSystemChanges</c> function.
        ///   Identifies the system changes that this subject could possibly make.
        /// </summary>
        /// <returns>System::Changes::BitMask - A bitmask of the possible system changes.</returns>
        /// <seealso cref="ISubject::GetPotentialSystemChanges"/>
        virtual System::Changes::BitMask GetPotentialSystemChanges(void);

        /// <summary cref="NetworkScene::GetSystemChangeData">
        ///   Implementation of the <c>ISubject::GetPotentialSystemChanges</c> function.
        ///   Identifies the system changes that this subject could possibly make.
        /// </summary>
        /// <param name="SystemChange">A bitmask of the changes in the Network scene.</param>
        virtual const void* GetSystemChangeData(System::Change SystemChange);

    protected:

        // These three variables are used for determining what hotkey combination was used
        NetworkSystem*                    m_pNetworkSystem;
        std::map<std::string, unsigned long>          m_aKeyboard;
        NetworkSystem::hotKeyIt           m_pHotKeyIter;

        // Main task for the Network scene
        NetworkTask*                      m_pNetworkTask;

        // Network scene objects
        std::list<NetworkObject*>         m_Objects;

        // Enable catching mouse movements
        bool                            m_MouseMoveEnabled;
        bool                            m_bControlKeyPressed;

        LPDIRECTNetwork8                  m_pDI;          // The DirectNetwork object
        DIACTIONFORMAT                  m_diaf;

        f32                             m_nRotateLeftRight;
        f32                             m_nRotateUpDown;
        f32                             m_nMoveLeftRight;
        f32                             m_nMoveForwardBack;
        f32                             m_nMoveUpDown;

        //
        // For the keyboard, we need to save the amount moved on a keydown so that it can be reversed on a keyup.  We
        // cannot recalculate because DeltaTime is slightly different every update so you will end up with drifting.
        //
        f32                             m_nOldForward;
        f32                             m_nOldBack;
        f32                             m_nOldLeft;
        f32                             m_nOldRight;
        f32                             m_nOldUp;
        f32                             m_nOldDown;

        //
        // All of the devices attached to the PC
        //
#define MAX_Network_DEVICES 5
        u32                             m_nDevices;
        LPDIRECTNetworkDEVICE8            m_pNetworkDevices[MAX_Network_DEVICES];
};
