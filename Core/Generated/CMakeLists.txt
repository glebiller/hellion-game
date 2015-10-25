cmake_minimum_required(VERSION 2.8)

project("Generated")

if(MSVC)
    add_definitions("/wd4996")
endif(MSVC)

check_root_module()

add_project_sources("Source")
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE C++)
