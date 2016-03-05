cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(GraphicSystem LANGUAGES CXX)

find_package(Ogre REQUIRED COMPONENTS Overlay Terrain)

add_library(GraphicSystem SHARED
        src/Listener/CustomLogListener.cpp
        src/Object/CameraGraphicObject.cpp
        src/Object/CursorGraphicObject.cpp
        src/Object/LightGraphicObject.cpp
        src/Object/MeshGraphicObject.cpp
        src/Object/Object.cpp
        src/Object/ParticleGraphicObject.cpp
        src/Object/SkyGraphicObject.cpp
        src/Object/TerrainGraphicObject.cpp
        src/GraphicSystem.cpp
        src/Scene.cpp
        src/System.cpp
        src/Task.cpp
        )

target_include_directories(GraphicSystem PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${OGRE_INCLUDE_DIR}
        ${OGRE_Terrain_INCLUDE_DIR}
        ${OGRE_Overlay_INCLUDE_DIR}
        )

target_compile_features(GraphicSystem
        PUBLIC cxx_auto_type
        PRIVATE cxx_variadic_templates
        )

target_link_libraries(GraphicSystem
        Boost
        Boost-log
        Framework
        ${OGRE_LIBRARIES}
        ${OGRE_Overlay_LIBRARIES}
        ${OGRE_Terrain_LIBRARIES}
        )

install(TARGETS GraphicSystem
        ARCHIVE  DESTINATION lib
        LIBRARY  DESTINATION lib
        RUNTIME  DESTINATION bin)  # This is for Windows
install(DIRECTORY include/ DESTINATION include)

export(TARGETS GraphicSystem FILE GraphicSystemConfig.cmake)
