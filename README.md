# Visualizer

tool for loading and testing shader files. [ experimental ]
- Metal Shaders
- GLSL Shaders

# Building on Windows

Download the Requirements:
- [Visual Studio 2022](https://visualstudio.microsoft.com/) IDE
- use cmake and vcpkg that comes with Visual Studio

Open up the Developer PowerShell for VS 2022 and navigate to the directory that contains `vcpkg.json`, then run:

```shell
vcpkg x-update-baseline --add-initial-baseline
```

this adds the **builtin-baseline** to the manifest file, 
then run the following commands to build the project:

```
cmake --preset win-release
cmake --build --preset Release
```