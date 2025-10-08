#ifndef HDEF_ENGINEEVENTS
#define HDEF_ENGINEEVENTS

namespace Mochi::Audio
{
    class IAudioManager;
}
namespace Mochi::Graphics
{
    class Camera;
    class IRenderer;
    class AbstractGUI;
}
namespace Mochi::Debug
{
    class IGizmos;
}
namespace Mochi::Input
{
    class IActionManager;
}
namespace Mochi
{
    struct ApplicationQuitEvent
    {
    };

    struct GamepadAddedEvent
    {
        unsigned int GamepadID;
    };
    struct GamepadRemovedEvent
    {
        unsigned int GamepadID;
    };

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
        Graphics::AbstractGUI *newGui;
        Debug::IGizmos *newGizmos;
    };
}

#endif