#ifndef HDEF_ENGINE
#define HDEF_ENGINE

#include <memory>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <sol/sol.hpp>

#include "Time/TimeSystem.h"
#include "Event/EventBus.h"

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
        class IRenderer;
        class AnimationFactory;
        class AbstractGUI;
        struct RenderCommand;
    }
    namespace Audio
    {
        class IAudioManager;
    }
    namespace Input
    {
        class IActionManager;
    }
    namespace FS
    {
        class PackCatalog;
    }
    namespace Event
    {
        class ISystemEventDispatcher;
    }
    namespace Scripting
    {
        class ScriptingManager;
    }
    namespace Debug
    {
        class IGizmos;
    }
    class Layer;
    class Engine
    {
    private:
        int mTargetFPS;
        std::chrono::steady_clock::time_point mFrameStart;
        float mLastDeltaTime;
        float mLastRealDelta;
        bool mIsRunning;
        Event::SubscriptionHandler mAppQuitHandler;

        std::vector<Graphics::RenderCommand> mRenderQueue;
        std::vector<Layer *> mPopLayerQueue;
        std::vector<Layer *> mPushLayerQueue;
        std::unique_ptr<Event::ISystemEventDispatcher> mEventDispatcher;
        std::unique_ptr<Graphics::IRenderer> mRenderer;
        std::unique_ptr<Audio::IAudioManager> mAudio;
        std::unique_ptr<Input::IActionManager> mActionManager;
        std::unique_ptr<FS::PackCatalog> mCatalog;
        std::unique_ptr<Graphics::Camera> mCamera;
        std::unique_ptr<Graphics::AbstractGUI> mGUI;
        std::unique_ptr<Event::EventBus> mEventBus;
        std::unique_ptr<Scripting::ScriptingManager> mScripting;
        std::unique_ptr<Debug::IGizmos> mGizmos;
        std::vector<std::unique_ptr<Layer>> mLayers;

        void PreciseDelay(std::chrono::nanoseconds ns) const;

    public:
        Engine(const char *appName, const char *appVersion, const char *appId, const char *windowName);
        virtual ~Engine();
        void Run();
        void Render();
        bool Update(const float &dt);

        static Engine &Get();
        void PushLayer(Layer *layer);
        void PopLayer(Layer *layer);

        float GetLastRealDelta() const { return mLastRealDelta; }
        void AddRenderCommand(const Graphics::RenderCommand &command);
        void AddRenderCommands(const std::vector<Graphics::RenderCommand> &commands);

        // Access subsystems
        Graphics::IRenderer *GetRenderer() const { return mRenderer.get(); }
        Audio::IAudioManager *GetAudio() const { return mAudio.get(); }
        Input::IActionManager *GetActionManager() const { return mActionManager.get(); }
        FS::PackCatalog *GetCatalog() const { return mCatalog.get(); }
        Graphics::Camera *GetCamera() const { return mCamera.get(); }
        Graphics::AbstractGUI *GetGUI() const { return mGUI.get(); }
        Event::EventBus *GetEventBus() const { return mEventBus.get(); }
        Scripting::ScriptingManager *GetScriptingManager() const { return mScripting.get(); }
        Debug::IGizmos *GetGizmos() const { return mGizmos.get(); }

        void SwapAudioManager(std::unique_ptr<Audio::IAudioManager> &&);
        void SwapActionManager(std::unique_ptr<Input::IActionManager> &&);
        void SwapCamera(std::unique_ptr<Graphics::Camera> &&);
        void SwapRenderer(std::unique_ptr<Graphics::IRenderer> &&);
    };
}
#endif