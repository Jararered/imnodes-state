cmake_minimum_required(VERSION 4.0)

FetchContent_Declare(
    GlfwGit
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
)

FetchContent_MakeAvailable(GlfwGit)