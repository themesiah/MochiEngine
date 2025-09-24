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
#include "Graphics/Sprite.h"
#include "Graphics/AnimatedSprite.h"
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

namespace Mochi
{

    Engine::Engine(const char *appName, const char *appVersion, const char *appId, const char *windowName) : mTargetFPS(60), mLastDeltaTime(0.016f), mLastRealDelta(0.0f)
    {
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

            mRenderer = std::make_shared<Graphics::Renderer>(appName, appVersion, appId, windowName);
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

            mFmod = std::make_shared<Audio::FMODWrapper>(mCatalog, mScripting);
            mFmod->LoadBank(CONST_MASTER_BANK);
            LOG_OK("FMOD Initialized");

            auto inputManager = std::make_shared<Input::InputManager>(std::make_shared<Input::SDLKeyboardProvider>(),
                                                                      std::make_shared<Input::SDLMouseProvider>(mRenderer),
                                                                      std::make_shared<Input::SDLGamepadProvider>(mEventBus));
            mActionManager = std::make_shared<Input::ActionManager>(inputManager);
            auto actionsBuffer = mCatalog->GetFile(CONST_ACTIONS_FILE);
            bool success = mActionManager->LoadActions(actionsBuffer);
            LOG_OK("Action manager Initialized");

            mGUI = std::make_shared<Graphics::GUI>(mCatalog, mRenderer, mActionManager);
            LOG_OK("GUI Initialized");

            mFrameStart = std::chrono::steady_clock::now();
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

            Render();

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

#ifdef DEBUG
        // Dev build message
        SDL_RendererLogicalPresentation *rlp{NULL};
        int logicalW, logicalH;
        SDL_GetRenderLogicalPresentation(mRenderer->GetRenderer().get(), &logicalW, &logicalH, rlp);

        SDL_SetRenderScale(mRenderer->GetRenderer().get(), 1, 1);
        mGUI->Text(CONST_DEVBUILD_TEXT, 8, {0, (float)logicalH - 8}, {255, 255, 255, SDL_ALPHA_OPAQUE});
        mGUI->Text(std::format("{} fps", (int)(1.0f / mLastRealDelta)).c_str(), 8, {0, 0}, {255, 255, 255, SDL_ALPHA_OPAQUE});
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

    Engine::~Engine()
    {
    }
}