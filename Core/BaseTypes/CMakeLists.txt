cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(BaseTypes LANGUAGES CXX)

add_library(BaseTypes
        src/ObjectId/ObjectId.cpp
        src/MathUtils.cpp
        )

target_include_directories(BaseTypes PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )

target_compile_features(BaseTypes
        PUBLIC cxx_auto_type
        )

target_link_libraries(BaseTypes
        Boost
        )

install(TARGETS BaseTypes
        ARCHIVE DESTINATION lib
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
        )
install(DIRECTORY include/ DESTINATION include)

export(TARGETS BaseTypes
        FILE BaseTypesConfig.cmake
        )

# Testing
if (BUILD_TEST)
    find_package(GTest REQUIRED)

    include_directories(${GTEST_INCLUDE_DIRS})
    aux_source_directory(Test BaseTypesTest_SOURCES)

    add_executable(BaseTypesTest ${BaseTypesTest_SOURCES})
    target_link_libraries(BaseTypesTest BaseTypes pthread ${GTEST_BOTH_LIBRARIES})

    add_test(SingletonTest BaseTypesTest)
endif (BUILD_TEST)