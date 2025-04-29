cmake_minimum_required(VERSION 4.0)

add_executable(NodeSimulation main.cpp)

target_include_directories(NodeSimulation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(NodeSimulation PRIVATE glad_gl glfw imgui)

add_subdirectory(Node)
add_subdirectory(Simulation)
add_subdirectory(Window)