#ifndef HDEF_LAYER
#define HDEF_LAYER

#include "Engine.h"
#include "Packer/PackCatalog.h"
#include "ScriptingManager.h"
#include "Graphics/Camera.h"
#include "Event/EventBus.h"
#include "GUI/AbstractGUI.h"
#include "Input/IActionManager.h"
#include "Audio/IAudioManager.h"
#include "Graphics/IRenderer.h"
#include "Debug/IGizmos.h"

namespace Mochi
{
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
        Debug::IGizmos *mGizmos;

    public:
        Layer();
        ~Layer();
        virtual bool Update(const float &dt) = 0;
        virtual void Render() const = 0;
        virtual void GUI() const = 0;
#if DEBUG
        virtual void Debug() const {}
#endif
    };
}

#endif