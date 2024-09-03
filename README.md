<div style="display: block;" align="center">
    <img class="only-dark" width="10%" height="10%" src="https://crossgl.github.io/_static/logo.png#gh-dark-mode-only"/>
</div>

------------------------------------------------------------------------

<div style="display: block;" align="center">
    <img class="dark-light" width="5%" >
    <a href="https://crossgl.net/">
        <img class="dark-light" height="5%" width="5%" src="https://github.com/samthakur587/docs2.0/blob/main/web_icon.png">
    </a>
    <img class="dark-light" width="5%" >
    <a href="https://crossgl.github.io/language.html">
        <img class="dark-light" height="5%" width="5%" src="https://github.com/samthakur587/docs2.0/blob/main/docs.png">
    </a>
    <img class="dark-light" width="5%" >
    <a href="https://github.com/CrossGL/demos">
        <img class="dark-light" height="5%" width="5%" src="https://github.com/samthakur587/docs2.0/blob/main/written.png">
    </a>
    <img class="dark-light" width="5%" >
    <a href="https://crossgl.github.io/design.html">
        <img class="dark-light" height="5%" width="5%" src="https://github.com/samthakur587/docs2.0/blob/main/strategic-plan.png">
    </a>
</div>

<br>

<div style="margin-top: 10px; margin-bottom: 10px; display: block;" align="center">
    <a href="https://github.com/CrossGL/crossvl/issues">
        <img class="dark-light" style="padding-right: 4px; padding-bottom: 4px;" src="https://img.shields.io/github/issues/CrossGL/crossvl">
    </a>
    <a href="https://github.com/CrossGL/crossvl/network/members">
        <img class="dark-light" style="padding-right: 4px; padding-bottom: 4px;" src="https://img.shields.io/github/forks/CrossGL/crossvl">
    </a>
    <a href="https://github.com/CrossGL/crossvl/stargazers">
        <img class="dark-light" style="padding-right: 4px; padding-bottom: 4px;" src="https://img.shields.io/github/stars/CrossGL/crossvl">
    </a>
    <a href="https://github.com/CrossGL/crossvl/pulls">
        <img class="dark-light" style="padding-right: 4px; padding-bottom: 4px;" src="https://img.shields.io/badge/PRs-welcome-brightgreen.svg">
    </a>
    <a href="https://discord.com/invite/uyRQKXhcyW">
        <img class="dark-light" style="padding-right: 4px; padding-bottom: 4px;" src="https://img.shields.io/discord/1240998239206113330?color=blue&label=%20&logo=discord&logoColor=white">
    </a>
</div>
<br clear="all" />

# CrossVL

CrossVL is an in-house tool for loading and testing transpiled shader files. The transpiled shader files are generated using [CrossTL](https://github.com/CrossGL/crosstl)
- Metal Shaders
- GLSL Shaders
- HLSL Shaders


| Platform |     DX11      |      DX12       |   OpenGL    |     Metal     |     Vulkan      |
|----------|---------------|-----------------|-------------|---------------|-----------------|
| **Windows**  | In Progress   | Not Implemented | In Progress | Not Supported | Not Implemented |
| **Linux**    | Not Supported | Not Supported   | In Progress | Not Supported | Not Implemented |
| **MacOSX**   | Not Supported | Not Supported   | Not Supported | In Progress   | Not Implemented |

# Build Status
[![Windows](https://github.com/CrossGL/visualizer/actions/workflows/windows.yml/badge.svg?branch=main)](https://github.com/CrossGL/visualizer/actions/workflows/windows.yml)

[![MacOSX](https://github.com/CrossGL/visualizer/actions/workflows/macosx.yml/badge.svg)](https://github.com/CrossGL/visualizer/actions/workflows/macosx.yml)

[![Linux](https://github.com/CrossGL/visualizer/actions/workflows/linux.yml/badge.svg)](https://github.com/CrossGL/visualizer/actions/workflows/linux.yml)

# Project Structure

The project is divided into two sub projects

- **VisualizerCore**: Static library that contains the core functionality
- **VisualizerApp**: Contains the platform independent GUI application that depends on the Core

# Getting started

To build the project ensure you have the following toolsets and external libraries installed.
- [Xmake](https://xmake.io/)
- C++23 compiler

### External Dependencies

- [SDL2](https://libsdl.org/)
- NCurses (linux/mac only)

## Clone the repository

```
git clone https://github.com/CrossGL/visualizer.git
```

## Load Build Macros

The repo comes with a couple helper xmake macros. Load these macros by running the [LoadMacros.bat](./LoadMacros.bat) or [LoadMacros.sh](./LoadMacros.sh) file depending on the OS. **You only need to run this bat script only once, or if you delete the generated *`.xmake`* folder**.

After running the script the following macros will be imported (case insensitive):

- **BuildAllConfigs**: Builds the visualizer for both debug and release configurations
- **Clean**: Cleans the xmake configs and targets
- **DeepClean**: Similar to the *Clean* macro but also deletes the build folder
- **WatchDir**: Compiles the project on every Edit and Save.
- **GenVS**: Generates Visual Studio 2022 solution files
- **GenXCode**: Generates XCode project files

You can run these above macros using the following command

```bash
xmake macro <macro name>
```

## Choosing The Build Configuration

Before compiling the project you can also set the build config

To make use of the debug config

```bash
xmake f -m debug
```

To make use of the release config

```bash
xmake f -m release
```

## Compiling The Project

### Using The Macros

Build all the configs

```bash
xmake macro BuildAllConfigs
```

### Compiling Using Visual Studio

Generate VS 2022 files

```bash
xmake macro GenVS
```

This will generate the solution files in the vsxmake2022 folder

### Compiling Using CMake

Generate CMakeLists.txt

```bash
xmake project -k cmakelists
```

This will generate the CMakeLists.txt in the root folder

# 🌈 Example Usage

To test your own `.obj` models, place the model file in the `Samples/ModelLoading/Assets/` directory. Then, update the filename in `ModelLoading.cpp` to match your `.obj` file.

Here `barrel.obj` model is used to show as an example, the output would look like :

<p align="center">
  <img src="Output/Output.gif" alt="animated" />
</p>

*Get the `barrel.obj` model from Output folder to test it out yourself.

# Contributing

When contributing to the project. Ensure the project passes the compilation test locally on your PC.

```bash
xmake rhitest -p <platform>
```
Where the valid platforms are:
- windows
- linux
- macosx

This will run a series of matrix test for each valid API for that platform (e.g. for macosx it will test compilation for opengl, metal and vulkan). Make sure to test in both debug and release build configs.

The complete testing process should look as follows (using windows as example):
```bash
xmake f -m debug
xmake rhitest -p windows

xmake f -m release
xmake rhitest -p windows
```

The expected output should be such that all projects compile and link successfully
