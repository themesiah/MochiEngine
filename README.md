# SDLSpaceShooter

## Table of contents
1. [Introduction](#introduction)
2. [Building](#configuring-and-building)
3. [Basic usage](#basic-usage)
4. [Links](#links)

## Introduction

MochiEngine is a personal project of a 2D pixel art game engine based on [SDL3](https://github.com/libsdl-org/SDL) with the purpose of being used as a portfolio of general C++ knowledge and architecture skills.
This means that the engine has been purposedly prepare to work with 2D renderers and specifically pixel art. Any other usage of this engine has not been tested.

If you stumbled across this project and want to try it, be free to do so!

Next there are instructions to build the sample projects and documentation with which you can start building games.

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

## Basic usage

There are 4 minimal requirements for building a game with MochiEngine:

1. Download this project.
2. A CMake configuration file on a subfolder and including the subdirectory in the main CMakeLists.txt
3. A main.cpp file.
4. A [layer](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_layer.html) to inject in the engine.

You will also need CMake version 4.0+ and any C++ compiler.

Let's go step by step. We will create the base for a game called "Mega Space Robots".

### Download this project

Clone this repository somewhere using git.

```bash
git clone https://github.com/themesiah/MochiEngine.git
```

On the project folder, create a new folder called "MegaSpaceRobots".

### CMakeLists

On the newly created folder, create a text file called CMakeLists.txt and put this:

```cmake
cmake_minimum_required(VERSION 4.0)


# Set the project name and version
project(MegaSpaceRobots VERSION 0.1)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/MegaSpaceRobots/lib/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/MegaSpaceRobots/lib/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/MegaSpaceRobots/bin/$<CONFIG>)


# Add all subdirectories
file(GLOB MegaSpaceRobots_src CONFIGURE_DEPENDS "src/*.cpp")
add_library(MegaSpaceRobotsLib ${MegaSpaceRobots_src})
add_executable(MegaSpaceRobots src/main.cpp)
target_include_directories(MegaSpaceRobotsLib INTERFACE "src")
target_link_libraries(MegaSpaceRobotsLib PUBLIC Engine)
target_link_libraries(MegaSpaceRobots
PRIVATE
MegaSpaceRobotsLib
)
```

That's a minimal CMake file to generate our game files.
Now go to *CMakeLists.txt* on the **root** directory and add this at the end of the file:

```cmake
add_subdirectory("MegaSpaceRobots")
```

This will configure the full project to add our game as a target.

### Main file

In our *MegaSpaceRobots* folder, create a folder called *src*. Then inside create a file called *main.cpp* and put this:

```cpp
#include "Engine.h"
#include "GameLayer.h"

int main()
{
    const char *appName = "Mega Space Robots";
    const char *appVersion = "0.1";
    const char *appId = "com.mycompany.megaspacerobots";
    const char *windowName = "Mega Space Robots";
    Mochi::Engine engine(appName, appVersion, appId, windowName);
    MSR::GameLayer *gameLayer = new MSR::GameLayer();
    engine.PushLayer(gameLayer);

    engine.Run();
}
```

We don't have the game layer yet, but it will be what starts everything for our game!

Btw, we will work on the namespace **MSR** (Mega Space Robots).

### Layer file

In the *MegaSpaceRobots/src* directory create two files: **GameLayer.h** and **GameLayer.cpp**.

GameLayer.h
```cpp
#ifndef HDEF_GAMELAYER
#define HDEF_GAMELAYER

#include "Layer.h"

namespace MSR
{
    // Inherit from Layer, necessary to be injected in the engine.
    class GameLayer : public Mochi::Layer
    {
    public:
        GameLayer();
        virtual ~GameLayer();
        // Override the GUI method, as we will show a text.
        virtual void GUI() const override;
    };
}

#endif
```

This code declares that we will have a layer that will only override the GUI method for now.

GameLayer.cpp
```cpp
#include "GameLayer.h"
#include "GUI/AbstractGUI.h"
#include "GUI/GUICommon.hpp"

namespace MSR
{
    inline constexpr const char *CONST_HELLO_WORLD = "Hello\nMega Space Robots!";

    // Our constructor will do nothing for now.
    GameLayer::GameLayer() : Layer() {}
    GameLayer::~GameLayer() {}
    void GameLayer::GUI() const
    {
        // This will define how our text will be shown.
        const Mochi::Graphics::GUITextOptions titleTextOptions{
            .DstRect = {Mochi::Rectf({0.0f, 0.0f}, {})},
            .ScreenAnchor = Mochi::Graphics::GUI_MIDDLE_CENTER,
            .TextPivot = Mochi::Graphics::GUI_MIDDLE_CENTER,
            .TextSize = 62.0f};

        // Show the text
        mGUI->Text(CONST_HELLO_WORLD, titleTextOptions);
    }
}
```

### Run the game

If everything is correct, we will be able to configure and compile our game now. Run this:

```bash
cmake -B build
cmake --build build --target MegaSpaceRobots --config Debug
```

The first line create our intermediate project that will be build. It can take a few minutes the first time you build it because it has to download all dependencies to be able to work.

The second like should take a few minutes to build the first time too, as it has to compile the dependencies too.

After it finishes, you should find the executable in *build/MegaSpaceRobots/bin/Debug*. Execute it. You should see the text!

You can then close the window or do an alt+f4 to close the application.

Check [the documentation](https://themesiah.github.io/MochiEngine/Docs/index.html) for more info about what you can do in the engine.
You can also check how the samples are done to get ideas!

## Links

- [Documentation](https://themesiah.github.io/MochiEngine/Docs/index.html)
- [Architecture](./ARCHITECTURE.md)