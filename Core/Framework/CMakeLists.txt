cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Framework LANGUAGES CXX)

add_library(Framework
        src/Debugger/Debugger.cpp
        src/Debugger/ObjectChangesDebugger.cpp
        src/Debugger/SceneChangesDebugger.cpp
        src/Generic/Framework.cpp
        src/Generic/Instrumentation.cpp
        src/Generic/IProperty.cpp
        src/Generic/ISubject.cpp
        src/Generic/Scheduler.cpp
        src/Manager/ChangeManager.cpp
        src/Manager/ServiceManager.cpp
        src/Manager/TaskManager.cpp
        src/Object/ISceneObject.cpp
        src/Service/DefinitionService.cpp
        src/Service/RuntimeService.cpp
        src/Service/SettingService.cpp
        src/Service/SystemService.cpp
        src/Service/WindowService.cpp
        src/System/ISystem.cpp
        src/System/ISystemObject.cpp
        src/System/ISystemScene.cpp
        src/System/ISystemTask.cpp
        src/Task/GenericCallbackData.cpp
        src/Task/ParallelForBody.cpp
        src/Task/StallTask.cpp
        src/Task/SynchronizeTask.cpp
        src/Universal/UObject.cpp
        src/Universal/UScene.cpp
        src/Main.cpp
        )

target_include_directories(Framework PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )


target_compile_features(Framework
        PUBLIC cxx_auto_type
        PRIVATE cxx_variadic_templates
        )

target_link_libraries(Framework
        Boost
        Boost-log
        Boost-timer
        Boost-thread
        Boost-system
        Boost-filesystem
        TBB
        Generated
        BaseTypes
        )

install(TARGETS Framework
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        )
install(DIRECTORY include/ DESTINATION include)

export(TARGETS Framework FILE FrameworkConfig.cmake)

