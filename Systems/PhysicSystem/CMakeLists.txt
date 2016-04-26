cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(PhysicSystem LANGUAGES CXX)

find_package(Bullet REQUIRED)
find_package(Ogre REQUIRED)

add_library(PhysicSystem SHARED
        include/DebugDrawer.h
        include/Object/CharacterPhysicObject.h
        include/Object/MovablePhysicObject.h
        include/Object/PhysicObject.h
        include/Object/TerrainPhysicObject.h
        include/Scene.h
        include/System.h
        include/Task.H
        src/DebugDrawer.cpp
        src/Object/CharacterPhysicObject.cpp
        src/Object/MovablePhysicObject.cpp
        src/Object/PhysicObject.cpp
        src/Object/TerrainPhysicObject.cpp
        src/PhysicSystemMain.cpp
        src/Scene.cpp
        src/PhysicSystem.cpp
        src/Task.cpp
        )

target_include_directories(PhysicSystem PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${BULLET_INCLUDE_DIRS}
        ${OGRE_INCLUDE_DIR}
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
        ${OGRE_LIBRARIES}
        )

install(TARGETS PhysicSystem
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

export(TARGETS PhysicSystem FILE PhysicSystemConfig.cmake)
