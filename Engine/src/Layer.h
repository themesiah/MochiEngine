#ifndef HDEF_LAYER
#define HDEF_LAYER

#include "Engine.h"
#include "Packer/PackCatalog.h"
#include "Scripting/ScriptingManager.h"
#include "Graphics/Camera.h"
#include "Event/EventBus.h"
#include "GUI/AbstractGUI.h"
#include "Input/IActionManager.h"
#include "Audio/IAudioManager.h"
#include "Graphics/IRenderer.h"
#include "Debug/IGizmos.h"

namespace Mochi
{
    /// @brief The layer class offers a simple way to organize domains of the application while having access to most of the Engine subsystems.
    /// Note that a layer is meant to be inherited from and can't be instantated as is.
    /// A layer will Update, Render and show the GUI as a small Engine class. So the layer is just a convenient responsability separator.
    /// Ownership:
    /// - The layer class owns only what its children owns
    ///
    /// Lifecycle:
    /// - It should be created on the heap manually and pushed onto the engine. The rest of the lifecycle is managed there.
    /// - The engine initializes it at the end of the frame, and when removed, it destroys it at the end of the frame too.
    ///
    /// Guarantees:
    /// - Participation in the lifecycle when pushed to the engine.
    /// - Safe destruction after removing it or destroying the engine instance.
    ///
    /// Thread safety:
    /// - Layers are thread safe if the implementation is thread safe.
    class Layer
    {
    private:
        Event::SubscriptionHandler mAudioSwapHandle;
        Event::SubscriptionHandler mCameraSwapHandle;
        Event::SubscriptionHandler mActionManagerSwapHandle;
        Event::SubscriptionHandler mRendererSwapHandle;

    protected:
        FS::PackCatalog *mCatalog;
        Scripting::ScriptingManager *mScripting;
        Graphics::Camera *mCamera;
        Event::EventBus *mEventBus;
        Graphics::AbstractGUI *mGUI;
        Input::IActionManager *mActionManager;
        Audio::IAudioManager *mAudioManager;
        Graphics::IRenderer *mRenderer;
#if DEBUG
        Debug::IGizmos *mGizmos;
#endif

    public:
        /// @brief Default constructor. It initializes with references to the engine subsystems.
        /// Should only be constructed after the engine is setup.
        Layer();
        virtual ~Layer();

        /// @brief Override this to initialize members and data of your layer when the engine adds this layer to the layer list.
        /// This is useful if you want to create the layer and delay its inclusion on the engine.
        virtual void InitLayer() {};

        /// @brief Override this to give your layer a way to be updated each frame.
        /// @param dt The delta time
        /// @returns true if the execution should continue, false if the execution should halt.
        virtual bool Update(const float &dt) { return true; }

        /// @brief Override this to send the render commands to the engine.
        /// This task could be done on the update, but making it here its clearer.
        virtual void Render() const {}

        /// @brief Override this to draw the GUI of your layer.
        /// Even if this could be done anywhere, doing it here ensure it to be drawn after all rendering is done.
        virtual void GUI() const {}
#if DEBUG
        /// @brief Override this to print, draw or show debug GUI's.
        /// This will be called after all renderings and other GUIs and will only be shown while in DEBUG configuration.
        virtual void Debug() const {}
#endif
    };
}

#endif