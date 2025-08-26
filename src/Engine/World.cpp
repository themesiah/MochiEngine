#include "World.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <filesystem>

#include "Audio/FMODWrapper.h"
#include "Graphics/Sprite.h"

World::World()
{
    _timesUpdated = 0;

    SDL_SetAppMetadata("Space Shooter", "0.1", "com.magicmochi.spaceshooter");

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        throw SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Space Shooter", 640, 480, SDL_WINDOW_ALWAYS_ON_TOP, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        throw SDL_APP_FAILURE;
    }

    if (!SDL_SetRenderVSync(renderer, 1))
    {
        SDL_Log("Couldn't set vsyinc to true: %s", SDL_GetError());
        throw SDL_APP_FAILURE;
    }

    mSampleSprite = new Sprite(renderer, "Data\\Coproducers.jpg");

    mFmod = std::make_unique<FMODWrapper>();
    mFmod->Init();
    mFmod->LoadBank("Master");
}

void World::Update(const float &dt)
{
    const bool *keyboardState = SDL_GetKeyboardState(NULL);

    _timesUpdated++;

    mFmod->Update();

    if (keyboardState[SDL_SCANCODE_B] == true)
    {
        std::cout << "B is pressed!" << std::endl;
        mFmod->PlayBGM("TestMusic");
    }

    if (keyboardState[SDL_SCANCODE_C] == true)
    {
        std::cout << "C is pressed!" << std::endl;
        mFmod->PauseBGM();
    }

    if (keyboardState[SDL_SCANCODE_D] == true)
    {
        std::cout << "D is pressed!" << std::endl;
        mFmod->ResumeBGM();
    }
}

void World::AppEvent(SDL_Event *event)
{
}

void World::Render() const
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    ///////////////////////////////

    mSampleSprite->Render(renderer);

    // Dev build message
    int w = 0;
    int h = 0;

#ifdef DEBUG
    SDL_GetRenderOutputSize(renderer, &w, &h);
    SDL_SetRenderScale(renderer, 1, 1);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    if (!SDL_RenderDebugText(renderer, 5, h - 10, "DEVELOPMENT BUILD"))
    {
        std::cout << "Can't render text? Error is: " << SDL_GetError() << std::endl;
    }
#endif
    ///////////////////////////////
    SDL_RenderPresent(renderer); /* put it all on the screen! */
}

World::~World()
{
    delete mSampleSprite;
}