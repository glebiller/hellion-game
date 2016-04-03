cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(PhysicSystem LANGUAGES CXX)

find_package(Bullet REQUIRED)
find_package(PNG REQUIRED)

add_library(PhysicSystem SHARED
        include/Object/MovablePhysicObject.h
        include/Object/PhysicObject.h
        include/Object/TerrainPhysicObject.h
        include/Scene.h
        src/Object/MovablePhysicObject.cpp
        src/Object/PhysicObject.cpp
        src/Object/TerrainPhysicObject.cpp
        src/PhysicSystem.cpp
        src/Scene.cpp
        src/GraphicSystem.cpp
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
        ${PNG_LIBRARIES}
        )

install(TARGETS PhysicSystem
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

export(TARGETS PhysicSystem FILE PhysicSystemConfig.cmake)
