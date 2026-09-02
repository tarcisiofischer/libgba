libgba
===

A basic GBA library + a cmake wrapper + some python tools.
Examples and experiments using this library are available on [gba-experiments](https://github.com/tarcisiofischer/gba-experiments).

Requirements
---

A GBA-targeted LLVM/Clang build is required. Set the `GBA_LLVM` environment variable to point to it:

```
export GBA_LLVM=/path/to/gba-llvm/build
```

Building and installing
---

```
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/gba-toolchain.cmake -DCMAKE_INSTALL_PREFIX=build/artifacts
cmake --build build
cmake --install build
```

This installs the libraries, headers, linker script, and CMake integration files under the given prefix.

Using from a consuming project
---

Pass the toolchain file and the install location to your project's CMake configuration:

```
cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=$LIBGBA_PATH/lib/cmake/libgba/gba-toolchain.cmake \
  -DLIBGBA_PATH=$LIBGBA_PATH/lib/cmake/libgba
```

Where `LIBGBA_PATH` is the install prefix used above. In your `CMakeLists.txt`:

```cmake
find_package(libgba REQUIRED
  PATHS ${LIBGBA_PATH}
  NO_DEFAULT_PATH
)
```

Then use `gba_exec` to define GBA executables:

```cmake
gba_exec(
  NAME hello-world
  SOURCES
  hello-world/hello-world.cc
)
```

Make sure `libgba/tools` is in your `PATH` if you want to use `SPRITESHEETS` and/or a `TILEMAP`.
The `TILEMAP`s are generated using the [Tiled Map Editor](https://github.com/mapeditor/tiled).
`SPRITESHEETS` are generated from image files.

```cmake
gba_exec(
  NAME spritesheet
  SOURCES
  spritesheet/main.cc
  SPRITESHEETS
  spritesheet/chicken.png
  spritesheet/bg.png
  TILEMAP
  spritesheet/foo.json
)
```
