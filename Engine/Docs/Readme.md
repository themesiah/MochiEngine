This document covers the minimal knowledge required to create a game in Mochi Engine. For further knowledge, read the documents for each subsystem.

## 1. Engine class
Create a class inheriting from [[Engine|Engine]] from _Engine.h_.
For the document purposes, let's call it _MyEngine_. At least, you should implement the _OnUpdate_ method, which is called each frame. It returns true, unless you have custom logic for closing the game (like with a keyboard shortcut).

## 2. Main file
Create a main cpp file with a main function that instantiates your class and update it on a loop, like this:
~~~cpp
int main()
{
    const char *appName = "My Game";
    const char *appVersion = "0.1";
    const char *appId = "com.mycompany.mygame";
    const char *windowName = "My Game";
    MyEngine engine(appName, appVersion, appId, windowName);

    while (engine.Update())
    {
    }
}
~~~

## 3. Setup your filesystem
Mochi Engine uses a custom filesystem for loading files.
For the purposes of the document, we will work on a folder called _MyData_ on the working directory. You can do this on _MyEngine_.
~~~cpp
mCatalog->OpenPack("MyData");
~~~

## 4. Load some textures
A _Sprite_ is the minimal unit of renderable entities in the engine.
You can create one with this in _MyEngine_ to load a sprite located on "MyData/Path/to/your/" and called "sprite.png".
~~~cpp
auto sprite = std::make_shared<Mochi::Graphics::Sprite>(mTextureFactory, "Path/to/your/sprite.png");
AddEntity(sprite);
~~~

## 5. Play some audio
Mochi Engine uses FMOD for audio playback. You require at least an Audio Bank and an Audio Bank.string files. Let's call them _MyBank.bank_ and _MyBank.strings.bank_. It should look like this in _MyEngine_ to load a bank located in _MyData/Audio_.
~~~cpp
mFmod->LoadBank("Audio/MyBank");
mFmod->PlayBGM("Audio/MusicEvent");
~~~

## 6. Configure some actions
Actions are abstractions that manage inputs. Actions are defined in a json file called _Actions.json_ on the root of the _Data_ folder. Warning! NOT in your _MyData_ folder, but in the _Data_ folder. You can load your own _Actions.json_ later.
_Actions.json_ looks like this:
~~~json
{
	"Actions":[
		{"Name":"Shot", "Cases":[
			{"Type":"Keyboard", "Trigger":1, "Keycode":29, "Value":1},
			{"Type":"Mouse", "Trigger":1, "Button":0, "Value":1},
			{"Type":"Gamepad", "Trigger":1, "Button":0, "Value":1}
		]},
		{"Name":"Horizontal", "Cases":[
			{"Type":"Keyboard", "Trigger":1, "Keycode":79, "Value":1},
			{"Type":"Keyboard", "Trigger":1, "Keycode":80, "Value":-1},
			{"Type":"GamepadAxis", "Axis":0}
		]},
		{"Name":"Vertical", "Cases":[
			{"Type":"Keyboard", "Trigger":1, "Keycode":82, "Value":1},
			{"Type":"Keyboard", "Trigger":1, "Keycode":81, "Value":-1},
			{"Type":"GamepadAxis", "Axis":1}
		]}
	]
~~~
This allows you to press the keyboard arrows to update the horizontal and vertical axis, and press Z to "Shot". You can also press A in your gamepad and use the left stick.

Then, on your _MyEngine_ you can do this:
~~~cpp
if (mActionManager->Performed("Shot"))
{
	// Do something
}
auto horizontal = mActionManager->Value("Horizontal");
auto vertical = mActionManager->Value("Vertical");

// Do something with horizontal and vertical
~~~
## 7. Add some update logic
In your Update implementation, add some custom logic. Use your [[Readme#6. Configure some actions|actions]] to execute some code and voila!

## 8. Cmake
This project is prepared to work with CMake, and the Mochi Engine folder should live in your project folder. Let's say the folder is named _Engine_. Then, a minimal CMake file would look like this:
~~~cmake
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(MyProject VERSION 0.1)


# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib/$<CONFIG>)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/$<CONFIG>)

# Add all subdirectories
add_subdirectory("Engine")

add_executable(Game ${GAME_WIN32_FLAG} main.cpp MyEngine.cpp)

target_link_libraries(Game
                      PRIVATE
                      Engine
)
~~~

## 9. Build

First of all, go to _Engine/Includes/SDL3_image/external_ and run _download.sh_ or _Get-GitModules.ps1_.
Then go to _Engine/Includes/SDL3_ttf/external_ and again run _download.sh_ or _Get-GitModules.ps1_.
Then, return to your root directory and on the console, do:

~~~bash
cmake -B build
cmake --build build
~~~

If all is correct, on your _{sourceDir}/build/bin/Debug_ directory should appear a bunch of DLL files and your _Game.exe_. Copy your data folder there, and let's play!