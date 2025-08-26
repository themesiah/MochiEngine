#define SDL_MAIN_USE_CALLBACKS 1

#include <chrono>
#include <iostream>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "Engine/World.h"

/* We will use this renderer to draw into this window every frame. */
constexpr int screenFps{60};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    int i;

    try
    {
        *appstate = new World();
        World &state = *static_cast<World *>(*appstate);
    }
    catch (SDL_AppResult errorCode)
    {
        return errorCode;
    }

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    World &state = *static_cast<World *>(appstate);
    state.AppEvent(event);
    if (event->type == SDL_EVENT_QUIT)
    {
        return SDL_APP_SUCCESS; /* end the program, reporting success to the OS. */
    }
    else if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_ESCAPE)
    {
        return SDL_APP_SUCCESS;
    }
    else if (event->type == SDL_EVENT_KEY_DOWN && event->key.key == SDLK_F1)
    {
        SDL_SetWindowSize(state.window, 800, 600);
    }
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate)
{
    auto start = std::chrono::high_resolution_clock::now();

    // Update the world
    World &state = *static_cast<World *>(appstate);
    state.Update(0.016f);
    state.Render();

    // Sleep if we are ahead of FPS
    auto end = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    constexpr int64_t nsPerFrame = 1000000000 / screenFps;
    if (ns < nsPerFrame)
    {
        int64_t sleepTime = nsPerFrame - ns;
        SDL_DelayNS(sleepTime);
    }

    return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    /* SDL will clean up the window/renderer for us. */
}
