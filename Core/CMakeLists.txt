cmake_minimum_required(VERSION 2.8)

project("Core")

check_root_module()

add_project_module("BaseTypes")
add_project_module("Framework")
add_project_module("Generated")
add_project_module("Interfaces")
