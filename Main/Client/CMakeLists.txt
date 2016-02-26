cmake_minimum_required(VERSION 2.8)

project("Client")

check_root_module()

add_project_dependency("Framework")
add_project_sources("Source")
list(REMOVE_ITEM "${PROJECT_NAME}_DEPENDENCIES" ${PROJECT_NAME})
add_executable(${PROJECT_NAME} WIN32 ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})
#add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES}
#    "GraphicSystem" "InputSystem" "NetworkSystem" "PhysicSystem")
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES}
            "GraphicSystem")
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES}
    ${LOGOG_LIBRARIES} ${TBB_LIBRARIES}
    ${Boost_THREAD_LIBRARY} ${Boost_LOG_LIBRARY} ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY} ${Boost_TIMER_LIBRARY})
