cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(InputSystem LANGUAGES CXX)

find_package(OIS REQUIRED)
find_package(OISB REQUIRED)

add_library(InputSystem SHARED
        src/Object/Object.cpp
        src/Object/PlayerObject.cpp
        src/InputSystem.cpp
        src/Scene.cpp
        src/System.cpp
        src/Task.cpp
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
        Boost
        Boost-log
        Framework
        OIS
        OISB
        )

install(TARGETS InputSystem
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

export(TARGETS InputSystem FILE InputSystemConfig.cmake)

