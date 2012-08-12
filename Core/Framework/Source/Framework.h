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

#include "System.h"
#include "FrameworkAPI.h"

class Scheduler;
class ChangeManager;
class UScene;
class UObject;
class Gdf;

/**
 * Framework.
 * Responsible for tying in all the different managers and systems.  Also handles
 * loading and parsing of the global definition file (gdf).
 * 
 * @sa  IService::ISystemAccess
 * @sa  IService::ISystemAccess
 */
class Framework : public IService::ISystemAccess {
    public:

        /**
         * Default constructor.
         */
        Framework(void) throw(...);

        /**
         * Destructor.
         */
        ~Framework(void);

        /**
         * Initializes this :.
         *
         * @return  Error status.
         */
        Error Initialize(void);

        /**
         * Shuts down this : and frees any resources it is using.
         */
        void Shutdown(void);

        /**
         * Executes the framework..
         *
         * @return An error code.
         */
        Error Execute(void) throw(...);


    protected:

        /**
         * Gets a system.
         * Implementation of IService::ISystemAccess::GetSystem.
         *
         * @param   pszSystemName   Name of the system.
         * @return  The system.
         */
        virtual Handle GetSystem(const char* pszSystemName);

        /**
         * Implementation of IService::ISystemAccess::GetSystem.
         *
         * @param   Type    The type.
         * @return  The system.
         */
        virtual Handle GetSystem(SystemProto::Type Type);

        /**
         * Gets a scene.
         * Implementation of IService::ISystemAccess::GetScene.
         *
         * @param   pszSystemName   Name of the system.
         * @return  The scene.
         */
        virtual Handle GetScene(const char* pszSystemName);

        /**
         * Implementation of IService::ISystemAccess::GetScene.
         *
         * @param   Type    The type.
         * @return  The scene.
         */
        virtual Handle GetScene(SystemProto::Type Type);

        /**
         * Gets a system object.
         * Implementation of IService::ISystemAccess::GetSystemObject.
         *
         * @param   pszSystemName   Name of the system.
         * @param   pszName         The name.
         * @return  The system object.
         */
        virtual Handle GetSystemObject(const char* pszSystemName, const char* pszName);

        /**
         * Implementation of IService::ISystemAccess::GetSystemObject.
         *
         * @param   Type    The type.
         * @param   pszName The name.
         * @return  The system object.
         */
        virtual Handle GetSystemObject(SystemProto::Type Type, const char* pszName);

        /**
         * Gets a system property.
         * Implementation of IService::ISystemAccess::GetSystemProperty.
         *
         * @param   hSystem             Handle of the system.
         * @param [in,out]  Property    The property.
         */
        virtual void GetSystemProperty(Handle hSystem, InOut Properties::Property& Property);

        /**
         * Sets a system property.
         * Implementation of IService::ISystemAccess::SetSystemProperty.
         *
         * @param   hSystem             Handle of the system.
         * @param [in,out]  Property    The property.
         */
        virtual void SetSystemProperty(Handle hSystem, In Properties::Property& Property);

        /**
         * Gets a scene property.
         * Implementation of IService::ISystemAccess::GetSceneProperty.
         *
         * @param   hScene              Handle of the scene.
         * @param [in,out]  Property    The property.
         */
        virtual void GetSceneProperty(Handle hScene, InOut Properties::Property& Property);

        /**
         * Sets a scene property.
         * Implementation of IService::ISystemAccess::SetSceneProperty.
         *
         * @param   hScene              Handle of the scene.
         * @param [in,out]  Property    The property.
         */
        virtual void SetSceneProperty(Handle hScene, In Properties::Property& Property);

        /**
         * Gets an object property.
         * Implementation of IService::ISystemAccess::GetObjectProperty.
         *
         * @param   hObject             Handle of the object.
         * @param [in,out]  Property    The property.
         */
        virtual void GetObjectProperty(Handle hObject, InOut Properties::Property& Property);

        /**
         * Sets an object property.
         * Implementation of IService::ISystemAccess::SetObjectProperty.
         *
         * @param   hObject             Handle of the object.
         * @param [in,out]  Property    The property.
         */
        virtual void SetObjectProperty(Handle hObject, In Properties::Property& Property);

        /**
         * Issues all the pending property changes.  This only occurs after the scheduler has
         *  executed all the waiting items.
         *
         * @param   SystemTypes The system types of the system that can accept property changes.
         */
        void IssuePendingSystemPropertyChanges(System::Types::BitMask SystemTypes = System::Types::All);


    protected:

        struct PropertyIssue {
            System::Components      SystemComponent;
            SystemProto::Type            SystemType;
            Handle                  hItem;
            Properties::Property    Property;
        };
        typedef std::vector<PropertyIssue>      PropertyIssues;
        typedef PropertyIssues::iterator        PropertyIssuesIt;

        PropertyIssues                          m_PropertyIssues;
        DEFINE_SPIN_MUTEX(m_PropertyIssuesSpinWait);

    protected:

        Scheduler*                              m_pScheduler;
        ChangeManager*                          m_pSceneCCM;
        ChangeManager*                          m_pObjectCCM;
        UScene*                                 m_pScene;

        bool                                    m_bExecuteLoop;

#ifdef __ALLOW_DEBUG_WINDOW__
        Handle                                  m_hDebugWindow;
#endif

    private:

        std::string                             m_sNextScene;

};
