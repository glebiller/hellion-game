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

#include "FrameworkAPI.h"

/*******************************************************************************
* CLASS: Framework
*
* DESCRIPTION:
* Responsible for tying in all the different managers and systems.  Also handles
*  loading and parsing of the global definition file (gdf).
*******************************************************************************/

class Scheduler;
class ChangeManager;
class UScene;
class UObject;
class Gdf;

class Framework : public IService::ISystemAccess {
    public:

        //
        // Constructor.
        //
        Framework(void) throw(...);

        //
        // Destructor.
        //
        ~Framework(void);

        Error Initialize(void);

        void Shutdown(void);

        //
        // Executes the framework.
        //   returns - an error code.
        //
        Error Execute(void) throw(...);


    protected:

        ////////////////////////////////////////////////////////////////////////////////////////////////
        // IService::ISystemAccess Implementations.

        /// <summary cref="IService::ISystemAccess::GetSystem(const char*)">
        ///   Implementation of IService::ISystemAccess::GetSystem.
        /// </summary>
        virtual Handle GetSystem(const char* pszSystemName);

        /// <summary>
        ///   Implementation of IService::ISystemAccess::GetSystem.
        /// </summary>
        virtual Handle GetSystem(System::Type Type);

        /// <summary cref="IService::ISystemAccess::GetScene(const char*)">
        ///   Implementation of IService::ISystemAccess::GetScene.
        /// </summary>
        virtual Handle GetScene(const char* pszSystemName);

        /// <summary>
        ///   Implementation of IService::ISystemAccess::GetScene.
        /// </summary>
        virtual Handle GetScene(System::Type Type);

        /// <summary cref="IService::ISystemAccess::GetSystemObject(const cconst char* const char*)">
        ///   Implementation of IService::ISystemAccess::GetSystemObject.
        /// </summary>
        virtual Handle GetSystemObject(const char* pszSystemName, const char* pszName);

        /// <summary>
        ///   Implementation of IService::ISystemAccess::GetSystemObject.
        /// </summary>
        virtual Handle GetSystemObject(System::Type Type, const char* pszName);

        /// <summary cref="IService::ISystemAccess::GetSystemProperty">
        ///   Implementation of IService::ISystemAccess::GetSystemProperty.
        /// </summary>
        virtual void GetSystemProperty(Handle hSystem, InOut Properties::Property& Property);

        /// <summary cref="IService::ISystemAccess::SetSystemProperty">
        ///   Implementation of IService::ISystemAccess::SetSystemProperty.
        /// </summary>
        virtual void SetSystemProperty(Handle hSystem, In Properties::Property& Property);

        /// <summary cref="IService::ISystemAccess::GetSceneProperty">
        ///   Implementation of IService::ISystemAccess::GetSceneProperty.
        /// </summary>
        virtual void GetSceneProperty(Handle hScene, InOut Properties::Property& Property);

        /// <summary cref="IService::ISystemAccess::SetSceneProperty">
        ///   Implementation of IService::ISystemAccess::SetSceneProperty.
        /// </summary>
        virtual void SetSceneProperty(Handle hScene, In Properties::Property& Property);

        /// <summary cref="IService::ISystemAccess::GetObjectProperty">
        ///   Implementation of IService::ISystemAccess::GetObjectProperty.
        /// </summary>
        virtual void GetObjectProperty(Handle hObject, InOut Properties::Property& Property);

        /// <summary cref="IService::ISystemAccess::SetObjectProperty">
        ///   Implementation of IService::ISystemAccess::SetObjectProperty.
        /// </summary>
        virtual void SetObjectProperty(Handle hObject, In Properties::Property& Property);

        /// <summary>
        ///   Issues all the pending property changes.  This only occurs after the scheduler has
        ///    executed all the waiting items.
        /// </summary>
        /// <param name="SystemTypes">The system types of the system that can accept property
        ///  changes.</param>
        void IssuePendingSystemPropertyChanges(System::Types::BitMask SystemTypes = System::Types::All);


    protected:

        struct PropertyIssue {
            System::Components      SystemComponent;
            System::Type            SystemType;
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

        class GDFParser {
            public:

                GDFParser(UScene* pScene, std::string sGDF);

                void ParseEnvironment(void);

                void ParseSystems(void);

                void ParseScene(std::string sScene);

                std::string StartupScene(void);

                GDFParser& operator=(const GDFParser&) { ASSERT(false); }

            protected:

                void ApplyProperties(
                    ProtoPropertyList& PropertyList,
                    Properties::Array& GetProperties,
                    Properties::Array& SetProperties
                ) const;


            protected:

                Proto::Gdf                                      m_Gdf;

                UScene*                                         m_pScene;
                ISystem*                                        m_pSystem;
                ISystemScene*                                   m_pSystemScene;

                UObject*                                        m_pUObject;
                ISystemObject*                                  m_pSystemObject;

                Properties::Array                               m_GetProperties;
                Properties::Array                               m_SetProperties;

                std::map<u32, Properties::Array>                 m_SetPropertiesMap;
                std::map<UObject*, std::map<u32, Properties::Array>>  m_AllObjectProperties;
        };
};
