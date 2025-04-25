# Node Simulation

## What

This is a small project that I started to experiment around with being able to connect and simulate differrent environments using nodes and linking them together.

## Why

Part of the challenge of this project was to use the CMake function FetchContent to be able to handle automatically grabbing all of the dependencies to configure GLAD/GLFW/IMGUI/CATCH2, without and manual downloading/generation of files.

## How

### Requirements

- Python
- CMake
- GCC

### Building

1. Configuring the CMake project

As with any CMake project, configure it to allow generators/compilers to be found:

```bash
cmake -S . -B ./build
```

2. Building

Due to the way GLAD generates it's files, building the project once is currently required for #include statements to get corrected in the code. The glad.c and glad.h files ares generated, compiled into a static library during the build process.

```bash
cmake --build ./build --config Debug --target all --
```
![Node Editor Screenshot](https://i.imgur.com/Hs2Iqxz.png)
