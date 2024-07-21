# visualizer

tool for loading and testing shader files.
- Metal Shaders
- GLSL Shaders

## Building on Windows

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

## Building on macOS

Download `cmake`, and `pkg-config` via Homebrew, then clone the `vcpkg` repo:
```
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg && ./bootstrap-vcpkg.sh
```
then set `VCPKG_ROOT` variable and add the `vcpkg` to the path in the `.zshrc` file located in the `HOME` directory:

```shell
export VCPKG_ROOT=/path/to/vcpkg
export PATH="$VCPKG_ROOT:$PATH"
```

clone the `visualizer` and then run the following commands to build the project:

```shell
cmake --preset mac-release
cmake --build --preset ReleaseOSX
```