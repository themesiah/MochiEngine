#ifndef HDEF_ENGINE
#define HDEF_ENGINE

#include <memory>
#include <vector>
#include <unordered_map>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL.h>
#include <chrono>
#include <sol/sol.hpp>

#include "Time/TimeSystem.h"

struct TTF_Font;
struct TTF_TextEngine;
struct TTF_Text;

namespace Mochi
{
    namespace Graphics
    {
        class Sprite;
        class AnimatedSprite;
        class TextureFactory;
        class Camera;
        class Renderer;
        class AnimationFactory;
        class GUI;
        struct RenderCommand;
    }
    namespace Audio
    {
        class FMODWrapper;
    }
    namespace Input
    {
        class ActionManager;
    }
    namespace FS
    {
        class PackCatalog;
    }
    namespace Event
    {
        class EventBus;
    }
    namespace Scripting
    {
        class ScriptingManager;
    }
    class Engine
    {
    private:
        int mTargetFPS;
        std::chrono::steady_clock::time_point mFrameStart;
        float mLastDeltaTime;
        float mLastRealDelta;

        void Render();
        std::vector<Graphics::RenderCommand> mRenderQueue;

    protected:
        std::shared_ptr<Graphics::Renderer> mRenderer;
        std::shared_ptr<Audio::FMODWrapper> mFmod;
        std::shared_ptr<Input::ActionManager> mActionManager;
        std::shared_ptr<FS::PackCatalog> mCatalog;
        std::shared_ptr<Graphics::TextureFactory> mTextureFactory;
        std::shared_ptr<Graphics::AnimationFactory> mAnimationFactory;
        std::shared_ptr<Graphics::Camera> mCamera;
        std::shared_ptr<Graphics::GUI> mGUI;
        std::shared_ptr<Event::EventBus> mEventBus;
        std::shared_ptr<Scripting::ScriptingManager> mScripting;
        virtual bool OnUpdate(const float &dt) = 0;

        void AddRenderCommand(const Graphics::RenderCommand &command);
        void AddRenderCommands(const std::vector<Graphics::RenderCommand> &commands);

    public:
        Engine(const char *appName, const char *appVersion, const char *appId, const char *windowName);
        bool Update();
        virtual ~Engine();
    };
}
#endif