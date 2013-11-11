cmake_minimum_required(VERSION 2.8)

project("Client")

check_root_module()

add_project_dependency("Framework")
add_project_sources("Source")
list(REMOVE_ITEM "${PROJECT_NAME}_DEPENDENCIES" ${PROJECT_NAME})
add_executable(${PROJECT_NAME} WIN32 ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES} 
    "GraphicSystem" "InputSystem" "NetworkSystem" "PhysicSystem")
target_link_libraries(${PROJECT_NAME} "Pdh.lib" ${${PROJECT_NAME}_DEPENDENCIES}
    ${PROTOBUF_LIBRARIES} ${LOGOG_LIBRARIES})
