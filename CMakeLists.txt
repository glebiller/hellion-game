cmake_minimum_required(VERSION 2.8)

project("HellionGame")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} CACHE PATH "Directory for Libraries")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} CACHE PATH "Directory for Executables.")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR} CACHE PATH "Directory for Static Libraries.")

if(UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -std=c++11")
endif(UNIX)
if(MSVC)
    add_definitions("/wd4099 /nologo")
endif(MSVC)

set(Boost_FIND_QUIETLY ON)
set(Boost_USE_STATIC_LIBS ON)

include("ProjectUtils")
find_package("Boost" COMPONENTS thread REQUIRED)
find_package("TBB" REQUIRED)
find_package("Protobuf" REQUIRED)
find_package("FlatBuffers" REQUIRED)
find_package("Logog" REQUIRED)

include_directories(${Boost_INCLUDE_DIRS} ${TBB_INCLUDE_DIRS} 
    ${PROTOBUF_INCLUDE_DIRS} ${FLATBUFFERS_INCLUDE_DIR} ${LOGOG_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS} ${TBB_LIBRARY_DIRS})

add_subdirectory("Core")
add_subdirectory("Systems")
add_subdirectory("Main")
