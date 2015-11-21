cmake_minimum_required(VERSION 2.8)

project("GraphicSystem")

check_root_module()

find_package("Ogre" COMPONENTS "Overlay" REQUIRED "Terrain" REQUIRED)
find_package("CEF" REQUIRED)
include_directories(${OGRE_INCLUDE_DIR} ${CEF_INCLUDE_DIR})

add_project_dependency("Framework")
add_project_sources("Source")
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES}
    "${PROJECT_NAME}.def")
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES})
message(WARNING ${CEF_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES}
                                      ${Boost_LOG_LIBRARY}
                                      ${OGRE_LIBRARIES} ${OGRE_Terrain_LIBRARIES} ${OGRE_Overlay_LIBRARIES}
                                      ${CEF_LIBRARIES})
