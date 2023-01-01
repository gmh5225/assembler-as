cmake_minimum_required(VERSION 3.0.0)
project(asx86)

add_compile_options(-std=c++17)

include_directories(.)

add_subdirectory(libelf)
add_subdirectory(src)

