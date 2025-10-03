#include "Engine.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <filesystem>
#include <chrono>
#include <format>

#include "Packer/PackCatalog.h"

#include "Graphics/AnimationFactory.h"
#include "Graphics/TextureFactory.h"
#include "Graphics/Renderer.h"
#include "Graphics/Camera.h"
#include "GUI/GUI.h"

#include "Input/InputManager.h"
#include "Input/ActionManager.h"
#include "Input/SDLKeyboardProvider.h"
#include "Input/SDLMouseProvider.h"
#include "Input/SDLGamepadProvider.h"

#include "Audio/FMODWrapper.h"

#include "Constants.h"
#include "Exception.hpp"
#include "Utils/Logger.h"
#include "Utils/Assert.h"

#include "Event/EventBus.h"

#include "ScriptingManager.h"
#include "Layer.h"

#ifdef DEBUG
#include "Debug/DebugLayer.h"
#endif

namespace Mochi
{

    static std::unique_ptr<Engine> gEngine = nullptr;

    Engine &Engine::Get()
    {
        return *gEngine.get();
    }

    Engine::Engine(const char *appName, const char *appVersion, const char *appId, const char *windowName)
        : mTargetFPS(60), mLastDeltaTime(0.016f), mLastRealDelta(0.0f), mLayers(), mPopLayerQueue(), mPushLayerQueue()
    {
        gEngine.reset(this);
        try
        {
#ifdef DEBUG
            mCatalog = std::make_shared<FS::PackCatalog>(FS::PackCatalog::FileLoaderType::FileSystem);
#else
            mCatalog = std::make_shared<FS::PackCatalog>(FS::PackCatalog::FileLoaderType::Packfile);
#endif
            mCatalog->OpenPack("Data/Core");
            LOG_OK("Catalog Initialized");

            mScripting = std::make_shared<Scripting::ScriptingManager>(mCatalog);
            LOG_OK("LUA Initialized");

            mRenderer = std::make_unique<Graphics::Renderer>(appName, appVersion, appId, windowName);
            mRenderQueue.clear();
            LOG_OK("SDL Initialized");

            mEventBus = std::make_shared<Event::EventBus>();
            LOG_OK("Event bus Initialized");

            mCamera = mRenderer->CreateCamera();
            LOG_OK("Camera Initialized");

            mTextureFactory = std::make_shared<Graphics::TextureFactory>(mCatalog, mRenderer->GetRenderer());
            LOG_OK("Main texture factory Initialized");

            mAnimationFactory = std::make_shared<Graphics::AnimationFactory>(mCatalog);
            LOG_OK("Main animation factory Initialized");

            mFmod = std::make_unique<Audio::FMODWrapper>(mCatalog, mScripting);
            mFmod->LoadBank(CONST_MASTER_BANK);
            LOG_OK("FMOD Initialized");

            mActionManager = std::make_unique<Input::ActionManager>(new Input::InputManager(std::make_unique<Input::SDLKeyboardProvider>(),
                                                                                            std::make_unique<Input::SDLMouseProvider>(mRenderer.get()),
                                                                                            std::make_unique<Input::SDLGamepadProvider>(mEventBus)));
            auto actionsBuffer = mCatalog->GetFile(CONST_ACTIONS_FILE);
            bool success = mActionManager->LoadActions(actionsBuffer);
            LOG_OK("Action manager Initialized");

            mGUI = std::make_shared<Graphics::GUI>(mCatalog, mRenderer.get(), mActionManager.get());
            LOG_OK("GUI Initialized");

            mFrameStart = std::chrono::steady_clock::now();

#ifdef DEBUG
            PushLayer(new DebugLayer(mRenderer.get(), mGUI.get()));
#endif
        }
        catch (const SystemInitializationError &e)
        {
            LOG_ERROR(e.what());
            exit(-1);
        }
        catch (const EngineError &e)
        {
            LOG_PANIC(e.what());
            exit(-1);
        }
    }

    bool Engine::Update()
    {
        try
        {
            auto frameStart = std::chrono::steady_clock::now();
            // Time
            Time::TimeSystem::GetInstance().Tick(mLastDeltaTime);

            // SDL Events
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_EVENT_QUIT)
                {
                    return 0;
                }
                mEventBus->Publish<SDL_Event>(event);
            }

            // Input
            mActionManager->Update(Time::TimeSystem::GetDeltaTime());

            // Audio
            mFmod->Update();

            // USER DEFINED
            if (!OnUpdate(Time::TimeSystem::GetDeltaTime()))
            {
                return 0;
            }
            float dt = Time::TimeSystem::GetDeltaTime();
            for (const std::unique_ptr<Layer> &layer : mLayers)
            {
                layer->Update(dt);
            }

            Render();

            for (const auto &outLayer : mPopLayerQueue)
            {
                mLayers.erase(
                    std::remove_if(mLayers.begin(), mLayers.end(),
                                   [&](const std::unique_ptr<Layer> &layer)
                                   { return layer.get() == outLayer; }),
                    mLayers.end());
            }
            mPopLayerQueue.clear();

            for (const auto &inLayer : mPushLayerQueue)
            {
                mLayers.push_back(std::unique_ptr<Layer>(inLayer));
            }
            mPushLayerQueue.clear();

            const auto target = std::chrono::nanoseconds(1'000'000'000 / mTargetFPS);
            auto work = std::chrono::steady_clock::now() - mFrameStart;
            mLastRealDelta = std::chrono::duration<float>((std::chrono::steady_clock::now() - frameStart)).count();

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
        catch (const EngineError &e)
        {
            LOG_PANIC(e.what());
            exit(-1);
        }
        catch (const std::exception &e)
        {
            LOG_PANIC(e.what());
            exit(-1);
        }
    }

    void Engine::Render()
    {
        mRenderer->StartFrameRendering();
        ///////////////////////////////

        mRenderer->Render(mRenderQueue, mCamera);
        mRenderQueue.clear();

        OnRender();

        for (const std::unique_ptr<Layer> &layer : mLayers)
        {
            layer->Render();
        }
        ///////////////////////////////
        mRenderer->FinishRendering(); /* put it all on the screen! */
    }

    void Engine::AddRenderCommand(const Graphics::RenderCommand &command)
    {
        mRenderQueue.push_back(command);
    }

    void Engine::AddRenderCommands(const std::vector<Graphics::RenderCommand> &commands)
    {
        mRenderQueue.insert(mRenderQueue.end(), commands.begin(), commands.end());
    }

    void Engine::PushLayer(Layer *inLayer)
    {
        mPushLayerQueue.push_back(inLayer);
    }

    void Engine::PopLayer(Layer *outLayer)
    {
        mPopLayerQueue.push_back(outLayer);
    }

    Engine::~Engine()
    {
    }
}