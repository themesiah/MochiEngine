#include "Layer.h"

#include "Event/EngineEvents.h"

namespace Mochi
{
    Layer::Layer()
    {
        Engine &e = Engine::Get();
        mCatalog = e.GetCatalog();
        mScripting = e.GetScriptingManager();
        mCamera = e.GetCamera();
        mEventBus = e.GetEventBus();
        mGUI = e.GetGUI();
        mActionManager = e.GetActionManager();
        mAudioManager = e.GetAudio();
        mRenderer = e.GetRenderer();
        mECSWorld = e.GetECSWorld();
#if DEBUG
        mGizmos = e.GetGizmos();
#endif

        mAudioSwapHandle = mEventBus->Subscribe<AudioManagerSwappedEvent>(
            [this](const AudioManagerSwappedEvent &ev)
            { mAudioManager = ev.newManager; });

        mCameraSwapHandle = mEventBus->Subscribe<CameraSwappedEvent>(
            [this](const CameraSwappedEvent &ev)
            { mCamera = ev.newCamera; });

        mActionManagerSwapHandle = mEventBus->Subscribe<ActionManagerSwappedEvent>(
            [this](const ActionManagerSwappedEvent &ev)
            { mActionManager = ev.newActionManager; });

        mRendererSwapHandle = mEventBus->Subscribe<RendererSwappedEvent>(
            [this](const RendererSwappedEvent &ev)
            {
                mRenderer = ev.newRenderer;
                mGUI = ev.newGui;
#if DEBUG
                mGizmos = ev.newGizmos;
#endif
            });
    }

    Layer::~Layer()
    {
        mEventBus->Unsubscribe<AudioManagerSwappedEvent>(mAudioSwapHandle);
        mEventBus->Unsubscribe<CameraSwappedEvent>(mCameraSwapHandle);
        mEventBus->Unsubscribe<ActionManagerSwappedEvent>(mActionManagerSwapHandle);
        mEventBus->Unsubscribe<RendererSwappedEvent>(mRendererSwapHandle);
    }
}