#ifndef HDEF_ENGINE
#define HDEF_ENGINE

#include <memory>
#include <vector>
#include <unordered_map>
#include <chrono>

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
        class FrameProfiler;
    }
    class Layer;

    /// @brief Main class of the MochiEngine. It owns and coordinates all subsystems and layers, and drives the application lifecycle.
    ///
    /// Ownership:
    /// - Engine has ownership of most of the subsystems.
    /// - Layers are created outside of the class and injected into the engine.
    ///
    /// Lifecycle:
    /// - Create with default constructor and call "Setup" after to initialize.
    ///     - Alternatively, call the constructor Engine(const char *appName, const char *appVersion, const char *appId, const char *windowName) to initialize with default systems.
    /// - Call Run to start the main loop. Run will end automatically when the game receives an exit signal.
    /// - When created, Engine is a singleton accessible everywhere, providing centralized access to core subsystems and giving ability to inject layers and render commands.
    ///
    /// Guarantees:
    /// - Full initialization before entering the main loop. Failures in this step are detected early and exit the application.
    /// - Update is called each frame with a delta time
    /// - Subsystems are automatically updated
    /// - Layers are automatically managed and updated
    /// - Rendering is done once per frame, separated between Graphics and UI
    ///
    /// Thread safety:
    /// - This class is not thread-safe.
    ///
    /// Non responsible:
    /// - Game logic
    /// - Automatic rendering
    /// - Entity management
    class Engine
    {
    private:
        int mTargetFPS;
        std::chrono::steady_clock::time_point mFrameStart;
        float mLastDeltaTime;
        float mLastRealDelta;
        bool mIsRunning;
        bool mIsPaused;
        float mUnpausedTimeScale;
        Event::SubscriptionHandler mAppQuitHandler;

        std::unique_ptr<Event::EventBus> mEventBus;
        std::vector<Graphics::RenderCommand> mRenderQueue;
        std::vector<std::unique_ptr<Layer>> mLayers;
        std::vector<Layer *> mPopLayerQueue;
        std::vector<Layer *> mPushLayerQueue;
        std::unique_ptr<Event::ISystemEventDispatcher> mEventDispatcher;
        std::unique_ptr<Graphics::IRenderer> mRenderer;
        std::unique_ptr<Audio::IAudioManager> mAudio;
        std::unique_ptr<Input::IActionManager> mActionManager;
        std::unique_ptr<FS::PackCatalog> mCatalog;
        std::unique_ptr<Graphics::Camera> mCamera;
        std::unique_ptr<Graphics::AbstractGUI> mGUI;
        std::unique_ptr<Scripting::ScriptingManager> mScripting;
        std::unique_ptr<Debug::IGizmos> mGizmos;
        std::unique_ptr<Debug::FrameProfiler> mProfiler;

        void PreciseDelay(std::chrono::nanoseconds ns) const;
        void MinimalSetup();
        bool Update(const float &dt);
        void Render();

    public:
        /// @brief Access Engine from anywhere
        /// Access is only available after default construction or after setup.
        /// Otherwise, it returns nullptr.
        static Engine &Get();

        /// @brief Constructs the Engine class without initializing some core systems to allow the user to use Setup to initialize them manually.
        /// Doesn't initialize:
        /// - Renderer
        /// - Action Manager
        /// - Audio Manager
        Engine();

        /// @brief Constructs the Engine class and setups it with default core subsystems.
        /// - Renderer: SDLRenderer
        /// - Action Manager: Default action manager
        /// - Audio Manager: FMOD
        /// @param appName The name of the application.
        /// @param appVersion The version of the application.
        /// @param appId A unique string in reverse order identifying the application.
        /// @param windowName The name the window will receive in the OS.
        Engine(const char *appName, const char *appVersion, const char *appId, const char *windowName);
        virtual ~Engine();

        /// @brief Runs the engine, starting the per-frame lifecycle.
        /// Once called, it won't stop until receiving ApplicationQuitEvent or getting "false" when updating a layer.
        void Run();

        /// @brief Manually setups some subsystems for the engine.
        /// Setup should only be called once, and it will keep ownership of the subsystems.
        /// @param renderer The renderer. It will have rendering and UI responsabilities.
        /// @param actionManager The action manager that will define how input will be interpreted.
        /// @param audioManager The audio manager.
        void Setup(std::unique_ptr<Graphics::IRenderer> &&renderer, std::unique_ptr<Input::IActionManager> &&actionManager, std::unique_ptr<Audio::IAudioManager> &&audioManager);

        /// @brief Pushes a layer in the list of layers to initialize. Then initializes it at the end of the frame and gets ownership of it to update it each frame.
        /// The same layer should only be pushed once. However, several instances of the same layer type can be pushed safely.
        /// @param layer A pointer to the layer. Note that several layers of the same type can be active at a time and is the user responsability to manage which layers to push.
        void PushLayer(Layer *layer);

        /// @brief Pushes a layer in the list of layers to deactivate. At the end of the frame (when it is safe) the layer is removed from the list and destroyed.
        /// @param layer The pointer to the layer to remove.
        void PopLayer(Layer *layer);

        float GetLastRealDelta() const { return mLastRealDelta; }

        /// @brief Adds a single render command to be rendered this frame.
        /// @param command The render command
        void AddRenderCommand(const Graphics::RenderCommand &command);

        /// @brief Adds a range of render commands to be rendered this frame.
        /// @param commands The vector of commands
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

        /// @brief Pauses the engine setting the delta time scale to 0.
        /// Pause can be called several times in succession, but it will only be applied once.
        void Pause();

        /// @brief Resumes the game setting the delta time scale to the last value it was before it was paused.
        /// Resume can be called several times in succession, but it will only be applied once.
        void Resume();
    };
}
#endif