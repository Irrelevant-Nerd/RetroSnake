# 2. Building and Running

[Back to README](README.md) · Purpose: get RetroSnake compiled and running, and fix the usual problems.

> **Not run here.** These steps come from `CMakeLists.txt` and the files in the shipped `build/` folder (CMake 3.30.4, Ninja, MinGW GCC 14.2.0, Windows). They were not executed in this environment.

## Prerequisites

| Need | Details |
|---|---|
| Compiler | A C++17 compiler. The author used MinGW-w64 GCC 14.2.0 (`C:/mingw64/bin`) |
| CMake | 3.10 or newer (3.30.4 was used) |
| Ninja | Used as the generator in the shipped `build/` folder (`C:/mingw64/bin/ninja.exe`) |
| SFML 3.1.0 for MinGW | Expected at `C:/SFML-3.1.0_mingw64` (see below) |

SFML must be the build made for **your** compiler. A MinGW build of SFML will not work with MSVC, and the reverse.

## Where SFML is found

`CMakeLists.txt` hard-codes the SFML location:

```cmake
set(SFML_DIR "C:/SFML-3.1.0_mingw64/lib/cmake/SFML")
find_package(SFML 3.1 COMPONENTS Graphics Window System Audio Network REQUIRED)
```

If SFML lives elsewhere, change that path to the folder that contains `SFMLConfig.cmake` (inside `lib/cmake/SFML`). Because Audio and Network are listed as `REQUIRED`, those parts of SFML must be installed too, even though the game does not use them.

## Build steps (Windows, MinGW, Ninja)

Run from the project root (the folder containing `CMakeLists.txt`):

```bat
cmake -S . -B build -G Ninja
cmake --build build
```

This produces `build/RetroSnake.exe`. Source files are found automatically: the script collects every `.cpp` under `src/` (`file(GLOB_RECURSE ... CONFIGURE_DEPENDS)`), so adding a new `.cpp` file to `src/` needs no change to `CMakeLists.txt`. Headers are found through `include/`.

## Running

The game loads its font with a **relative** path, `../assets/fonts/HomeVideo-BLG6G.ttf`, so the program's *working directory* must be one level below the project root, for example `build/`.

```bat
cd build
RetroSnake.exe
```

Double-clicking `build/RetroSnake.exe` in Explorer also works, because Windows then uses the exe's folder as the working directory.

The SFML DLLs must be next to the exe (or on your `PATH`). The shipped `build/` folder already contains them (`sfml-graphics-3.dll`, `sfml-window-3.dll`, `sfml-system-3.dll`, `sfml-audio-3.dll`, `sfml-network-3.dll`, plus matching `-d-3.dll` debug versions). Nothing in `CMakeLists.txt` copies them, so a fresh build folder needs them copied in from `C:/SFML-3.1.0_mingw64/bin`.

## Build variants

| Variant | Status |
|---|---|
| Debug / Release | `CMAKE_BUILD_TYPE` is empty in the shipped cache, and the code has no `#ifdef`, `assert`, or `NDEBUG` checks, so behavior does not differ |
| Static / shared SFML | The shipped build links the DLLs (shared) |
| Other platforms | Nothing in the code is Windows-specific, but `CMakeLists.txt` and `.vscode/c_cpp_properties.json` contain Windows-only paths and would need editing |

## Editor setup

`.vscode/c_cpp_properties.json` configures VS Code IntelliSense only (it does not build anything). It points at `C:/mingw64/bin/g++.exe`, `C:\SFML-3.1.0_mingw64\include`, and a vcpkg include path, with C++ standard `gnu++17`.

## Troubleshooting

| Symptom | Cause | Fix |
|---|---|---|
| CMake says it cannot find SFML | `SFML_DIR` path is wrong, or SFML is for a different compiler | Fix the path in `CMakeLists.txt`; install the SFML build that matches your compiler |
| CMake complains about missing Audio or Network | Those components are `REQUIRED` | Install the full SFML package, or remove them from `find_package` and from `target_link_libraries` (no code uses them) |
| `NMake` errors on Windows | CMake picked the Visual Studio/NMake generator | Configure with a MinGW-compatible generator: `cmake -S . -B build -G "MinGW Makefiles"` (then `cmake --build build`), or use `-G Ninja` as above |
| Program closes immediately, or error about `sf::Exception` | Font file not found because of the working directory | Run from `build/` (or any folder one level below the project root) |
| "The code execution cannot proceed because sfml-*.dll was not found" | DLLs are not next to the exe or on `PATH` | Copy the SFML DLLs into the folder with `RetroSnake.exe` |
