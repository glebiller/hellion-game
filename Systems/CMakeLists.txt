cmake_minimum_required(VERSION 2.8)

project("Systems")

check_root_module()

add_project_module("GraphicSystem")
add_project_module("InputSystem")
add_project_module("NetworkSystem")
add_project_module("PhysicSystem")
