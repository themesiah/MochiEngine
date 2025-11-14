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
#include "Input/DummyInputProviders.h"

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
#include "Debug/Profiler.hpp"

#include "Scripting/ScriptingManager.h"
#include "Layer.h"
#include "Scripting/ScriptingLayer.h"

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

    Engine::Engine()
        : mTargetFPS(60),
          mLastDeltaTime(1.0f / mTargetFPS),
          mLastRealDelta(0.0f),
          mIsRunning(false),
          mIsPaused(false),
          mUnpausedTimeScale(1.0f),
          mLayers(),
          mPopLayerQueue(),
          mPushLayerQueue()
    {
        gEngine = this;
        MinimalSetup();
    }

    Engine::Engine(const char *appName, const char *appVersion, const char *appId, const char *windowName)
        : mTargetFPS(60), mLastDeltaTime(0.016f), mLastRealDelta(0.0f), mIsRunning(false), mIsPaused(false), mUnpausedTimeScale(1.0f), mLayers(), mPopLayerQueue(), mPushLayerQueue()
    {
        gEngine = this;
        try
        {
            MinimalSetup();

            mRenderer = std::make_unique<Graphics::SDLRenderer>(appName, appVersion, appId, windowName);
            mRenderQueue.clear();
            LOG_OK("Renderer Initialized");

            mAudio = std::make_unique<Audio::FMODWrapper>(mCatalog.get(), mScripting.get());
            LOG_OK("FMOD Initialized");

            mActionManager = std::make_unique<Input::ActionManager>(std::make_unique<Input::InputManager>(std::make_unique<Input::SDLKeyboardProvider>(),
                                                                                                          std::make_unique<Input::SDLMouseProvider>(mRenderer.get()),
                                                                                                          std::make_unique<Input::SDLGamepadProvider>(mEventBus.get())));

            auto actionsBuffer = mCatalog->GetFile(CONST_ACTIONS_FILE);
            bool success = mActionManager->LoadActions(actionsBuffer);
            LOG_OK("Action manager Initialized");

            mGUI = mRenderer->CreateGUI(mCatalog.get(), mActionManager.get());
            LOG_OK("GUI Initialized");

            PushLayer(new Scripting::ScriptingLayer());
#ifdef DEBUG
            mGizmos = mRenderer->CreateGizmos();
            LOG_OK("Gizmos Initialized");
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

    void Engine::MinimalSetup()
    {
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

            mEventBus = std::make_unique<Event::EventBus>();
            LOG_OK("Event bus Initialized");

            mCamera = std::make_unique<Graphics::Camera>(Vector2f(0.0f, 0.0f), 1.0f, Vector2f(static_cast<float>(CONST_RENDER_LOGICAL_X), static_cast<float>(CONST_RENDER_LOGICAL_Y)));
            LOG_OK("Camera Initialized");

            mEventDispatcher = std::make_unique<Event::SDLSystemEventDispatcher>(mEventBus.get());
            LOG_OK("Event dispatcher Initialized");

            mProfiler = std::make_unique<Mochi::Debug::FrameProfiler>();
            LOG_OK("Profiler initialized");

            mAppQuitHandler = mEventBus->Subscribe<ApplicationQuitEvent>(
                [&](const ApplicationQuitEvent &e)
                { mIsRunning = false; });
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

    void Engine::Setup(std::unique_ptr<Graphics::IRenderer> &&renderer,
                       std::unique_ptr<Input::IActionManager> &&actionManager,
                       std::unique_ptr<Audio::IAudioManager> &&audioManager)
    {
        mRenderer = std::move(renderer);
        mActionManager = std::move(actionManager);
        mAudio = std::move(audioManager);
        mGUI = mRenderer->CreateGUI(mCatalog.get(), mActionManager.get());

        PushLayer(new Scripting::ScriptingLayer());

#ifdef DEBUG
        mGizmos = mRenderer->CreateGizmos();
        PushLayer(new DebugLayer());
#endif
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
        mFrameStart = std::chrono::steady_clock::now();
        try
        {
            while (mIsRunning)
            {

                // Time
                Time::TimeSystem::GetInstance().Tick(mLastDeltaTime);

                Update(Time::TimeSystem::GetInstance().GetDeltaTime()); // Get delta time from TimeSystem to use time scale
                Render();
                const auto target = std::chrono::nanoseconds(1'000'000'000 / mTargetFPS);
                auto work = std::chrono::steady_clock::now() - mFrameStart;
                mLastRealDelta = std::chrono::duration<float>((std::chrono::steady_clock::now() - mFrameStart)).count();

                if (work < target)
                {
                    // PreciseDelay(target - work);
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
        if (dt > 0.03f)
        {
            LOG_WARNING(std::format("Dangerous delta time of {}!", dt));
            auto report = mProfiler->Report();
            LOG_WARNING(report);
        }
        mProfiler->NewFrame();

        // System Events
        mProfiler->BeginSection("PollEvents");
        mEventDispatcher->PollEvents();
        mProfiler->EndSection("PollEvents");

        // Input
        mProfiler->BeginSection("ActionManager");
        mActionManager->Update(dt);
        mProfiler->EndSection("ActionManager");

        // Audio
        mProfiler->BeginSection("AudioManager");
        mAudio->Update(dt);
        mProfiler->EndSection("AudioManager");

        // Scripting coroutines
        mProfiler->BeginSection("Scripting");
        mScripting->Update(dt);
        mProfiler->EndSection("Scripting");

        // UI
        mProfiler->BeginSection("GUIUpdate");
        mGUI->Update(dt);
        mProfiler->EndSection("GUIUpdate");

        int layerIndex = 0;
        for (const std::unique_ptr<Layer> &layer : mLayers)
        {
            mProfiler->BeginSection(std::format("Layer{}", layerIndex));
            if (!layer->Update(dt))
                mIsRunning = false;
            mProfiler->EndSection(std::format("Layer{}", layerIndex));
            layerIndex++;
        }

        mProfiler->BeginSection("Layer removal");
        for (const auto &outLayer : mPopLayerQueue)
        {
            mLayers.erase(
                std::remove_if(mLayers.begin(), mLayers.end(),
                               [&](const std::unique_ptr<Layer> &layer)
                               { return layer.get() == outLayer; }),
                mLayers.end());
        }
        mPopLayerQueue.clear();
        mProfiler->EndSection("Layer removal");

        mProfiler->BeginSection("Layer push");
        for (const auto &inLayer : mPushLayerQueue)
        {
            inLayer->InitLayer();
            mLayers.push_back(std::unique_ptr<Layer>(inLayer));
        }
        mPushLayerQueue.clear();
        mProfiler->BeginSection("Layer push");

        return mIsRunning;
    }

    void Engine::Render()
    {
        mProfiler->BeginSection("Rendering");
        mProfiler->BeginSection("Start rendering");
        mRenderer->StartFrameRendering();
        mProfiler->EndSection("Start rendering");
        ///////////////////////////////

        mProfiler->BeginSection("Layers render");
        for (const std::unique_ptr<Layer> &layer : mLayers)
        {
            layer->Render();
        }
        mProfiler->EndSection("Layers render");
        mProfiler->BeginSection("Render commands");
        mRenderer->Render(mRenderQueue, mCamera.get());
        mRenderQueue.clear();
        mProfiler->EndSection("Render commands");

        mProfiler->BeginSection("Layers gui");
        for (const std::unique_ptr<Layer> &layer : mLayers)
        {
            layer->GUI();
        }
        mProfiler->EndSection("Layers gui");

#if DEBUG && !CTEST
        mProfiler->BeginSection("Layers debug");
        for (const std::unique_ptr<Layer> &layer : mLayers)
        {
            layer->Debug();
        }
        mProfiler->EndSection("Layers debug");
#endif
        ///////////////////////////////
        mProfiler->BeginSection("Finish rendering");
        mRenderer->FinishRendering(); /* put it all on the screen! */
        mProfiler->EndSection("Finish rendering");
        mProfiler->EndSection("Rendering");
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
        mLayers.clear(); // Clear layers first, so layers resources are freed BEFORE the renderer is destroyed, avoiding weird leaks
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
        mEventBus->Publish<RendererSwappedEvent>({renderer.get(), mGUI.get(), mGizmos.get()});
        mRenderer = std::move(renderer);
#if DEBUG
        mGizmos = renderer->CreateGizmos();
#endif
    }

    void Engine::Pause()
    {
        if (mIsPaused)
        {
            LOG_WARNING("Trying to pause the engine: it was already paused");
            return;
        }
        mIsPaused = true;
        mUnpausedTimeScale = Time::TimeSystem::GetTimeScale();
        Time::TimeSystem::SetTimeScale(0.0f);
        mAudio->PauseBGM();
    }

    void Engine::Resume()
    {
        if (!mIsPaused)
        {
            LOG_WARNING("Trying to resume the engine: it was not paused");
            return;
        }
        mIsPaused = false;
        Time::TimeSystem::SetTimeScale(mUnpausedTimeScale);
        mAudio->ResumeBGM();
    }
}