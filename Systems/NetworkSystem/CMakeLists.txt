cmake_minimum_required(VERSION 2.8)

project("NetworkSystem")

check_root_module()

add_project_dependency("Framework")
add_project_sources("Source")
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES}
    "${PROJECT_NAME}.def")
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES})
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES} 
    ${PROTOBUF_LIBRARIES} ${LOGOG_LIBRARIES})
