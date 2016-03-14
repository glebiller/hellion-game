cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Client LANGUAGES CXX)

add_executable(Client
        #src/Main.cpp
        src/CocoaMain.cpp
        )

target_link_libraries(Client
        Framework
        GraphicSystem
        InputSystem
        PhysicSystem
        )

if (APPLE)

    set_source_files_properties(src/CocoaMain.cpp
            PROPERTIES
            COMPILE_FLAGS "-x objective-c++"
            )

    set_target_properties(Client
            PROPERTIES
            #MACOSX_BUNDLE TRUE
            LINK_FLAGS "-sectcreate __TEXT __info_plist ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocoa/Info.plist"
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/src/Cocoa/Info.plist
            )

endif ()
