cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Generated LANGUAGES CXX)

add_library(Generated INTERFACE)

target_include_directories(Generated INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>)

install(DIRECTORY include/ DESTINATION include)

export(TARGETS Generated FILE GeneratedConfig.cmake)