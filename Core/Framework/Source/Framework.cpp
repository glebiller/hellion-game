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

#include "Defines.h"
#include "BaseTypes.h"
#include "Interface.h"

#include <Proto.h>

#include "Universal.h"
#include "DebugWindow.h"
#include "Manager/MemoryManager.h"
#include "Manager/SystemManager.h"
#include "Manager/PlatformManager.h"
#include "Manager/ChangeControlManager.h"
#include "Manager/EnvironmentManager.h"
#include "Manager/ServiceManager.h"
#include "Manager/TaskManager.h"
#include "Scheduler.h"
#include "Instrumentation.h"
#include "Framework.h"

CreateSingleton(MemoryManager);
CreateSingleton(EnvironmentManager);
CreateSingleton(PlatformManager);
CreateSingleton(SystemManager);
CreateSingleton(ServiceManager);
CreateSingleton(Instrumentation);

TaskManager*    g_pTaskManager = NULL;

void
ExecuteFramework(void) {
#ifndef DEBUG_BUILD
    try
#endif
    {
        Framework   Framework;

        if (Framework.Initialize() == Errors::Success) {
            Framework.Execute();
            Framework.Shutdown();
        }
    }

#ifndef DEBUG_BUILD
    catch (...) {
        // Display an error message.
    }
#endif
}


Framework::Framework(void) : m_bExecuteLoop(true), m_pScheduler(NULL), m_pSceneCCM(NULL), m_pObjectCCM(NULL)
#ifdef __ALLOW_DEBUG_WINDOW__
    , m_hDebugWindow(NULL)
#endif
{
    //
    // g_pTaskManager and m_pScheduler are instantiated after the environment variables
    // in the config file are parsed
    //
    m_pSceneCCM = new ChangeManager();
    ASSERT(m_pSceneCCM != NULL);
    m_pObjectCCM = new ChangeManager();
    ASSERT(m_pObjectCCM != NULL);
}


Framework::~Framework(void) {
    SAFE_DELETE(m_pScheduler);
    SAFE_DELETE(g_pTaskManager);
    SAFE_DELETE(m_pSceneCCM);
    SAFE_DELETE(m_pObjectCCM);
}


Error Framework::Initialize(void) {
    //
    // Set the current directory to the location of the GDF.
    //
    static const char* apszFile = "Application.gdf.bin";
    static const char* apszLocations[] = { ".\\." };

    if (!Singletons::PlatformManager.FileSystem().SetCurrentDirToFileLocation(apszFile, apszLocations)) {
        ASSERT(false, "Framework could not locate the GDF file Application.gdf.");
        return Errors::File::NotFound;
    }

    //
    // Create the initial universal scene.
    //
    m_pScene = new UScene(m_pSceneCCM, m_pObjectCCM);
    ASSERT(m_pScene != NULL);

    if (m_pScene == NULL) {
        return Errors::Memory::OutOfMemory;
    }

    //
    // Instantiate the parser, parse the environment variables in the GDF.
    //
    GDFParser Parser(m_pScene, apszFile);
    Parser.ParseEnvironment();
    //
    // Register the framework as the system access provider.  The system access provider gives the
    //  ability for systems to set the properties in other systems.
    //
    Singletons::ServiceManager.RegisterSystemAccessProvider(this);
    //
    // Instantiate the task manager.
    //
    g_pTaskManager = new TaskManager();
    g_pTaskManager->Init();
    //
    // Instantiate the scheduler.
    //
    m_pScheduler = new Scheduler(g_pTaskManager);
    ASSERT(m_pScheduler != NULL);

    if (m_pScheduler == NULL) {
        return Errors::Memory::OutOfMemory;
    }

    //
    // Complete the parsing of the GDF and the initial scene.
    //
    Parser.ParseSystems();
    m_sNextScene = Parser.StartupScene();
    Parser.ParseScene(m_sNextScene);
    //
    // Set the initial scene for the scheduler.
    //
    m_pScheduler->SetScene(m_pScene);
    return Errors::Success;
}


void
Framework::Shutdown(
    void
) {
    //
    // Get rid of the scene.
    //
    SAFE_DELETE(m_pScene);
#ifdef __ALLOW_DEBUG_WINDOW__

    //
    // Destroy the debug window.
    //
    if (m_hDebugWindow != NULL) {
        DestroyDebugWindow(m_hDebugWindow);
    }

#endif
    //
    // De-register the framework as the system access provider.
    //
    Singletons::ServiceManager.UnregisterSystemAccessProvider(this);
    //
    // Free resources used for parallel execution by the change manager.
    //
    m_pObjectCCM->ResetTaskManager();
    m_pSceneCCM->ResetTaskManager();

    //
    // Free the task manager.
    //
    if (g_pTaskManager != NULL) {
        g_pTaskManager->Shutdown();
        delete g_pTaskManager;
        g_pTaskManager = NULL;
    }
}


