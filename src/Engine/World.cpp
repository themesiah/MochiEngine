#include "World.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <SDL3_image/SDL_image.h>
#include <filesystem>

#include "Constants.h"
#include "CoreConstants.h"
#include "Audio/FMODWrapper.h"
#include "Graphics/Sprite.h"
#include "Input/InputManager.h"
#include "Input/ActionManager.h"

#include "Logger.h"

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

    if (!TTF_Init())
    {
        SDL_Log("Couldn't init ttf: %s", SDL_GetError());
        throw SDL_APP_FAILURE;
    }

    mFont = TTF_OpenFont(std::format("{}/{}", CONST_DATA_FOLDER, CONST_MAIN_FONT_PATH).c_str(), CONST_DEVBUILD_TEXT_SIZE);
    if (!mFont)
    {
        SDL_Log("Couldn't load %s: %s", CONST_MAIN_FONT_PATH, SDL_GetError());
        throw SDL_APP_FAILURE;
    }
    mTextEngine = TTF_CreateRendererTextEngine(renderer);
    if (!mTextEngine)
    {
        SDL_Log("Couldn't create a ttf text engine: %s", SDL_GetError());
        throw SDL_APP_FAILURE;
    }
    mText = TTF_CreateText(mTextEngine, mFont, CONST_DEVBUILD_TEXT, 0);
    if (!mText)
    {
        SDL_Log("Couldn't create a ttf text: %s", SDL_GetError());
        throw SDL_APP_FAILURE;
    }

    LOG_OK("SDL Initialized");

    mSampleSprite = new Sprite(renderer, std::format("{}/{}", CONST_DATA_FOLDER, CONST_TEST_IMAGE));

    mFmod = std::make_shared<FMODWrapper>();
    if (mFmod->Init() == FMOD_OK && mFmod->LoadBank(CONST_MASTER_BANK) == FMOD_OK)
    {
        LOG_OK("FMOD Initialized");
    }

    mActionManager = std::make_shared<ActionManager>(std::make_shared<InputManager>());
    bool success = mActionManager->LoadActions(std::format("{}/{}", CONST_DATA_FOLDER, CONST_ACTIONS_FILE));
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

    int tw, th;
    TTF_GetTextSize(mText, &tw, &th);
    TTF_DrawRendererText(mText, w - tw - 1, h - th);
#endif
    ///////////////////////////////
    SDL_RenderPresent(renderer); /* put it all on the screen! */
}

World::~World()
{
    delete mSampleSprite;
    if (mFont)
    {
        TTF_CloseFont(mFont);
    }
    if (mTextEngine)
    {
        TTF_DestroyRendererTextEngine(mTextEngine);
    }
    if (mText)
    {
        TTF_DestroyText(mText);
    }
}