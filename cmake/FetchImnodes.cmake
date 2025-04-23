FetchContent_Declare(
    imnodes
    GIT_REPOSITORY https://github.com/Nelarius/imnodes
    GIT_TAG v0.5
)

FetchContent_MakeAvailable(imnodes)

target_sources(imgui PUBLIC
    ${imnodes_SOURCE_DIR}/imnodes.cpp
)

target_include_directories(imgui PUBLIC ${imnodes_SOURCE_DIR})

# Defined this here so that imgui_internal.h stops complaining abont imnodes not 
# defining this before including imgui.h (this library is for an older version of imgui)
target_compile_definitions(imgui PUBLIC IMGUI_DEFINE_MATH_OPERATORS)