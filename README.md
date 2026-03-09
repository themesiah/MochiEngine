# Mochi Engine

## Table of contents
1. [Introduction](#introduction)
2. [Architecture](#architecture-overview)
3. [Important classes](#important-classes)
4. [Building](#configuring-and-building)
5. [See more](#see-more)

## Introduction

MochiEngine is a personal project of a 2D pixel art game engine based on [SDL3](https://github.com/libsdl-org/SDL) with the purpose of being used as a portfolio of general C++ knowledge and [architecture](./ARCHITECTURE.md) skills.
This means that the engine has been purposedly prepare to work with 2D renderers and specifically pixel art. Any other usage of this engine has not been tested.

If you stumbled across this project and want to try it, be free to do so!

Next there are instructions to build the sample projects and documentation with which you can start building games.

## Architecture overview

Check first the [architecture overview here](./ARCHITECTURE.md).

## Important classes

Check those classes docs now for a quick overview of the most important code of the project:

- [Engine](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_engine.html) [(src)](./Engine/src/Engine.cpp)
- [Layer](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_layer.html) [(src)](./Engine/src/Layer.cpp)
- [SDL Renderer](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_graphics_1_1_s_d_l_renderer.html) [(src)](./Engine/src/Graphics/SDL/SDLRenderer.cpp)
- [Sprite Base](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_graphics_1_1_sprite_base.html) [(src)](./Engine/src/Graphics/SpriteBase.cpp)
- [Pack catalog](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_f_s_1_1_pack_catalog.html) [(src)](./Engine/src/Packer/PackCatalog.cpp)
- [Scripting manager](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_scripting_1_1_scripting_manager.html) [(src)](./Engine/src/Scripting/ScriptingManager.cpp)

## Configuring and building

MochiEngine uses cmake to build its projects.

Configuring the project generates CMake intermediate files and downloads all dependencies. The first time this is done
it can take a while, as it downloads SDL3 and some of its modules, a lua distribution, and other utility libraries.

Configure:
```cmake
cmake -B build
```
This will generate the configuration on a new folder named "build".

```cmake
cmake --B build -DUSE_FMOD=ON
```
By default, MochiEngine uses SDL3_Mixer as an audio backend. You can configure CMake to use FMOD as an audio backend setting USE_FMOD to ON.


Build:

```cmake
cmake --build build --config <CONFIG>
```
To build all targets (including tests) on a specific configuration. Configurations are **Debug** and **Release**.


```cmake
cmake --build build --config <CONFIG> --target <SAMPLE_TARGET>
```
To build one of the samples with an specific configuration. Currently the only sample is **SpaceShooter**.

```cmake
cd build
ctest -C Debug
```
To run all tests. Do this only after building all targets.


## See more

- [Architecture](./ARCHITECTURE.md)
- [Quick start](./QUICK_START.md)
- [Space Shooter Sample](./Samples/SpaceShooter/)
- [Documentation](https://themesiah.github.io/MochiEngine/Docs/index.html)