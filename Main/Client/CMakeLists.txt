cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(Client LANGUAGES CXX)

add_executable(Client
        src/Main.cpp
        )

target_link_libraries(Client
        Framework
        GraphicSystem
        InputSystem
        PhysicSystem
        )