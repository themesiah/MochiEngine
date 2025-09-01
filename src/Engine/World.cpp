#include "World.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <filesystem>

#include "Packer/PackCatalog.h"

#include "Graphics/AnimationFactory.h"
#include "Graphics/TextureFactory.h"
#include "Graphics/Renderer.h"
#include "Graphics/Sprite.h"
#include "Graphics/AnimatedSprite.h"

#include "Input/InputManager.h"
#include "Input/ActionManager.h"

#include "Audio/FMODWrapper.h"

#include "Constants.h"
#include "Logger.h"

World::World()
{
#ifdef DEBUG
    mCatalog = std::make_shared<PackCatalog>(PackCatalog::FileLoaderType::FileSystem);
#else
    mCatalog = std::make_shared<PackCatalog>(PackCatalog::FileLoaderType::Packfile);
#endif
    mCatalog->OpenPack("Data");

    mRenderer = std::make_shared<Renderer>();
    if (!mRenderer->Init(CONST_APP_NAME, CONST_APP_VERSION, CONST_APP_ID, CONST_WINDOW_NAME))
    {
        LOG_PANIC("Can't initialize renderer, panic!");
        throw SDL_APP_FAILURE;
    }

    if (!TTF_Init())
    {
        SDL_Log("Couldn't init ttf: %s", SDL_GetError());
        throw SDL_APP_FAILURE;
    }

    mTextureFactory = std::make_shared<TextureFactory>(mCatalog, mRenderer->GetRenderer());
    mAnimationFactory = std::make_shared<AnimationFactory>(mCatalog);

    auto fontBuffer = mCatalog->GetFile(CONST_MAIN_FONT_PATH);
    mFont = TTF_OpenFontIO(SDL_IOFromConstMem(fontBuffer.data(), fontBuffer.size()), true, CONST_DEVBUILD_TEXT_SIZE);
    if (!mFont)
    {
        SDL_Log("Couldn't load %s: %s", CONST_MAIN_FONT_PATH, SDL_GetError());
        throw SDL_APP_FAILURE;
    }
    mTextEngine = TTF_CreateRendererTextEngine(mRenderer->GetRenderer().get());
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

    mSampleSprite = new Sprite(mTextureFactory, "Sprites/Background2.png");

    mAnimatedSprite = new AnimatedSprite(mAnimationFactory, mTextureFactory, "Sprites/Snake.json", "Idle");

    mFmod = std::make_shared<FMODWrapper>(mCatalog);
    if (mFmod->Init() == FMOD_OK && mFmod->LoadBank(CONST_MASTER_BANK) == FMOD_OK)
    {
        LOG_OK("FMOD Initialized");
    }

    mActionManager = std::make_shared<ActionManager>(std::make_shared<InputManager>());

    auto actionsBuffer = mCatalog->GetFile(CONST_ACTIONS_FILE);
    bool success = mActionManager->LoadActions(actionsBuffer);
    if (!success)
    {
        std::cout << "Can't open Actions.json" << std::endl;
    }
}

SDL_AppResult World::Update(const float &dt)
{
    const bool *keyboardState = SDL_GetKeyboardState(NULL);
    mActionManager->Update(dt, keyboardState);
    mFmod->Update();

    if (mActionManager->Performed("Debug1"))
    {
        std::cout << "B is pressed!" << std::endl;
        // mFmod->PlayBGM("TestMusic");
        mAnimatedSprite->PlayAnimation("Walk");
    }

    if (mActionManager->Performed("Debug2"))
    {
        std::cout << "C is pressed!" << std::endl;
        // mFmod->PauseBGM();
        mAnimatedSprite->PlayAnimation("Run");
    }

    if (mActionManager->Performed("Debug3"))
    {
        std::cout << "D is pressed!" << std::endl;
        // mFmod->ResumeBGM();
        mAnimatedSprite->PlayAnimation("Attack");
    }

    mAnimatedSprite->UpdateAnimation(dt);
    return SDL_APP_CONTINUE;
}

SDL_AppResult World::AppEvent(SDL_Event *event)
{
    return SDL_APP_CONTINUE;
}

void World::Render() const
{
    mRenderer->StartFrameRendering();
    ///////////////////////////////

    std::vector<RenderCommand> renderQueue;
    renderQueue.push_back(mSampleSprite->GetRenderData());
    renderQueue.push_back(mAnimatedSprite->GetRenderData());

    mRenderer->Render(renderQueue);

#ifdef DEBUG
    // Dev build message
    int w = 0;
    int h = 0;
    SDL_GetRenderOutputSize(mRenderer->GetRenderer().get(), &w, &h);
    SDL_SetRenderScale(mRenderer->GetRenderer().get(), 1, 1);
    SDL_SetRenderDrawColor(mRenderer->GetRenderer().get(), 255, 255, 255, SDL_ALPHA_OPAQUE);

    int tw, th;
    TTF_GetTextSize(mText, &tw, &th);
    TTF_DrawRendererText(mText, w - tw - 1, h - th);
#endif
    ///////////////////////////////
    mRenderer->FinishRendering(); /* put it all on the screen! */
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