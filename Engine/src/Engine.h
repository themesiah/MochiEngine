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
    class Layer;
    class Engine
    {
    private:
        int mTargetFPS;
        std::chrono::steady_clock::time_point mFrameStart;
        float mLastDeltaTime;
        float mLastRealDelta;

        void Render();
        bool Update();
        std::vector<Graphics::RenderCommand> mRenderQueue;
        std::vector<std::unique_ptr<Layer>> mLayers;
        std::vector<Layer *> mPopLayerQueue;
        std::vector<Layer *> mPushLayerQueue;
        std::unique_ptr<Graphics::Renderer> mRenderer;
        std::unique_ptr<Audio::FMODWrapper> mFmod;
        std::unique_ptr<Input::ActionManager> mActionManager;
        std::unique_ptr<FS::PackCatalog> mCatalog;
        std::unique_ptr<Graphics::Camera> mCamera;
        std::unique_ptr<Graphics::GUI> mGUI;
        std::unique_ptr<Event::EventBus> mEventBus;
        std::unique_ptr<Scripting::ScriptingManager> mScripting;

    public:
        Engine(const char *appName, const char *appVersion, const char *appId, const char *windowName);
        virtual ~Engine();
        void Run();

        static Engine &Get();
        void PushLayer(Layer *layer);
        void PopLayer(Layer *layer);

        float GetLastRealDelta() const { return mLastRealDelta; }
        void AddRenderCommand(const Graphics::RenderCommand &command);
        void AddRenderCommands(const std::vector<Graphics::RenderCommand> &commands);

        // Access subsystems
        Graphics::Renderer *GetRenderer() const { return mRenderer.get(); }
        Audio::FMODWrapper *GetAudio() const { return mFmod.get(); }
        Input::ActionManager *GetActionManager() const { return mActionManager.get(); }
        FS::PackCatalog *GetCatalog() const { return mCatalog.get(); }
        Graphics::Camera *GetCamera() const { return mCamera.get(); }
        Graphics::GUI *GetGUI() const { return mGUI.get(); }
        Event::EventBus *GetEventBus() const { return mEventBus.get(); }
        Scripting::ScriptingManager *GetScriptingManager() const { return mScripting.get(); }
    };
}
#endif