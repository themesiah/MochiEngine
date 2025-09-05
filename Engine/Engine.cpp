#include "Engine.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <filesystem>
#include <chrono>

#include "Packer/PackCatalog.h"

#include "Graphics/AnimationFactory.h"
#include "Graphics/TextureFactory.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "Graphics/Sprite.h"
#include "Graphics/AnimatedSprite.h"

#include "Input/InputManager.h"
#include "Input/ActionManager.h"

#include "Audio/FMODWrapper.h"

#include "Constants.h"
#include "Logger.h"
#include "Assert.h"

#include "Entity/IEntity.h"
#include "Entity/IRenderable.h"
#include "Entity/IUpdateable.h"
#include "Entity/IAnimatable.h"

Engine::Engine() : mTargetFPS(60), mLastDeltaTime(0.016f), mLastEntityHandler(1)
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

    SDL_FPoint point;
    point.x = 0;
    point.y = 0;
    mCamera = std::make_shared<Camera>(point, 1);

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

    mFrameStart = std::chrono::steady_clock::now();
}

bool Engine::Update()
{
    TimeSystem::GetInstance().Tick(mLastDeltaTime);
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            return 0;
        }
    }
    const bool *keyboardState = SDL_GetKeyboardState(NULL);
    mActionManager->Update(TimeSystem::GetDeltaTime(), keyboardState);
    mFmod->Update();

    for (auto updatable : mUpdateables)
    {
        updatable->Update(TimeSystem::GetDeltaTime(), mActionManager);
    }
    for (auto animatable : mAnimatables)
    {
        animatable->UpdateAnimation(TimeSystem::GetDeltaTime());
    }

    // USER DEFINED
    if (!OnUpdate(TimeSystem::GetDeltaTime()))
    {
        return 0;
    }

    Render();

    const auto target = std::chrono::nanoseconds(1'000'000'000 / mTargetFPS);
    auto work = std::chrono::steady_clock::now() - mFrameStart;

    if (work < target)
    {
        SDL_DelayNS((target - work).count());
    }

    auto frameEnd = std::chrono::steady_clock::now();
    auto frameDur = frameEnd - mFrameStart;
    mFrameStart = frameEnd;

    mLastDeltaTime = std::chrono::duration<float>(frameDur).count();

    ASSERT(std::format("Delta time should not be less than the target, as we waited for it: Last {} target {}", mLastDeltaTime, 1.f / mTargetFPS), mLastDeltaTime >= 1.f / mTargetFPS);
    if (mLastDeltaTime > 0.25f)
        mLastDeltaTime = 0.25f; // Cap at 250ms

    return true;
}

void Engine::Render() const
{
    mRenderer->StartFrameRendering();
    ///////////////////////////////

    std::vector<RenderCommand> renderQueue;
    for (auto renderable : mRenderables)
    {
        renderQueue.push_back(renderable->GetRenderData());
    }

    mRenderer->Render(renderQueue, mCamera);

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

EntityHandler Engine::AddEntity(std::shared_ptr<IEntity> e)
{
    if (std::shared_ptr<IUpdateable> updateable = dynamic_pointer_cast<IUpdateable>(e))
    {
        mUpdateables.push_back(updateable);
    }
    if (std::shared_ptr<IRenderable> renderable = dynamic_pointer_cast<IRenderable>(e))
    {
        mRenderables.push_back(renderable);
    }
    if (std::shared_ptr<IAnimatable> animatable = dynamic_pointer_cast<IAnimatable>(e))
    {
        mAnimatables.push_back(animatable);
    }

    EntityHandler handler;
    if (mFreeEntityHandlers.size() > 0)
    {
        handler = mFreeEntityHandlers.back();
        mFreeEntityHandlers.pop_back();
    }
    else
    {
        handler = mLastEntityHandler;
        mLastEntityHandler++;
    }

    mEntities[handler] = e;

    return handler;
}

bool Engine::RemoveEntity(EntityHandler entityHandler)
{
    auto it = mEntities.find(entityHandler);
    if (it == mEntities.end())
        return false;
    auto e = mEntities[entityHandler];
    mEntities.erase(it);

    if (std::shared_ptr<IUpdateable> updateable = dynamic_pointer_cast<IUpdateable>(e))
    {
        mUpdateables.erase(std::remove(mUpdateables.begin(), mUpdateables.end(), updateable), mUpdateables.end());
    }
    if (std::shared_ptr<IRenderable> renderable = dynamic_pointer_cast<IRenderable>(e))
    {
        mRenderables.erase(std::remove(mRenderables.begin(), mRenderables.end(), renderable), mRenderables.end());
    }
    if (std::shared_ptr<IAnimatable> animatable = dynamic_pointer_cast<IAnimatable>(e))
    {
        mAnimatables.erase(std::remove(mAnimatables.begin(), mAnimatables.end(), animatable), mAnimatables.end());
    }
    return true;
}

Engine::~Engine()
{
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