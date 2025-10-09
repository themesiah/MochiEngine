#ifndef HDEF_INPUTMANAGER
#define HDEF_INPUTMANAGER

#include "InputCommons.h"

#include <memory>

#include "../Types/Types.hpp"

namespace Mochi::Graphics
{
    class Renderer;
}
namespace Mochi::Input
{
    class IKeyboardProvider;
    class IMouseProvider;
    class IGamepadProvider;
    class InputManager
    {
    private:
        std::unique_ptr<IKeyboardProvider> mKeyboardProvider;
        std::unique_ptr<IMouseProvider> mMouseProvider;
        std::unique_ptr<IGamepadProvider> mGamepadProvider;

        bool mKeyboardState[SDL_SCANCODE_COUNT];
        bool mKeyboardLastState[SDL_SCANCODE_COUNT];
        std::array<bool, 5> mMouseState;
        std::array<bool, 5> mMouseLastState;

        float mMouseX;
        float mMouseY;
        float mDeltaMouseX;
        float mDeltaMouseY;

        GamepadData mGamepadData;
        GamepadData mLastGamepadData;

    public:
        InputManager(std::unique_ptr<IKeyboardProvider> keyboardProvider, std::unique_ptr<IMouseProvider> mouseProvider, std::unique_ptr<IGamepadProvider> mGamepadProvider);
        virtual ~InputManager();
        bool IsDown(const int &key) const;
        bool WasPressed(const int &key) const;
        bool WasReleased(const int &key) const;
        void Update();
        bool MouseIsDown(const unsigned int &mouseButton) const;
        bool MouseWasPressed(const unsigned int &mouseButton) const;
        bool MouseWasReleased(const unsigned int &mouseButton) const;
        Vector2f GetMousePosition() const;
        Vector2f GetMouseDelta() const;
        float GetGamepadAxis(const GamepadAxis &axis) const;
        bool GamepadButtonIsDown(const GamepadButton &button) const;
        bool GamepadButtonWasPressed(const GamepadButton &button) const;
        bool GamepadButtonWasReleased(const GamepadButton &button) const;
    };
}

#endif