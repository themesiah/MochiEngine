#ifndef HDEF_ENGINEEVENTS
#define HDEF_ENGINEEVENTS

namespace Mochi::Audio
{
    class IAudioManager;
}
namespace Mochi::Graphics
{
    class Camera;
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
}

#endif