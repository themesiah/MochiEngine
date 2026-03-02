# SDLSpaceShooter

# Table of contents
1. [Purpose](#Purpose)
2. [Building](#Configuring and building)
3. [Links of interest](#Links of interest)

## Purpose

blablabla

## Configuring and building

MochiEngine uses cmake to build its projects.

Configuring the project generates CMake intermediate files and downloads all dependencies. The first time this is done
it can take a while, as it downloads SDL3 and some of its modules, a lua distribution, and other utility libraries.

Configure:
```cmake
cmake -B build
```
This will generate the configuration on a new folder named "build".


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

## Links of interest

[Engine documentation](https://themesiah.github.io/MochiEngine/Docs/index.html)
[Architecture](./ARCHITECTURE.md)