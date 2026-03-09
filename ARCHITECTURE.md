# Architecture

## Table of contents
1- [Intro](#intro)
2- [About interfaces](#interface-or-not-to-interface)
3- [Engine lifecycle](#engine-lifecycle)
4- [The renderer](#the-renderer)
5- [Layer System](#layer-system)
6- [Action Manager](#action-manager)
7- [Scripting](#scripting)
8- [The Catalog](#the-catalog)

## Intro

This document explains the **whats** and **whys** of the architecture decisions for **MochiEngine**.

First of all, note that this is a **portfolio** project. This project is meant to show my qualities as an engineer, and I didn't focus (or I actually neglected) several
features or processes that didn't help that purpose.

The most important thing to note is that this is not a general purpose engine. **MochiEngine** is meant to be a 2D Pixel Art engine. Most of the non mandatory features it has are there because one or more of the created samples required them. Others are there because I feel they improve the creation and iteration pipelines.

What I actually wanted **MochiEngine** to do:

- Be simple.
- Be fast.
- Be testable.
- Be extendible.
- Allow to be used in several ways, with the same results.

At the time by the first sample (Space Shooter) was finished, I think all of those points are already true. The next samples are just proving that the engine can be used for a variety of games.

## Interface or not to interface

One of the things I considered when creating this engine was what to make an interface and what not. Which modules could be interchanged? What made sense to switch on runtime?

The engine started with few interfaces: just have the job done as fast as i could. After I had a minimal renderer, input, files and audio systems working on a well manageable codebase, I started focused on testing those modules.

As some modules used submodules or other type of classes (like how the textures are created with a texture factory, or how the audio system read files from the filesystem) I started to notice patterns and making an interface only for what i needed: if I wanted to test how a module reacted to an action, I needed a dummy action manager that did what the test needed. Or later, when I wanted to test full loops on the engine, i didn't need the renderer to show anything, i just needed to check the game state.

That way, I made interfaces of what needed to be *faked*, and left other systems as simple as i could. As a side benefit, now those interfaces allow to extend the engine easily!

### Interfaces VS Abstract classes

Might be a little obvious, but I should explain why some abstractions are interfaces and some abstract classes, so it's not interpreted as some inconsistency.

When an abstraction has some common functionality between implementations (like how the GUI manages the selected button or how the factories store textures in its own cache) I make them abstract classes. When not, I make them as interfaces.

There can be a discussion about if all of them should be interfaces, and external classes should exist to do whatever common ground have those implementations (for the given examples, a GUISelectionSystem class and a TextureCache class), and then the interface implementations use those classes. That would've work, maybe even in a cleaner way, but I decided in favor of abstract classes to avoid adding more project structure complexity.

## Engine lifecycle

*MochiEngine* has the following lifetime steps: Creation, Setup, Run, Destroy. I also provide a shortcut Creation constructor that also fully setups the engine. Let's see each one:

### Creation and setup

When we create the engine with the constructor, two things could happen: You use the default constructor or you use the shortcut constructor.

The default constructor setups a minimal set of the Subsystems, the ones that cannot be overriden, and expects you to call **engine.Setup** with the following subsystems, initialized by you: IRenderer, IActionManager, IAudioManager. This allows, besides being able to customize the engine with your own implementations, to setup tests with dummy or virtual subsistems. To use the shortcut constrcutor yo call this:

```cpp
Engine(const char *appName, const char *appVersion, const char *appId, const char *windowName)
```

And the *IRenderer* subsystem will be setup as **SDLRenderer**, the *IActionManager* will be **ActionManager** and *IAudioManager* will be either **FMOD** or **SDL_Mixer**.

### Run

*MochiEngine* provides a single method, called **Run** that starts the game process. Inside this method there is a **while loop** that keeps working until the engine receives a close event (alt+f4 or pressing the cross in the window) or the game logic sends that signal or any update on a layer returns a false. Between each frame there is a wait until it's time for the next frame, after all processes for this frame are done.

During this step of the lifetime, Update is called for each layer, besides updating all subsystems automatically.

### Destruction

When the while loop ends be it by getting false from a layer update or because of the application close event, the main method ends (usually, depends on implementation) and then the engine is destroyed, destroying all of its contents (subsystems, layers, and the contents of those).

## The renderer

One of the most difficult systems to abstract was the renderer.

As an SDL renderer that functioned with [SDL_RenderTexture](https://wiki.libsdl.org/SDL3/SDL_RenderTexture), the base of the rendering was bound to be simple: Initialize the renderer, start a frame, do SDL_RenderTexture, end the frame and destroy resources at the end of the application lifetime. Some features made it a little more complex, but it was still nothing compared with a full fledged manual renderer.

However, there was one requirement that made me realize how many things were dependent on the renderer, and I needed some way to make an abstraction in a way for all of them to work.

The solution was to abstract each one of those things that depended on the renderer: Textures, Texture Factories, GUI (as it shows sprites), Gizmos (as it uses primitives) and of course, the very Renderer. Besides that, SDL uses SDL_FPoint and SDL_FRect as types for rendering and managing textures, and I needed an abstraction for that too.

### Point and Rect

SDL own Point and Rect implementations (simple structs with data) motivated me to make [my own types](https://themesiah.github.io/MochiEngine/Docs/group___types.html), also compatible with the SDL ones. That way I had types that i could use everywhere in the engine and an easy way to use them on SDL methods. That's why [Vector2f](https://themesiah.github.io/MochiEngine/Docs/struct_mochi_1_1_vector2f.html) and [Rectf](https://themesiah.github.io/MochiEngine/Docs/struct_mochi_1_1_rectf.html) have copy and conversion methods with their SDL counterparts. The rest of the engine uses my own types, leaving SDL types only to call SDL functions.

### IRenderer

The [IRenderer](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_graphics_1_1_i_renderer.html) abstraction was the most simple. Just StartFrameRendering, Render, and FinishRendering methods were enough to start and support most renderers. The Render function accepts a list of [Render Commands](https://themesiah.github.io/MochiEngine/Docs/struct_mochi_1_1_graphics_1_1_render_command.html), which should be enough to show most 2D graphics with any render api. Later, I would notice the need for two more interface methods, which I explain next.

### AbstractTextureFactory

[AbstractTextureFactory](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_graphics_1_1_abstract_texture_factory.html) implements an [Abstract Factory Pattern](https://refactoring.guru/design-patterns/abstract-factory). Why this pattern? I need that, for each type of renderer i want to support, have a factory that loads and returns textures in a specific way. Only SDL loads textures with the SDL library, and SDL only accepts SDL_Texture as a parameter for SDL_RenderTexture. As the logic for loading textures is different, a templated factory was not enough, and an Abstract Texture Factory was needed for the purpose. That way, IRenderer defined *CreateTextureFactory*, which returned an *AbstractTextureFactory*.

### The texture

If the *AbstractTextureFactory* created a specific type of texture, I had to abstract the texture too. *AbstractTextureFactory* returned an [ITexture](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_graphics_1_1_i_texture.html), which is pretty simple: An interface that allows to get the size of the texture and setting the alpha value of the texture (for effects). Each implementation, like [SDLTexture](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_graphics_1_1_s_d_l_texture.html) should be responsible of actually storing the texture in a specific format. As the [Render Commands](https://themesiah.github.io/MochiEngine/Docs/struct_mochi_1_1_graphics_1_1_render_command.html) have an [ITexture](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_graphics_1_1_i_texture.html), each renderer would then have to cast *ITexture* to their own type.

### GUI

Same logic as with [AbstractTextureFactory](#abstracttexturefactory), each GUI has its own logic to render their textures and texts. That's how [AbstractGUI](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_graphics_1_1_abstract_g_u_i.html) appears in scene. The interface and implementation are just samples on how to do an immediate GUI, and by any means they are final products. A real GUI system should be more robust and have more methods that are useful for complex GUIs, like input fields, sliders, toggles...

However, this GUI does the trick, shows the capabilities of the system and participate in the complexity of the engine, making it worth to explain.

### Gizmos

Same logic as with [AbstractTextureFactory](#abstracttexturefactory), the gizmos systems have their own logic, dependent on the renderer (because they mostly use primitives like drawing lines, points or circles). [IGizmos](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_debug_1_1_i_gizmos.html) is the abstraction for that.

## Layer System

The engine features a Layer System that allows to separate logic and/or contents between engine and game or between different domains of the game.

A layer is nothing more than a class that, after being injected in the engine, gets initialized and then updated each frame for each of these methods: *Update*, *Render*, *GUI* and in debug mode, *Debug*.

Layers allow, first and most important, to inject your own code into the application using *MochiEngine*. You create your own layer, construct the engine object, and then push the layer into the engine. When running the engine, the code in your own layer will be executed on each update too.

An example of this is shown in [the Space Shooter sample main file](./Samples/SpaceShooter/src/main.cpp), as follows:

```cpp
const char *appName = "Space Shooter";
const char *appVersion = "0.1";
const char *appId = "com.scarletmochi.spaceshooter";
const char *windowName = "Space Shooter";
Mochi::Engine engine(appName, appVersion, appId, windowName);
Mochi::Shooter::GameLayer *gameLayer = new Mochi::Shooter::GameLayer();
engine.PushLayer(gameLayer);
engine.Run();
```

As layers can be popped and pushed in runtime, this is great too to use them as "scenes". Implement some loading and unloading, push a loading screen layer, pop your level layer, push the new level layer, and when the new level is loaded, pop the loading screen. Or something like that. Its versatile enough to allow this or just to separate your game logic from your application logic from your UI logic, if needed.

## Action manager

It is difficult to know exactly which features something called "Action Manager" offers, as the range of options is very big. And the more a system like this grows in features, the more it grows in architectural complexity. That's why i chose to talk about the [IActionManager](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_input_1_1_i_action_manager.html) and its main implementation, [ActionManager](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_input_1_1_action_manager.html).

First of all, which were the needs to decide on a complex architecture like the one i'm going to describe?

- Has to support at least keyboard, mouse and gamepad

- Within those devices, there are different types of inputs: buttons, axis and single sign axis.

- An input is not an action, but an action can be achieved by several inputs.

- The player should be able to poll at least two kind of values: booleans and floats.

- Different kind of inputs could have special requirements (deadzone for sticks for example)

Those are not a lot of features, and there could be a lot more of features: more devices, a lot of special requirements like adding acceleration and gravity to inputs, support for mobile... But for the purposes of this engine, those are a good middle point. Now, how did I achieve all those features?

### Input layers

The name "Input Layer" is not seen anywhere in the engine, but conceptually it is architected like a series of layers that give information to their "manager" and finally, all data converges in the [ActionManager](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_input_1_1_action_manager.html), allowing the player to poll that data with the expected outputs.

At the bottommost layer we have the **InputProviders** ([IKeyboardProvider](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_input_1_1_i_keyboard_provider.html), [IMouseProvider](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_input_1_1_i_mouse_provider.html) and [IGamepadProvider](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_input_1_1_i_gamepad_provider.html)).

The implementations of those 3 classes poll data from their devices when asked to.

Next we have the [InputManager](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_input_1_1_input_manager.html), class that polls the information from **InputProviders** each frame, stores it, and keeps the last frame history too (to check when keys have been pressed and released).

Next we have [IPerformableActions](https://themesiah.github.io/MochiEngine/Docs/struct_mochi_1_1_input_1_1_i_performable_action.html), that defines **ONE** way to trigger an actual action. One way to trigger an action means pressing a key this frame, releasing a gamepad button or moving the mouse, for example. The implementations should use the [InputManager](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_input_1_1_input_manager.html) to retrieve the data and return if an action is performed and/or which value does it have. A example implementation is the [PerformableActionGamepad](https://themesiah.github.io/MochiEngine/Docs/struct_mochi_1_1_input_1_1_performable_action_gamepad.html), which checks if a gamepad button is pressed. Each implementation has to apply the special needs of the device (thresholds, deadzones...) on its own way.

And last, we have the [ActionManager](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_input_1_1_action_manager.html). The *ActionManager* has a map of [Actions](https://themesiah.github.io/MochiEngine/Docs/struct_mochi_1_1_input_1_1_action.html), each of which are collections of [IPerformableActions](https://themesiah.github.io/MochiEngine/Docs/struct_mochi_1_1_input_1_1_i_performable_action.html).

This way, when the user requests the state of a specific action, the *ActionManager* gets the *Action* from the map and loops through all *IPerformableActions*, checking the values for each one. Those *IPerformableActions* are checked against the *InputManager*, updated each frame with new data from the *InputProviders*, modified by any special need of the device. If any *IPerformableAction* is true or have a value different than 0, the *ActionManager* tells the user that value.

With all of that, we can use different input providers if our way to retrieve data changes (instead of SDL we use OpenGL, for example). We can also modify easily any performable action to add more functionality (like being considered pressed only when 0.3 seconds passed). An action can have inifinity of ways to be triggered too. Even if it is not implemented right now, having a set of actions instead of plain inputs would make it easier to remap controls.

As you can see, even simple concepts like reacting to input turns out very complex if the features are meant to be as extensible as possible. Even if the engine could have done well with just reading input, this kind of system is one I think is mandatory to have it well implented and that always pays off in the end.

## Scripting

*MochiEngine* scripting system works with LUA scripts, and it's binded to C++ with [sol2](https://github.com/ThePhD/sol2).

There are lots of things to say about the scripting, but I will start with the whys.

### Why scripting

Scripting provides faster iteration and hot reload when needed. This makes a lot easier to test new things or, if the optimization is not an issue, making full games
with an scripting system. The tools *MochiEngine* provides for scripting allow to make games entirely on scripting.

### Why LUA

Why not? Is a scripting language like any other. There is no big architectural, design or even personal reason to having implemented the scripting system using LUA instead of others, like Python or C#.

### Why not letting the user choose?

That's right. [ScriptingManager](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_scripting_1_1_scripting_manager.html) could have implemented an *IScriptingManager* interface and allow to chose the scripting backend and remove the direct dependency with the LUA state in some parts of the code, but it was not worth. All the samples work with LUA just fine and there was no real reason to switch scripting backends.

### Why sol2

Again, no big reason. [sol2](https://github.com/ThePhD/sol2) is a good library and it's been more updated than other lua binding libraries out there.

---

Now, let's see the features and how they work.

### What can the scripting system do?

The scripting system does 3 things: It exposes sol2 functionality (to bind classes and functions and share ownership of objects), it executes code and it keeps track of two environments, the global and a local one.

The executed code can be executed with plain code obtained in the C++ context (be it hardcoded, downloaded somehwere...) or from a file in the catalog. When loading the code from a file it just gets the plain string in the file and executes it.

And yes, there could be a map of environments and select where to execute each piece of code, so LUA tables are personalized for each engine layer, for example, to allow certain things and making the purpose of the code more clear. But again, it was good enough. In fact, those two environments allow us to put common bindings and functions on the global lua tables (that will live for all the lifetime of the application) and level specific data on the local environment. When a level is unloaded, the environment can just be reseted.

### What can be done in a script?

You can check the [LUA API](https://themesiah.github.io/MochiEngine/Docs/md__docs_data_2_lua_bindings.html) page in the documentation or check the .lua scripts in the samples.The things in the API should be enough for most games, and whatever is not there can be done on a per-game basis. You can load any amount of scripts and load them globally, even in your own game layer or even the main file. That allows you to define some utility functions that your game will need.

LUA is incredibly powerful and flexible. You could write an "Update" function on a lua script and call it from C++ like this:

```cpp
mScriptingManager->State["Update"](deltaTime);
```

And then update all your LUA owned entities on that update in the script, to minimize context changes.

### Coroutines

One interesting thing in the scripting manager is the feature of coroutines.

Using the [coroutines library](https://www.lua.org/pil/9.html) from LUA, I defined a simple yet powerful API inside an script that gets loaded with the engine to create corotuines (and tweens!). To make it even easier, the ScriptingManager already updates the Coroutines each frame, allowing you to use them with just writing in a script!

## The catalog

One thing that could come as unnecessary is [the pack catalog](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_f_s_1_1_pack_catalog.html) and the [packer](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_f_s_1_1_pack_file.html).

### The packer

The packer puts all files from a directory recursively on a single file. The packer has its own CMake project, and it builds when the engine is built. The binary file features two simple functions:

```bash
Packer.exe -pack "some/data/folder" "some/path/Name.pak"
Packer.exe -unpack "some/path/Name.pak" "some/output/folder"
```

Those two pack and unpack respectively a .pak file (or whatever extension you chose).

To do that, a header for the file is created and the **relative path** for each file is stored, alongside the current **head** of the binary blob and the **size** of the file.
Having for each file its path (the id), where they start and their size, accesing them is trivial, and we don't even have to load the full file: just keep a file descriptor pointing to the file. 

### Loading files

We have two ways of loading files into the engine, using the [IFileLoader](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_f_s_1_1_i_file_loader.html) interface: [PackFile](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_f_s_1_1_pack_file.html) and [SystemFileLoader](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_f_s_1_1_system_file_loader.html). The first one gets files from a .pak as seen before. The second one reads plain files from the OS.

Right now, the engine is configured to use PackFile on release builds, and SystemFileLoader on debug builds. CMake can be configured to auto-build the .pak files when doing a release build, as seen in [the CMake configuration file of the SpaceShooter sample](./Samples/SpaceShooter/CMakeLists.txt).

### The catalog

Now that we know what methods we have to load files, let's talk about the catalog. The catalog is just a collection of [IFileLoaders](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_f_s_1_1_i_file_loader.html) that work together. You open a pack (an *IFileLoader*) and then you can access any file on the pack, as you are getting something with the relative path of the pack.

Check [this page](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_f_s_1_1_pack_catalog.html#details) for a more thorough explanation of the file system hierarchy and overrides.

### Benefits of the packing

There are three main benefits to this system.

First, obfuscation. As a developer, I wouldn't want users to access my raw assets that easily. Sure, it can be reverse enginereed easily, but that's just a minimal of the userbase.

Second, easy management. If you have a game with several packs with game data, you can easily know where is each asset, which one to load for a new level... and is also easier to do updates for the game, as downloading a single file is a lot simpler and straightforward than downloading a full folder. And there is a last trick that allows better updates...

Third and most interesting, using the same relative path for a file in several packs priorizes the one on the last pack loaded, as seen in [the documentation page](https://themesiah.github.io/MochiEngine/Docs/class_mochi_1_1_f_s_1_1_pack_catalog.html#details). This allows several things:

- Modding. It would be easy to make a game that loads all .pak in the game folder, in a "Mods" folder for example, and allow users to create their own mods.

- Updating. Instead of changing a small asset in a 300mb .pak file and making users download a 300mb update, create a new one that will be loaded later with the same relative path and make users download 1kb of update. Of course, there is a tradeof, that is duplicated data. But is worth considering anyway.

- Setup. Each game has their own requirements, and some engine default assets might not do for the end user. The greatest example is the [Actions file](./Engine/CoreData/Actions.json), that has a default version on the engine, both as a base and as an example, and can be easily overriden by the game by putting an *Actions.json* file in the root of their main pack. 