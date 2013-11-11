cmake_minimum_required(VERSION 2.8)

project("BaseTypes")

check_root_module()
add_project_dependency("Generated")
add_project_sources("Source")
add_library(${PROJECT_NAME} STATIC ${${PROJECT_NAME}_HEADERS} ${${PROJECT_NAME}_SOURCES})

# Testing
if(BUILD_TEST)
    find_package(GTest REQUIRED)
    
    include_directories(${GTEST_INCLUDE_DIRS})
    aux_source_directory(Test BaseTypesTest_SOURCES)

    add_executable(BaseTypesTest ${BaseTypesTest_SOURCES})
    target_link_libraries(BaseTypesTest BaseTypes pthread ${GTEST_BOTH_LIBRARIES})

    add_test(SingletonTest BaseTypesTest)
endif(BUILD_TEST)