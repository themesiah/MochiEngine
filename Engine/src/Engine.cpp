#include "Engine.h"

#include <iostream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <format>

#include "Packer/PackCatalog.h"

#include "Graphics/IRenderer.h"
#include "Graphics/SDL/SDLRenderer.h"
#include "Graphics/Camera.h"
#include "GUI/AbstractGUI.h"
#include "GUI/SDLGUI.h"

#include "Input/InputManager.h"
#include "Input/ActionManager.h"
#include "Input/SDLKeyboardProvider.h"
#include "Input/SDLMouseProvider.h"
#include "Input/SDLGamepadProvider.h"

#include "Audio/IAudioManager.h"
#include "Audio/FMODWrapper.h"

#include "Constants.h"
#include "Exception.hpp"
#include "Utils/Logger.h"
#include "Utils/Assert.h"

#include "Event/EventBus.h"
#include "Event/EngineEvents.h"
#include "Event/ISystemEventDispatcher.h"
#include "Event/SDLSystemEventDispatcher.h"

#include "Debug/IGizmos.h"

#include "ScriptingManager.h"
#include "Layer.h"

#ifdef DEBUG
#include "Debug/DebugLayer.h"
#endif

namespace Mochi
{

    static Engine *gEngine = nullptr;

    Engine &Engine::Get()
    {
        return *gEngine;
    }

    Engine::Engine(const char *appName, const char *appVersion, const char *appId, const char *windowName)
        : mTargetFPS(60), mLastDeltaTime(0.016f), mLastRealDelta(0.0f), mIsRunning(false), mLayers(), mPopLayerQueue(), mPushLayerQueue()
    {
        gEngine = this;
        try
        {
#ifdef DEBUG
            mCatalog = std::make_unique<FS::PackCatalog>(FS::PackCatalog::FileLoaderType::FileSystem);
#else
            mCatalog = std::make_unique<FS::PackCatalog>(FS::PackCatalog::FileLoaderType::Packfile);
#endif
            mCatalog->OpenPack("Data/Core");
            LOG_OK("Catalog Initialized");

            mScripting = std::make_unique<Scripting::ScriptingManager>(mCatalog.get());
            LOG_OK("LUA Initialized");

            mRenderer = std::make_unique<Graphics::SDLRenderer>(appName, appVersion, appId, windowName);
            mRenderQueue.clear();
            LOG_OK("SDL Initialized");

            mEventBus = std::make_unique<Event::EventBus>();
            LOG_OK("Event bus Initialized");

            mCamera = mRenderer->CreateCamera();
            LOG_OK("Camera Initialized");

            mAudio = std::make_unique<Audio::FMODWrapper>(mCatalog.get(), mScripting.get());
            mAudio->LoadAudio(CONST_MASTER_BANK);
            LOG_OK("FMOD Initialized");

            mActionManager = std::make_unique<Input::ActionManager>(new Input::InputManager(std::make_unique<Input::SDLKeyboardProvider>(),
                                                                                            std::make_unique<Input::SDLMouseProvider>(mRenderer.get()),
                                                                                            std::make_unique<Input::SDLGamepadProvider>(mEventBus.get())));
            auto actionsBuffer = mCatalog->GetFile(CONST_ACTIONS_FILE);
            bool success = mActionManager->LoadActions(actionsBuffer);
            LOG_OK("Action manager Initialized");

            mGUI = mRenderer->CreateGUI(mCatalog.get(), mActionManager.get());
            LOG_OK("GUI Initialized");

            mGizmos = mRenderer->CreateGizmos();
            LOG_OK("Gizmos Initialized");

            mEventDispatcher = std::make_unique<Event::SDLSystemEventDispatcher>(mEventBus.get());

            mAppQuitHandler = mEventBus->Subscribe<ApplicationQuitEvent>(
                [&](const ApplicationQuitEvent &e)
                { mIsRunning = false; });

            mFrameStart = std::chrono::steady_clock::now();

#ifdef DEBUG
            PushLayer(new DebugLayer());
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

    void Engine::PreciseDelay(std::chrono::nanoseconds ns) const
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto end = start + ns;

        // Sleep most of the time
        if (ns > std::chrono::milliseconds(2))
            std::this_thread::sleep_for(ns - std::chrono::milliseconds(2));

        // Busy-wait the remainder
        while (std::chrono::high_resolution_clock::now() < end)
        {
        }
    }

    void Engine::Run()
    {
        mIsRunning = true;
        try
        {
            while (1)
            {
                auto frameStart = std::chrono::steady_clock::now();
                if (!Update(mLastDeltaTime))
                {
                    break;
                }
                Render();
                const auto target = std::chrono::nanoseconds(1'000'000'000 / mTargetFPS);
                auto work = std::chrono::steady_clock::now() - mFrameStart;
                mLastRealDelta = std::chrono::duration<float>((std::chrono::steady_clock::now() - frameStart)).count();

                if (work < target)
                {
                    PreciseDelay(target - work);
                }

                auto frameEnd = std::chrono::steady_clock::now();
                auto frameDur = frameEnd - mFrameStart;
                mFrameStart = frameEnd;

                mLastDeltaTime = std::chrono::duration<float>(frameDur).count();
            }
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

    bool Engine::Update(const float &dt)
    {
        // Time
        Time::TimeSystem::GetInstance().Tick(mLastDeltaTime);

        // System Events
        mEventDispatcher->PollEvents();

        // Input
        mActionManager->Update(dt);

        // Audio
        mAudio->Update(dt);

        for (const std::unique_ptr<Layer> &layer : mLayers)
        {
            layer->Update(dt);
        }

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

        return mIsRunning;
    }

    void Engine::Render()
    {
        mRenderer->StartFrameRendering();
        ///////////////////////////////

        for (const std::unique_ptr<Layer> &layer : mLayers)
        {
            layer->Render();
        }
        mRenderer->Render(mRenderQueue, mCamera.get());
        mRenderQueue.clear();

        for (const std::unique_ptr<Layer> &layer : mLayers)
        {
            layer->GUI();
        }

#if DEBUG && !CTEST
        for (const std::unique_ptr<Layer> &layer : mLayers)
        {
            layer->Debug();
        }
#endif
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
        mEventBus->Unsubscribe<ApplicationQuitEvent>(mAppQuitHandler);
        gEngine = nullptr;
    }

    void Engine::SwapAudioManager(std::unique_ptr<Audio::IAudioManager> &&audioManager)
    {
        mEventBus->Publish<AudioManagerSwappedEvent>({audioManager.get()});
        mAudio = std::move(audioManager);
    }

    void Engine::SwapActionManager(std::unique_ptr<Input::IActionManager> &&actionManager)
    {
        mEventBus->Publish<ActionManagerSwappedEvent>({actionManager.get()});
        mActionManager = std::move(actionManager);
    }

    void Engine::SwapCamera(std::unique_ptr<Graphics::Camera> &&camera)
    {
        mEventBus->Publish<CameraSwappedEvent>({camera.get()});
        mCamera = std::move(camera);
    }

    void Engine::SwapRenderer(std::unique_ptr<Graphics::IRenderer> &&renderer)
    {
        mGUI = renderer->CreateGUI(mCatalog.get(), mActionManager.get());
        mGizmos = renderer->CreateGizmos();
        mEventBus->Publish<RendererSwappedEvent>({renderer.get(), mGUI.get(), mGizmos.get()});
        mRenderer = std::move(renderer);
    }
}