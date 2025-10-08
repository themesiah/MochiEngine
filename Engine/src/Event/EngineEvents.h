#ifndef HDEF_ENGINEEVENTS
#define HDEF_ENGINEEVENTS

namespace Mochi::Audio
{
    class IAudioManager;
}
namespace Mochi::Graphics
{
    class Camera;
    class Renderer;
}
namespace Mochi::Input
{
    class IActionManager;
}
namespace Mochi
{
    struct AudioManagerSwappedEvent
    {
        Audio::IAudioManager *newManager;
    };

    struct CameraSwappedEvent
    {
        Graphics::Camera *newCamera;
    };

    struct ActionManagerSwappedEvent
    {
        Input::IActionManager *newActionManager;
    };

    struct RendererSwappedEvent
    {
        Graphics::IRenderer *newRenderer;
    };
}

#endif