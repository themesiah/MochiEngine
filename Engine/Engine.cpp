#include "Engine.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <filesystem>

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

#include "Entity/IEntity.h"
#include "Entity/IRenderable.h"
#include "Entity/IUpdateable.h"
#include "Entity/IAnimatable.h"

Engine::Engine() : mTargetFPS(60), mNsPerFrame(1000000000 / mTargetFPS), mLastDeltaTime(0.016f), mLastEntityHandler(1)
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
}

bool Engine::Update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            return 0;
        }
    }
    auto start = std::chrono::high_resolution_clock::now();
    const bool *keyboardState = SDL_GetKeyboardState(NULL);
    mActionManager->Update(mLastDeltaTime, keyboardState);
    mFmod->Update();

    for (auto updatable : mUpdateables)
    {
        updatable->Update(mLastDeltaTime);
    }
    for (auto animatable : mAnimatables)
    {
        animatable->UpdateAnimation(mLastDeltaTime);
    }

    // USER DEFINED
    if (!OnUpdate(mLastDeltaTime))
    {
        return 0;
    }

    Render();

    auto end = std::chrono::high_resolution_clock::now();
    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    if (ns < mNsPerFrame)
    {
        int64_t sleepTime = mNsPerFrame - ns;
        SDL_DelayNS(sleepTime);
    }
    else
    {
        mLastDeltaTime = ns / 1000000000;
    }

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