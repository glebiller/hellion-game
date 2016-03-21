cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(HellionGame LANGUAGES CXX)

LIST(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/CMake)

# Include Boost
set(Boost_FIND_QUIETLY ON)
add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost REQUIRED COMPONENTS log timer thread system filesystem)
add_library(Boost INTERFACE IMPORTED)
set_property(TARGET Boost PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})

add_library(Boost-log SHARED IMPORTED)
set_property(TARGET Boost-log PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
set_property(TARGET Boost-log PROPERTY
        IMPORTED_LOCATION ${Boost_LOG_LIBRARY})

add_library(Boost-timer SHARED IMPORTED)
set_property(TARGET Boost-timer PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
set_property(TARGET Boost-timer PROPERTY
        IMPORTED_LOCATION ${Boost_TIMER_LIBRARY})

add_library(Boost-thread SHARED IMPORTED)
set_property(TARGET Boost-thread PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
set_property(TARGET Boost-thread PROPERTY
        IMPORTED_LOCATION ${Boost_THREAD_LIBRARY})

add_library(Boost-system SHARED IMPORTED)
set_property(TARGET Boost-system PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
set_property(TARGET Boost-system PROPERTY
        IMPORTED_LOCATION ${Boost_SYSTEM_LIBRARY})

add_library(Boost-filesystem SHARED IMPORTED)
set_property(TARGET Boost-filesystem PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIRS})
set_property(TARGET Boost-filesystem PROPERTY
        IMPORTED_LOCATION ${Boost_FILESYSTEM_LIBRARY})

# Include FlatBuffers
find_package(FlatBuffers REQUIRED)
add_library(FlatBuffers INTERFACE IMPORTED)
set_property(TARGET FlatBuffers PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${FLATBUFFERS_INCLUDE_DIR})

# Include TBB
find_package(TBB REQUIRED)
add_library(TBB INTERFACE IMPORTED)
set_property(TARGET TBB PROPERTY
        INTERFACE_INCLUDE_DIRECTORIES ${TBB_INCLUDE_DIR})
set_property(TARGET TBB PROPERTY
        INTERFACE_LINK_LIBRARIES ${TBB_LIBRARIES})

#enable_testing()
add_subdirectory(Deps)
add_subdirectory(Framework)
add_subdirectory(Main)
add_subdirectory(Systems)

