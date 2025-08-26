#include "World.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <filesystem>

#include "Constants.h"
#include "Audio/FMODWrapper.h"
#include "Graphics/Sprite.h"
#include "Input/InputManager.h"
#include "Input/ActionManager.h"

World::World()
{
    SDL_SetAppMetadata(CONST_APP_NAME, CONST_APP_VERSION, CONST_APP_ID);

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        throw SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(CONST_WINDOW_NAME, 640, 480, 0, &window, &renderer))
    {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        throw SDL_APP_FAILURE;
    }

    if (!SDL_SetRenderVSync(renderer, 1))
    {
        SDL_Log("Couldn't set vsyinc to true: %s", SDL_GetError());
        throw SDL_APP_FAILURE;
    }

    mSampleSprite = new Sprite(renderer, CONST_TEST_IMAGE);

    mFmod = std::make_shared<FMODWrapper>();
    mFmod->Init();
    mFmod->LoadBank(CONST_MASTER_BANK);

    mActionManager = std::make_shared<ActionManager>(std::make_shared<InputManager>());
    bool success = mActionManager->LoadActions(CONST_ACTIONS_FILE);
    if (!success)
    {
        std::cout << "Can't open Actions.json" << std::endl;
    }
}

void World::Update(const float &dt)
{
    const bool *keyboardState = SDL_GetKeyboardState(NULL);
    mActionManager->Update(dt, keyboardState);
    mFmod->Update();

    if (mActionManager->Performed("Debug1"))
    {
        std::cout << "B is pressed!" << std::endl;
        mFmod->PlayBGM("TestMusic");
    }

    if (mActionManager->Performed("Debug2"))
    {
        std::cout << "C is pressed!" << std::endl;
        mFmod->PauseBGM();
    }

    if (mActionManager->Performed("Debug3"))
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