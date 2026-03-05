# Space Shooter Sample

## Intro

The first sample I made with *MochiEngine* is a Space Shooter because I love the genre! This makes also the first game (even if small) that I did almost completely (and I mean including art and music!). I learned pixel art and music production just to do projects like this.

However, this is little more than a sample of a game done in Mochi Engine, and is not a demonstration of clean code or great architecture: is just a show of the engine capabilities. So there is a lot of ugly code there, and what you can see from this is that the API designed for the engine works, and works well. Now let's talk about some specifics.

## FMOD

Surely the most different thing that you will see in the samples. The Space Shooter sample uses FMOD just to have events trigger on the song timeline. Yep, just to do that. I created the song and then designed the level according with the music, and that was possible thanks to FMOD Studio.

Thanks to the scripting system too, I could trigger LUA events when a timeline marker reached a point and be able to capture it on the game LUA script to trigger enemies appearing or shots being shot.

## Scripting

Great part of the game logic is written as scripting. Enemies appearing, when are shots made, direction and routes of the enemies, and even the starting cutscene and background clouds are made with scripts. Everything not performance sensible was scripted.

Check out [the core LUA script](./GameData/ShooterCore.lua) and [the first level script](./GameData/Level1Setup.lua). Those are proof of how powerful the LUA api is. All game iterations were made changing the script and starting the game again, using the binded method Audio_SkipToPosition of the [LUA API](https://themesiah.github.io/MochiEngine/Docs/md__docs_data_2_lua_bindings.html) to start from certain points of the song.

About the bullets: Enemy bullets are started on lua, but managed in C++. This was important, because I wanted to be flexible with how many bullets and where will appear, but as a performance sensible element, their lifetime should be managed in C++, as we will see in the next section.

## Performance considerations

A few sprites, simple animations and a 9-sliced image as the background don't need a lot of performance considerations and would run on a toaster. However, I knew that bullets were another story, as they can become lots and have to check for collisions each frame.

Being fair, the game is simple enough so that even doing this wrong, it would've not be a performance problem, but knowing already what to do to make it better it felt wrong not do it right from the start.

Keeping it simple, bullets follow a data oriented architecture, specifically a SOA model (Struct of Arrays) with arrays for bullet positions, directions, speeds... and a fixed size pool to avoid the creation and destruction of bullets during runtime.

Check out [AbstractBulletPool.cpp](./src/Bullets/AbstractBulletPool.cpp) to see what I mean. I loop through the max amount of bullets a pool has to perform each operation, and to create them I have to find an inactive bullet, set it to active and change its position on the position vector. This made easy the bullets API (which consist on a bullet creation method for LUA), checking for collisions and rendering them.