Error
Framework::Execute(
    void
) {
    //
    // Create the debug window.
    //
#ifdef __ALLOW_DEBUG_WINDOW__
    if (Singletons::EnvironmentManager.Variables().GetAsBool("Framework::DebugWindow", false)) {
        m_hDebugWindow = CreateDebugWindow();
    }

#endif
    //
    // Process the link messages in the CCMs first, for both the object and scene CCMs.  The link
    //  needs to be established before any other messages come through.
    //
    m_pObjectCCM->DistributeQueuedChanges(
        System::Types::All, System::Changes::Link | System::Changes::ParentLink
    );
    m_pSceneCCM->DistributeQueuedChanges(
        System::Types::All, System::Changes::Link | System::Changes::ParentLink
    );
    //
    // Distribute changes for object and scene CCMs.  The UObject propagates some object messages
    //  up to the scene so it needs to go first.
    //
    m_pObjectCCM->DistributeQueuedChanges();
    m_pSceneCCM->DistributeQueuedChanges();
#ifdef __ALLOW_DEBUG_WINDOW__

    if (m_hDebugWindow != NULL) {
        //
        // Initialize the debug window.
        //
        SetDebugWindowCCM(m_hDebugWindow, m_pSceneCCM, m_pObjectCCM);
        SetDebugWindowUScene(m_hDebugWindow, m_pScene);
    }

#endif
    //
    // Set the runtime status to running.
    //
    Singletons::EnvironmentManager.Runtime().SetStatus(IEnvironment::IRuntime::Status::Running);
    //
    // Initialize resources necessary for parallel change distribution.
    //
    m_pObjectCCM->SetTaskManager(g_pTaskManager);
    m_pSceneCCM->SetTaskManager(g_pTaskManager);

    while (m_bExecuteLoop) {
        //
        // Process any pending window messages.
        //
        Singletons::PlatformManager.WindowSystem().ProcessMessages();
        //
        // Call the scheduler to have the systems internally update themselves.
        //
        m_pScheduler->Execute();
        //
        // Set any properties that may have been issued for change.  Any propeties that correlate
        //  to system change notifications will be added to the change controller by the system.
        // NOTE: This is still untested as noone is using it.
        //
        IssuePendingSystemPropertyChanges();
        //
        // Distribute changes for object and scene CCMs.  The UObject propagates some object
        //  messages up to the scene CCM so it needs to go first.
        //
        m_pObjectCCM->DistributeQueuedChanges();
        m_pSceneCCM->DistributeQueuedChanges();

        //
        // Check with the environment manager if there is a change in the runtime status to quit.
        //
        if (Singletons::EnvironmentManager.Runtime().GetStatus() ==
                IEnvironment::IRuntime::Status::Quit) {
            //
            // Time to quit looping.
            //
            m_bExecuteLoop = false;
        }
    }

    return Errors::Success;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// IService::ISystemAccess Implementations.

Handle
Framework::GetSystem(
    const char* pszSystemName
) {
    //
    // Get the pointer to the system from the system manager.  Handle is just a void* so it will
    //  convert with any problems.  If you're one of those guys that will think of just casting
    //  the handle back you'd better not as you'll break the threading when you try to make calls
    //  into this interface directly.
    //
    return Singletons::SystemManager.Get(pszSystemName);
}


Handle
Framework::GetSystem(
    System::Type Type
) {
    //
    // Get the pointer to the system from the system manager.  Handle is just a void* so it will
    //  convert with any problems.  If you're one of those guys that will think of just casting
    //  the handle back you'd better not as you'll break the threading when you try to make calls
    //  into this interface directly.
    //
    return Singletons::SystemManager.Get(Type);
}


Handle
Framework::GetScene(
    const char* pszSystemName
) {
    Handle hScene = NULL;
    //
    // Get the system from the system manager to get the type.
    //
    ISystem* pSystem = Singletons::SystemManager.Get(pszSystemName);

    if (pSystem != NULL) {
        //
        // Get the scene based on the type.
        //
        hScene = GetScene(pSystem->GetSystemType());
    }

    return hScene;
}


Handle
Framework::GetScene(
    System::Type Type
) {
    Handle hScene = NULL;
    //
    // Find the scene extension in the universal scene.
    //
    UScene::SystemScenes::const_iterator it = m_pScene->GetSystemScenes().find(Type);

    if (it != m_pScene->GetSystemScenes().end()) {
        hScene = it->second;
    }

    return hScene;
}


Handle
Framework::GetSystemObject(
    const char* pszSystemName,
    const char* pszName
) {
    Handle hObject = NULL;
    //
    // Get the system from the system manager to get the type.
    //
    ISystem* pSystem = Singletons::SystemManager.Get(pszSystemName);

    if (pSystem != NULL) {
        hObject = GetSystemObject(pSystem->GetSystemType(), pszName);
    }

    return hObject;
}


Handle
Framework::GetSystemObject(
    System::Type Type,
    const char* pszName
) {
    Handle hObject = NULL;
    //
    // Find the universal object in the scene.
    //
    UObject* pUObject = m_pScene->FindObject(pszName);

    if (pUObject != NULL) {
        //
        // Get the system object extension of the universal object using the system type.
        //
        ISystemObject* pObject = pUObject->GetExtension(Type);

        if (pObject != NULL) {
            hObject = pObject;
        }
    }

    return hObject;
}


void
Framework::GetSystemProperty(
    Handle hSystem,
    InOut Properties::Property& Property
) {
    std::string sPropertyName = Property.GetName();
    //
    // Reinterpret the handle as an ISystem.
    //
    ASSERT(hSystem != NULL);
    ISystem* pSystem = reinterpret_cast<ISystem*>(hSystem);
    //
    // Get the properties and find a match.
    //
    Properties::Array aProperties;
    pSystem->GetProperties(aProperties);
    bool bFound = false;

    for (Properties::ConstIterator it = aProperties.begin(); it != aProperties.end(); it++) {
        if (sPropertyName == it->GetName()) {
            Property = *it;
            bFound = true;
        }
    }

    ASSERTMSG(bFound, "The property does not exist.");
}


void
Framework::SetSystemProperty(
    Handle hSystem,
    In Properties::Property& Property
) {
    ASSERT(hSystem != NULL);
    ISystem* pSystem = reinterpret_cast<ISystem*>(hSystem);
#ifdef DEBUG_BUILD
    //
    // This will cause an assertion if the property doesn't exist.
    //
    Properties::Property TempProperty = Property;
    GetSystemProperty(hSystem, TempProperty);
#endif
    PropertyIssue pi = { System::System, pSystem->GetSystemType(), hSystem, Property };
    SCOPED_SPIN_LOCK(m_PropertyIssuesSpinWait);
    m_PropertyIssues.push_back(pi);
}


void
Framework::GetSceneProperty(
    Handle hScene,
    InOut Properties::Property& Property
) {
    std::string sPropertyName = Property.GetName();
    //
    // Reinterpret the handle as an ISystemScene.
    //
    ASSERT(hScene != NULL);
    ISystemScene* pSystemScene = reinterpret_cast<ISystemScene*>(hScene);
    //
    // Get the properties and find a match.
    //
    Properties::Array aProperties;
    pSystemScene->GetProperties(aProperties);
    bool bFound = false;

    for (Properties::ConstIterator it = aProperties.begin(); it != aProperties.end(); it++) {
        if (sPropertyName == it->GetName()) {
            Property = *it;
            bFound = true;
        }
    }

    ASSERTMSG(bFound, "The property does not exist.");
}


void
Framework::SetSceneProperty(
    Handle hScene,
    In Properties::Property& Property
) {
    ASSERT(hScene != NULL);
    ISystemScene* pSystemScene = reinterpret_cast<ISystemScene*>(hScene);
#ifdef DEBUG_BUILD
    //
    // This will cause an assertion if the property doesn't exist.
    //
    Properties::Property TempProperty = Property;
    GetSceneProperty(hScene, TempProperty);
#endif
    PropertyIssue pi = { System::Scene, pSystemScene->GetSystemType(),
                         hScene, Property
                       };
    SCOPED_SPIN_LOCK(m_PropertyIssuesSpinWait);
    m_PropertyIssues.push_back(pi);
}


void
Framework::GetObjectProperty(
    Handle hObject,
    InOut Properties::Property& Property
) {
    std::string sPropertyName = Property.GetName();
    //
    // Reinterpret the handle as an ISystemScene.
    //
    ASSERT(hObject != NULL);
    ISystemObject* pSystemObject = reinterpret_cast<ISystemObject*>(hObject);
    //
    // Get the properties and find a match.
    //
    Properties::Array aProperties;
    pSystemObject->GetProperties(aProperties);
    bool bFound = false;

    for (Properties::ConstIterator it = aProperties.begin(); it != aProperties.end(); it++) {
        if (sPropertyName == it->GetName()) {
            Property = *it;
            bFound = true;
        }
    }

    ASSERTMSG(bFound, "The property does not exist.");
}


void
Framework::SetObjectProperty(
    Handle hObject,
    In Properties::Property& Property
) {
    ASSERT(hObject != NULL);
    ISystemObject* pSystemObject = reinterpret_cast<ISystemObject*>(hObject);
#ifdef DEBUG_BUILD
    //
    // This will cause an assertion if the property doesn't exist.
    //
    Properties::Property TempProperty = Property;
    GetObjectProperty(hObject, TempProperty);
#endif
    PropertyIssue pi = { System::Object, pSystemObject->GetSystemType(),
                         hObject, Property
                       };
    SCOPED_SPIN_LOCK(m_PropertyIssuesSpinWait);
    m_PropertyIssues.push_back(pi);
}


void
Framework::IssuePendingSystemPropertyChanges(
    System::Types::BitMask SystemTypes
) {
    //
    // Create an array used for setting the property.
    //
    Properties::Array aProperties(1);

    //
    // Iterate through all the properties to issue.
    //
    for (PropertyIssuesIt it = m_PropertyIssues.begin(); it != m_PropertyIssues.end(); it++) {
        //
        // Get the property to issue.
        //
        const PropertyIssue& pi = *it;

        //
        // Check if the schuler will allow issuing the property change.
        //
        if (pi.SystemType & SystemTypes) {
            aProperties[ 0 ] = pi.Property;

            switch (pi.SystemComponent) {
                case System::System:
                    reinterpret_cast<ISystem*>(pi.hItem)->SetProperties(aProperties);
                    break;

                case System::Scene:
                    reinterpret_cast<ISystemScene*>(pi.hItem)->SetProperties(aProperties);
                    break;

                case System::Object:
                    reinterpret_cast<ISystemObject*>(pi.hItem)->SetProperties(aProperties);
                    break;

                default:
                    ASSERTMSG(false, "Unhandled case.");
                    break;
            };
        }
    }

    m_PropertyIssues.clear();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
// Framework::GDFParser Implementations.

Framework::GDFParser::GDFParser(
    UScene* pScene,
    std::string sGDF
)
    : m_pScene(pScene) {
    //
    // Read the protobuf file
    //
    Error result = Singletons::PlatformManager.FileSystem().LoadProto(sGDF.c_str(), &m_Gdf);
    ASSERT(result == Errors::Success);
}


void
Framework::GDFParser::ParseEnvironment(
    void
) {
    ASSERT(m_Gdf.IsInitialized());
    //
    // Fill the property manager
    //
    const ProtoPropertyList& properties = m_Gdf.properties();

    for (ProtoPropertyList::const_iterator propertiesIt = properties.begin();
            propertiesIt != properties.end();
            propertiesIt++) {
        // Environment properties only have one value
        Singletons::EnvironmentManager.Variables().Add(propertiesIt->name().c_str(),
                propertiesIt->value().Get(0).c_str());
    }
}


void
Framework::GDFParser::ParseSystems(
    void
) {
    ASSERT(m_Gdf.systems_size() > 0);
    //
    // Load systems
    //
    const ProtoSystemList& systems = m_Gdf.systems();

    for (ProtoSystemList::const_iterator systemsIt = systems.begin();
            systemsIt != systems.end();
            systemsIt++) {
        //
        // Load the system library.
        //
        Singletons::PlatformManager.FileSystem().LoadSystemLibrary(
            systemsIt->lib().c_str(), &m_pSystem
        );
        // Get the default properties from system, then Initialize it
        m_pSystem->GetProperties(m_GetProperties);
        ApplyProperties(static_cast<ProtoPropertyList>(systemsIt->properties()), m_GetProperties, m_SetProperties);
        m_pSystem->Initialize(m_SetProperties);
        m_GetProperties.clear();
        m_SetProperties.clear();
        ASSERTMSG1(strcmp(systemsIt->type().c_str(), m_pSystem->GetName()) == 0,
                   "Parser identified an incorrect system type. It should be %s.", m_pSystem->GetName());
    }
}

void
Framework::GDFParser::ParseScene(
    std::string sScene
) {
    //
    // Parse the selected scene
    //
    const ProtoSceneList& scenes = m_Gdf.scenes();

    for (ProtoSceneList::const_iterator scenesIt = scenes.begin();
            scenesIt != scenes.end();
            scenesIt++) {
        if (sScene.compare(scenesIt->name()) == 0) {
            //
            // Create the initial scene for each system.
            //
            ISystem* pSystem = Singletons::SystemManager.GetFirst();

            while (pSystem != NULL) {
                m_pScene->Extend(pSystem);
                pSystem = Singletons::SystemManager.GetNext();
            }

            //
            // Parse the SDF file
            //
            Proto::Sdf sdf;
            std::string sceneProtoFile = scenesIt->sdf() + ".sdf.bin";
            Error result = Singletons::PlatformManager.FileSystem().LoadProto(sceneProtoFile.c_str(), &sdf);
            ASSERT(result == Errors::Success);

            //
            // Initialize the System scenes.
            //
            for (ProtoSystemPropertiesList::const_iterator systemPropertiesIt = sdf.systemproperties().begin();
                    systemPropertiesIt != sdf.systemproperties().end();
                    systemPropertiesIt++) {
                m_pSystem = Singletons::SystemManager.Get(systemPropertiesIt->systemtype().c_str());
                ASSERTMSG1(m_pSystem != NULL,
                           "Parser was unable to get system %s.", systemPropertiesIt->systemtype().c_str());

                if (m_pSystem != NULL) {
                    UScene::SystemScenesConstIt it = m_pScene->GetSystemScenes().find(m_pSystem->GetSystemType());
                    ASSERTMSG1(it != m_pScene->GetSystemScenes().end(),
                               "Parser was unable to find a scene for system %s.", systemPropertiesIt->systemtype().c_str());
                    m_pSystemScene = it->second;
                    ASSERT(m_pSystemScene != NULL);
                    // Initialize system scene properties
                    m_pSystemScene->GetProperties(m_GetProperties);
                    ApplyProperties(static_cast<ProtoPropertyList>(systemPropertiesIt->properties()), m_GetProperties, m_SetProperties);
                    m_pSystemScene->Initialize(m_SetProperties);
                    m_GetProperties.clear();
                    m_SetProperties.clear();
                }
            }

            //
            // Initialize the scene objects.
            //
            for (ProtoObjectList::const_iterator objectsIt = sdf.objects().begin();
                    objectsIt != sdf.objects().end();
                    objectsIt++) {
                //
                // Create the object.
                //
                m_pUObject = m_pScene->CreateObject();
                ASSERT(m_pUObject != NULL);
                m_pUObject->SetName(objectsIt->name().c_str());

                //
                // Added systems extension.
                //
                for (ProtoObjectPropertiesList::const_iterator objectPropertiesIt = objectsIt->objectproperties().begin();
                        objectPropertiesIt != objectsIt->objectproperties().end();
                        objectPropertiesIt++) {
                    m_pSystem = Singletons::SystemManager.Get(objectPropertiesIt->systemtype().c_str());
                    ASSERTMSG1(m_pSystem != NULL, "Parser was unable to get system %s.", objectPropertiesIt->systemtype().c_str());

                    if (m_pSystem != NULL) {
                        UScene::SystemScenesConstIt it = m_pScene->GetSystemScenes().find(m_pSystem->GetSystemType());
                        ASSERTMSG1(it != m_pScene->GetSystemScenes().end(),
                                   "Parser was unable to find a scene for the system %s.", m_pSystem->GetSystemType());
                        //
                        // Create object.
                        //
                        m_pSystemObject = m_pUObject->Extend(it->second, objectPropertiesIt->objecttype().c_str());
                        ASSERT(m_pSystemObject != NULL);

                        if (m_pSystemObject != NULL) {
                            //
                            // Initialize the extension.
                            //
                            m_pSystemObject->GetProperties(m_GetProperties);
                            ApplyProperties(static_cast<ProtoPropertyList>(objectPropertiesIt->properties()), m_GetProperties, m_SetProperties);
                            m_pSystemObject->Initialize(m_SetProperties);
                            m_GetProperties.clear();
                            m_SetProperties.clear();
                        }
                    }
                }
            }

            const UScene::SystemScenes Scenes = m_pScene->GetSystemScenes();

            for (UScene::SystemScenesConstIt it = Scenes.begin(); it != Scenes.end(); it++) {
                it->second->GlobalSceneStatusChanged(
                    ISystemScene::GlobalSceneStatus::PostLoadingObjects
                );
            }

            //
            // Initialize the links.
            //
            for (ProtoLinksList::const_iterator linksIt = sdf.links().begin();
                    linksIt != sdf.links().end();
                    linksIt++) {
                UObject* pSubject = m_pScene->FindObject(linksIt->subject().c_str());
                UObject* pObserver = m_pScene->FindObject(linksIt->observer().c_str());;
                std::string sSystemSubject = linksIt->subjectsystem();
                std::string sSystemObserver = linksIt->observersystem();
                ISystemObject* pSystemSubject = NULL;
                ISystemObject* pSystemObserver = NULL;
                ISystem* pSystem;

                //
                // Get the extension for the subject.
                //
                if (!sSystemSubject.empty()) {
                    pSystem = Singletons::SystemManager.Get(sSystemSubject.c_str());
                    pSystemSubject = pSubject->GetExtension(pSystem->GetSystemType());
                }

                //
                // Get the extension for the object.
                //
                pSystem = Singletons::SystemManager.Get(sSystemObserver.c_str());
                pSystemObserver = pObserver->GetExtension(pSystem->GetSystemType());

                //
                // Call the scene to register the links.
                //
                if (pSystemSubject != NULL) {
                    m_pScene->CreateObjectLink(pSystemSubject, pSystemObserver);
                } else {
                    m_pScene->CreateObjectLink(pSubject, pSystemObserver);
                }
            }

            break;
        }
    }
}

std::string
Framework::GDFParser::StartupScene(
    void
) {
    return m_Gdf.startupscene();
}

void
Framework::GDFParser::ApplyProperties(
    ProtoPropertyList& propertyList,
    Properties::Array& getProperties,
    Properties::Array& setProperties
) const {
    Properties::ConstIterator getPropIt = getProperties.end();
    size_t iProp = 0xFFFFFFFF;

    for (ProtoPropertyList::const_iterator prop = propertyList.begin();
            prop != propertyList.end();
            prop++) {
        //
        // Search for the name in the property array.
        //
        for (Properties::ConstIterator it = getProperties.begin();
                it != getProperties.end();
                it++) {
            //
            // If the name of the property match, it's good
            //
            if (strcmp(it->GetName(), prop->name().c_str()) == 0) {
                setProperties.push_back(*it);
                iProp = setProperties.size() - 1;
                getPropIt = it;
                break;
            }
        }

        ASSERTMSG1(getPropIt != getProperties.end(),
                   "Parser could not find the property named %s in the list given by the system.", prop->name());
        i32 iValue = 0;

        for (ProtoStringList::const_iterator value = prop->value().begin();
                value != prop->value().end();
                value++) {
            //
            // Read in the property.
            //
            switch (getPropIt->GetValueType(iValue)) {
                case Properties::Values::None:
                    ASSERTMSG1(false, "Parser encoutered a value '%s' with no type.", prop->name());
                    break;

                case Properties::Values::Boolean:
                case Properties::Values::Int32:
                    setProperties[ iProp ].SetValue(iValue, Convertor::StringToInt(*value));
                    break;

                case Properties::Values::Enum: {
                    for (i32 i = 0; getPropIt->GetEnumOption(i) != NULL; i++) {
                        if (strcmp((*value).c_str(), getPropIt->GetEnumOption(i)) == 0) {
                            setProperties[ iProp ].SetValue(iValue, i);
                            break;
                        }
                    }

                    break;
                }

                case Properties::Values::Float32:
                case Properties::Values::Vector3& Properties::Values::Mask:
                case Properties::Values::Vector4& Properties::Values::Mask:
                case Properties::Values::Quaternion& Properties::Values::Mask:
                case Properties::Values::Color3& Properties::Values::Mask:
                case Properties::Values::Color4& Properties::Values::Mask:
                    setProperties[ iProp ].SetValue(iValue, Convertor::StringToFloat(*value));
                    break;

                case Properties::Values::Angle:
                    setProperties[ iProp ].SetValue(
                        iValue, Math::Angle::Deg2Rad(Convertor::StringToFloat(*value))
                    );
                    break;

                case Properties::Values::String:
                case Properties::Values::Path:
                    setProperties[ iProp ].SetValue(iValue, *value);
                    break;

                default:
                    ASSERTMSG(false, "Parser encountered an unsupported property value.");
                    break;
            }

            iValue++;
        }
    }
}
