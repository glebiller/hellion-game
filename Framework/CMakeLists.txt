cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Framework LANGUAGES CXX)

add_library(Framework
        include/schema/component_type_generated.h
        include/schema/graphic_components_generated.h
        include/schema/input_components_generated.h
        include/schema/physic_components_generated.h
        include/schema/graphic_system_generated.h
        include/schema/input_system_generated.h
        include/schema/network_system_generated.h
        include/schema/physic_system_generated.h
        include/schema/system_type_generated.h
        include/schema/entity_change_generated.h
        include/schema/environment_generated.h
        include/schema/scene_generated.h

        include/Debugger/Debugger.h
        include/Debugger/ObjectChangesDebugger.h
        include/Debugger/SceneChangesDebugger.h
        include/Generic/Framework.h
        include/Generic/Instrumentation.h
        include/Generic/IObserver.h
        include/Generic/Scheduler.h
        include/Manager/ChangeManager.h
        include/Manager/Notification.h
        include/Manager/ObserverRequest.h
        include/Manager/SubjectInfo.h
        include/Manager/TaskManager.h
        include/System/ISystem.h
        include/System/ISystemObject.h
        include/System/ISystemScene.h
        include/System/ISystemTask.h
        include/Task/GenericCallbackData.h
        include/Task/GenericCallbackTask.h
        include/Task/ParallelForBody.h
        include/Task/PerformanceHint.h
        include/Task/StallTask.h
        include/Task/SynchronizeTask.h
        include/Universal/UObject.h
        include/Universal/UScene.h
        include/DataTypes.h
        include/Defines.h
        include/Errors.h
        src/Debugger/Debugger.cpp
        src/Debugger/ObjectChangesDebugger.cpp
        src/Debugger/SceneChangesDebugger.cpp
        src/Generic/Framework.cpp
        src/Generic/Instrumentation.cpp
        src/Generic/Scheduler.cpp
        src/Manager/ChangeManager.cpp
        src/Manager/TaskManager.cpp
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
        )

target_include_directories(Framework PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>
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
        )

install(TARGETS Framework
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        )
install(DIRECTORY include/ DESTINATION include)

export(TARGETS Framework FILE FrameworkConfig.cmake)


# Schema Components Generation

add_custom_command(
        OUTPUT include/schema/component_type_generated.h
        COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE}
        --cpp --scoped-enums -o include/schema/ -I ${CMAKE_CURRENT_SOURCE_DIR}/fbs/
        ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/components/component_type.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/components/component_type.fbs
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/schema/graphic_components_generated.h
        COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE}
        --cpp --scoped-enums --gen-mutable -o include/schema/ -I ${CMAKE_CURRENT_SOURCE_DIR}/fbs/
        ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/components/graphic_components.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/components/graphic_components.fbs
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/schema/input_components_generated.h
        COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE}
        --cpp --scoped-enums --gen-mutable -o include/schema/ -I ${CMAKE_CURRENT_SOURCE_DIR}/fbs/
        ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/components/input_components.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/components/input_components.fbs
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/schema/physic_components_generated.h
        COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE}
        --cpp --scoped-enums --gen-mutable -o include/schema/ -I ${CMAKE_CURRENT_SOURCE_DIR}/fbs/
        ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/components/physic_components.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/components/physic_components.fbs
)

# Schema Entity Generation

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/schema/entity_change_generated.h
        COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE}
        --cpp --no-prefix -o include/schema/ -I ${CMAKE_CURRENT_SOURCE_DIR}/fbs/
        ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/entity/entity_change.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/entity/entity_change.fbs
)

# Schema Systems Generation

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/schema/graphic_system_generated.h
        COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE}
        --cpp --scoped-enums -o include/schema/ -I ${CMAKE_CURRENT_SOURCE_DIR}/fbs/
        ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/systems/graphic_system.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/systems/graphic_system.fbs
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/schema/input_system_generated.h
        COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE}
        --cpp --scoped-enums -o include/schema/ -I ${CMAKE_CURRENT_SOURCE_DIR}/fbs/
        ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/systems/input_system.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/systems/input_system.fbs
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/schema/network_system_generated.h
        COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE}
        --cpp --scoped-enums -o include/schema/ -I ${CMAKE_CURRENT_SOURCE_DIR}/fbs/
        ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/systems/network_system.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/systems/network_system.fbs
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/schema/physic_system_generated.h
        COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE}
        --cpp --scoped-enums -o include/schema/ -I ${CMAKE_CURRENT_SOURCE_DIR}/fbs/
        ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/systems/physic_system.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/systems/physic_system.fbs
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/schema/system_type_generated.h
        COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE}
        --cpp --scoped-enums -o include/schema/ -I ${CMAKE_CURRENT_SOURCE_DIR}/fbs/
        ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/systems/system_type.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/systems/system_type.fbs
)

# Schema Generation

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/schema/environment_generated.h
        COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE}
        --cpp --scoped-enums -o include/schema/ -I ${CMAKE_CURRENT_SOURCE_DIR}/fbs/
        ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/environment.fbs
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/environment.fbs
)

add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/include/schema/scene_generated.h
        COMMAND ${FLATBUFFERS_FLATC_EXECUTABLE}
        --cpp --scoped-enums -o include/schema/ -I ${CMAKE_CURRENT_SOURCE_DIR}/fbs/
        ${CMAKE_CURRENT_SOURCE_DIR}/fbs/schema/scene.fbs
        DEPENDS fbs/schema/scene.fbs
)
