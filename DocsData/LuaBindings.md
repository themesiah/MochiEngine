# LUA API

This sections show a list with the classes and methods available to be used from a LUA script, being some of them
C++ bindings and other of them defined directly in LUA and loaded at the engine setup.

## Included libraries

MochiEngine already includes several lua libraries:

- [Coroutines](https://www.lua.org/pil/9.html)
- [Tables](https://www.lua.org/pil/19.html)
- [Debug](https://www.lua.org/pil/23.html)
- [Math](https://www.lua.org/pil/18.html)
- [String](https://www.lua.org/pil/20.html)

OS and I/O libraries have not been included purposedly. The engine is meant to allow easy modding, and adding those libraries could bring
security issues with untrusted modders.

## Non-binded methods

### Events

These LUA events are an implementation of an event buss pattern with a single channel. You are able to create and retrieve events, add listeners
to them and fire them with an arbitrary set of parameters.

```lua
GetEvent("EventName")
```

Retrieves (and creates if it doesn't exist) an event.


```lua
GetEvent("EventName"):addListener(function(param1, param2)
...
end)
```

Adds a listener to the event. Put any amount of params needed. The "Fire" method will be have to be called with those parameters too if you
want to retrieve the correct data.


```lua
GetEvent("EventName"):fire(1, "param2string")
```

Fires an event, triggering all listeners with the sent data.

### Math

```lua
Lerp(a, b, t)
```

Example:
```lua
Lerp(0, 100, 0.5) --> 50
```

```lua
InverseLerp(a, b, value)
```

Example:
```lua
InverseLerp(0, 100, 50) --> 0.5
```

NOTE: MochiEngine automatically includes the [lua math library](https://www.lua.org/pil/18.html), so we have some other methods there.
However, the math bindings could be improved with other methods like min and max. TODO.

### Coroutines

WE have the [coroutines library](https://www.lua.org/pil/9.html) already included as part of the MochiEngine setup, but we have several utility functions
that allow us to work easily with game development concepts, like tweens.

```lua
UpdateCoroutines(dt)
```

UpdateCoroutines is automatically called from \ref Mochi::Scripting::ScriptingManager. It updates this frame for all active coroutines and deactivate Coroutines
as needed.

```lua
StartCoroutine(func)
```

Example:
```lua
StartCoroutine(function()
	-- do something...
	Yield()
	-- do another thing next frame...
end)
```

Starts and stores a function as a coroutine. This function will be called each frame, stopping on Yield(), Wait(s) and WaitFor(predicate) until it finishes.

```lua
Yield()
```

Inside a coroutine, it waits until the next frame.

```lua
Wait(s)
```

Waits a defined amount of seconds with an internal timer and doing Yield() each frame until it gets to the specified time.

```lua
WaitFor(predicate)
```
Example:
```lua
WaitFor(myValue is 5)
```

Waits until a predicate returns true. Until then, it returns Yield() each frame.

```lua
Tween(update_callback, end_callback, time)
```
update_callback --> function(t, dt)
end_callback --> function()
time --> seconds

Tween is a function that creates a coroutine, updates each frame calling update_callback with the current t value (between 0 and 1) and the frame
delta time and calling end_callback when it finishes, after the specified time (update_callback is called too at the last frame with t = 1).
Example:

```lua
Tween(function(t, dt)
	local posx = Lerp(0, 20, t)
	someSprite:GetTransform().Position.x = posx
end, function()
	DeleteSprite(someSprite)
end,
5)
```

Considering that we have a reference to a sprite called "someSprite", this example moves the sprite from 0 to 20 in the x axis in an interval of 5 seconds,
and then deletes it.

## Binded methods

There is a list of binded methods that are not part of a class. That means that those methods can be called without creating instances of any class.

### Logs

```lua
LogOk(message)
LogInfo(message)
LogWarning(message)
LogError(message)
```

Different kind of logs to debug or get info from inside lua, making use of the engine logs.

### Factory methods

These methods allow to create and destroy different kind of graphics that will be owned by \ref Mochi::Scripting::ScriptingLayer.

```lua
CreateSprite(resourcePath)
```
resourcePath --> String. Path to the sprite in the catalog.

returns: \ref Mochi::Graphics::SpriteBase


```lua
CreateSpritesheet(resourcePath, startingFrame)
```
resourcePath --> String. Path to the animation data in the catalog.

startingFrame --> int. Frame index.

returns: \ref Mochi::Graphics::Spritesheet


```lua
CreateAnimatedSprite(resourcePath, mainAnimation)
```
resourcePath --> String. Path to the animation data in the catalog.

mainAnimation --> int. Animation index.

returns: \ref Mochi::Graphics::AnimatedSprite


```lua
CreateOneshotAnimation(resourcePath, mainAnimation)
```
resourcePath --> String. Path to the animation data in the catalog.

mainAnimation --> int. Animation index.

returns: \ref Mochi::Graphics::OneshotAnimation


```lua
DeleteSprite(deletableElement)
```
deletableElement --> \ref Mochi::Graphics::SpriteBase or child class.

### Input

Same as the documented methods in \ref Mochi::Input::IActionManager.

```lua
Action_Performed(actionName)
```

Returns --> bool

```lua
Action_Value(actionName)
```

Returns --> float

```lua
Action_CompoundValue(actionName1, actionName2)
```

Returns --> \ref Mochi::Vector2f

### Time

```lua
GetDeltaTime()
```

returns --> float with delta time

```lua
GetUnscaledDeltaTime()
```

returns --> float with delta time as it was unscaled

```lua
GetGameTime()
```

returns --> float. Scaled time span since application started until now

```lua
GetUnscaledGameTime()
```

returns --> float. Real time span since application started until now

```lua
GetOSTime()
```

returns --> int. Operative system time as a timestamp since epoch. "Re-implemented" because we don't integrate the OS lua library.


### Audio

Same as documented in \ref Mochi::Audio::IAudioManager or its current implementation.


```lua
Audio_LoadAudio(path)
Audio_UnloadAudio(path)
Audio_PlayBGM(audioName)
Audio_PauseBGM()
Audio_StopBGM()
Audio_ResumeBGM()
Audio_SkipToPosition(ms)
Audio_PlayOneshot(audioName)
Audio_SetParameter(parameterName, value)
Audio_SetMixerVolume(mixerName, value)
```

## Classes

Some of the bindings are classes with its own state and binded methods.

### Vector2f
```lua
local vec = Vector2f:new(3, 5)
vec.y = 0
LogInfo(vec:Distance())
LogInfo(vec:Normalized())
LogInfo(vec:Dot(otherVec))
LogInfo(vec:CosineProximity(otherVec))
LogInfo(vec + otherVec)
LogInfo(vec / 5)
```

### Rectf

```lua
local rect = Rectf:new(3, 5, 10, 10)
rect.x = 5
rect.w = 12
rect.h = 8
rect:SetPosition(2, 0)
LogInfo(rect:GetPosition())
rect:SetSize(8, 8)
LogInfo(rect:GetSize())
rect:Scale(2)
LogInfo(rect:IsTextureValid())
LogInfo(rect is otherRect)
```

### Transform

```lua
local transform = Transform:new()
transform.Position.x = 5
transform.Position.y = 0
transform.Scale = 2
transform:Move(Vector2f:new(3, 0))
```

### Graphics

Note that all methods available to a type of sprite is available too for each sprite type inheriting from it and its parents.

All methods accept the same parameters and return the same types as in the original classes.

\ref Mochi::Graphics::SpriteBase
```lua
local sprite = CreateSprite("path/to/sprite.png")
sprite:GetTransform().Move(Vector2f:new(3, 0))
sprite:SetTransform(otherTransform)
LogInfo(sprite:GetZIndex())
sprite:SetZIndex(5)
sprite:SetSrcRect(Rectf:new(0,0,32,32))
```

\ref Mochi::Graphics::Spritesheet
```lua
local spritesheet = CreateSpritesheet("path/to/animation.json", 0)
spritesheet:SetFrame(1)
LogInfo(spritesheet:GetFrame())
```

\ref Mochi::Graphics::AnimatedSprite
```lua
local animatedSprite = CreateAnimatedSprite("path/to/animation.json", "idle")
animatedSprite:PlayAnimation("attack")
animatedSprite:StopAnimation()
LogInfo(animatedSprite:GetCurrentAnimation())
```

\ref Mochi::Graphics::OneshotAnimation
```lua
local oneshot = CreateOneshotAnimation("path/to/animation.json", "explosion")
oneshot:SetFinishCallback(function()
	LogInfo("Explosion finished")
end)
```

\ref Mochi::Graphics::Camera
```lua
-- Camera is a reference in the lua global table, and is called "Camera"
Camera:SetZoom(5)
LogInfo(Camera:GetZoom())
Camera:SetPosition(Vector2f:new(0,0))
LogInfo(Camera:GetPosition())
LogInfo(Camera:IsOnScreen(Rectf:new(3,5,10,8))
```