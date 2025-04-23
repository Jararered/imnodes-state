cmake_minimum_required(VERSION 4.0)

add_executable(NodeSimulation main.cpp)

target_sources(NodeSimulation PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/Window.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/Window/MenuBar.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/Node/NodeManager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/Node/Node.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/Node/NodeLayer.cpp
)

target_include_directories(NodeSimulation PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(NodeSimulation PRIVATE glad_gl glfw imgui)
