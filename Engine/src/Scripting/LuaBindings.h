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
    /**
     * @defgroup LuaBindings Lua Binding Modules
     * Functions responsible for registering engine subsystems into lua
     * @{
     */

    /**
     * @brief Binds the graphics module classes to lua.
     * Graphics::SpriteBase and its children and Graphics::Camera.
     * @param scriptingManager The scripting manager module.
     * @param camera The main camera.
     */
    void BindGraphics(ScriptingManager *scriptingManager, Graphics::Camera *camera);
    /**
     * @brief Binds base types, input and time methods to lua.
     * Transform, Vector2f, Rectf, Color, Input::IActionManager and Time::TimeSystem.
     * @param scriptingManager The scripting manager module.
     * @param actionManager The action manager.
     */
    void BindLogic(ScriptingManager *scriptingManager, Input::IActionManager *actionManager);
    /**
     * @brief Binds audio methods to lua.
     * Load and unload audio, play, pause and resume music, play oneshots...
     * @param scriptingManager The scripting manager module.
     * @param audioManager The audio manager.
     */
    void BindAudio(ScriptingManager *scriptingManager, Audio::IAudioManager *audioManager);

    /**@} */
}

#endif