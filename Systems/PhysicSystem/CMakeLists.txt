cmake_minimum_required(VERSION 2.8)

project("PhysicSystem")

check_root_module()

find_package("Ogre" REQUIRED)
find_package("Havok" REQUIRED COMPONENTS "hkCompat" REQUIRED "hkSceneData" REQUIRED 
    "hkSerialize" REQUIRED "hkInternal" REQUIRED "hkGeometryUtilities" REQUIRED 
    "hkVisualize" REQUIRED "hkcdInternal" REQUIRED "hkcdCollide" REQUIRED 
    "hkpCollide" REQUIRED "hkpConstraint" REQUIRED "hkpConstraintSolver" REQUIRED 
    "hkpDynamics" REQUIRED "hkpInternal" REQUIRED "hkpUtilities" REQUIRED 
    "hkpVehicle" REQUIRED)
include_directories(${OGRE_INCLUDE_DIRS} ${HAVOK_INCLUDE_DIRS})

add_project_dependency("Framework")
add_project_sources("Source")
add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES}
    "${PROJECT_NAME}.def")
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES})
target_link_libraries(${PROJECT_NAME} ${${PROJECT_NAME}_DEPENDENCIES} 
    ${PROTOBUF_LIBRARIES} ${LOGOG_LIBRARIES}
    ${OGRE_LIBRARIES} ${HAVOK_LIBRARIES})
