libgba
===

A basic GBA library + a cmake wrapper + some python tools.
Examples and experiments using this library are available on [gba-experiments](https://github.com/tarcisiofischer/gba-experiments).

Cmake example usage
---

```
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/libgba/cmake)
include(GBAExecutable)

gba_exec(
  NAME hello-world
  SOURCES
  hello-world/hello-world.cc
)
```

Make sures `libgba/tools` is in your `PATH`, if you want to add `SPRITESHEETS` and/or a `TILEMAP`.
The `TILEMAP`s are generated using the [Tiled Map Editor](https://github.com/mapeditor/tiled).
`SPRITESHEETS` are generated from image files.

```
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
