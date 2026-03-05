# Architecture
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