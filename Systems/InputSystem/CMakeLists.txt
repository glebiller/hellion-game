cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(InputSystem LANGUAGES CXX)

add_library(InputSystem SHARED
        include/component/InputTriggerComponent.h
        include/component/InputVelocityComponent.h
        include/Object/Object.h
        include/Object/CameraInputObject.h
        include/Object/PlayerObject.h
        include/Scene.h
        include/System.h
        include/Task.h
        src/component/InputTriggerComponent.cpp
        src/component/InputVelocityComponent.cpp
        src/Object/Object.cpp
        src/Object/CameraInputObject.cpp
        src/Object/PlayerObject.cpp
        src/InputSystem.cpp
        src/Scene.cpp
        src/System.cpp
        src/GraphicTask.cpp
        )

target_include_directories(InputSystem PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

target_compile_features(InputSystem
        PUBLIC cxx_auto_type
        PRIVATE cxx_variadic_templates
        )

target_link_libraries(InputSystem
        Framework
        OIS
        OISB
        )

install(TARGETS InputSystem
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin
        )
install(DIRECTORY include/ DESTINATION include)

export(TARGETS InputSystem FILE InputSystemConfig.cmake)
