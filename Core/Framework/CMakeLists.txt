cmake_minimum_required(VERSION 2.8)

project(Framework)

check_root_module()

find_package("ZeroMQ" REQUIRED)
find_package("CEF" REQUIRED)
include_directories(${ZEROMQ_INCLUDE_DIRS} ${CEF_INCLUDE_DIR})

add_project_dependency("Interfaces")
add_project_sources("Source")
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES})
