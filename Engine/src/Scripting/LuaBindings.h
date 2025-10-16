#ifndef HDEF_LUABINDINGS
#define HDEF_LUABINDINGS

namespace Mochi::Scripting
{
    class ScriptingManager;
}
namespace Mochi::Audio
{
    class IAudioManager;
}
namespace Mochi::Input
{
    class IActionManager;
}
namespace Mochi::Graphics
{
    class Camera;
}
namespace Mochi::Scripting::Bindings
{
    void BindGraphics(ScriptingManager *, Graphics::Camera *);
    void BindLogic(ScriptingManager *, Input::IActionManager *);
    void BindAudio(ScriptingManager *, Audio::IAudioManager *audioManager);
}

#endif