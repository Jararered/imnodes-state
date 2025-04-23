cmake_minimum_required(VERSION 4.0)

project(ImGui-Blank-Slate)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build tests" ON)

add_subdirectory(cmake)
add_subdirectory(source)
