# PresentMon SDK

This is a C++ library that provides an interface to the PresentMon event tracing library. It is intended to be used by applications that need to capture and analyze the output of the PresentMon tool.

## Dependencies
- [MSVC](https://visualstudio.microsoft.com/downloads/)
  - Use the `Developer Powershell for VS 20XX` as terminal when compiling.
- [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell)
- [CMake](https://cmake.org/download/)
- [Ninja](https://ninja-build.org/)
    - You can install Ninja using [chocolatey](https://chocolatey.org/) by running `choco install ninja`.

## Building from Source

```bash
cmake -DCMAKE_BUILD_TYPE=Release --preset=vcpkg -G Ninja -S . -B build
cmake --build build
```