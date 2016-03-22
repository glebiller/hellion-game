cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(PhysicSystem LANGUAGES CXX)

find_package(Bullet REQUIRED)

add_library(PhysicSystem SHARED
        include/Scene.h
        src/Object/PhysicObject.cpp
        src/PhysicSystem.cpp
        src/Scene.cpp
        src/GraphicSystemSystem.cpp
        src/Task.cpp
        )

target_include_directories(PhysicSystem PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${BULLET_INCLUDE_DIRS}
        )

target_compile_features(PhysicSystem
        PUBLIC cxx_auto_type
        PRIVATE cxx_variadic_templates
        )

target_link_libraries(PhysicSystem
        Boost
        Boost-log
        Framework
        ${BULLET_LIBRARIES}
        )

install(TARGETS PhysicSystem
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

export(TARGETS PhysicSystem FILE PhysicSystemConfig.cmake)
