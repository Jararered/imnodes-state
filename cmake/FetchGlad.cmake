cmake_minimum_required(VERSION 4.0)

FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
    GIT_PROGRESS TRUE
    SOURCE_SUBDIR cmake
)

FetchContent_MakeAvailable(glad)

set(GLAD_GL_CORE_VERSION 3.3)

glad_add_library(glad_gl STATIC REPRODUCIBLE LOADER API gl:core=${GLAD_GL_CORE_VERSION})
