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
    /**
     * @brief Class that takes one IKeyboardProvider, one IMouseProvider and one IGamepadProvider and updates and collects data from them each frame automatically.
     * It provides functions to poll the state of each provider, used by the ActionManager.
     * This class is not meant to be implemented differently in any way, as it already receives different sets of providers for that purpose.
     *
     * NOTE: The engine is not prepared for more than one player as for now. The InputManager takes the polling of IGamepadProvider from the first slot gamepad only.
     *
     * Ownership:
     *
     * - It takes ownership as unique pointers of the IKeyboardProvider, IMouseProvider and IGamepadProvider.
     *
     * Guarantees:
     *
     * - The correct state of each state is polled.
     *
     * - Previous frame states are stored to poll the correct data.
     *
     * Non responsible:
     *
     * - InputManager doesn't check the validity of the provider implementations. The user is responsible to use providers that are tested and do what is expected.
     */
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
        /// @param keyboardProvider
        /// @param mouseProvider
        /// @param mGamepadProvider
        InputManager(std::unique_ptr<IKeyboardProvider> keyboardProvider, std::unique_ptr<IMouseProvider> mouseProvider, std::unique_ptr<IGamepadProvider> mGamepadProvider);
        virtual ~InputManager();
        /// @brief Checks if a keyboard key is down in this frame.
        /// @param key The key code, as defined in the [SDL keycode definitions](https://wiki.libsdl.org/SDL3/SDL_Keycode)
        /// @return True if is pressed.
        bool IsDown(const int &key) const;
        /// @brief Checks if a keyboard key is pressed this frame and wasn't the last.
        /// @param key The key code, as defined in the [SDL keycode definitions](https://wiki.libsdl.org/SDL3/SDL_Keycode)
        /// @return True if is pressed and wasn't pressed last frame.
        bool WasPressed(const int &key) const;
        /// @brief Checks if a keyboard key is not pressed this frame and was the last.
        /// @param key The key code, as defined in the [SDL keycode definitions](https://wiki.libsdl.org/SDL3/SDL_Keycode)
        /// @return True if not pressed and was pressed last frame.
        bool WasReleased(const int &key) const;
        /// @brief Updates the previous frame data with the data of the previous frame and updates the previous frame data with the data of this frame.
        void Update();
        /// @brief Checks if a mouse button is down in this frame.
        /// @param mouseButton The index of the mouse button as defined in MouseButton.
        /// @return True if pressed.
        bool MouseIsDown(const unsigned int &mouseButton) const;
        /// @brief Checks if a mouse button is down in this frame and wasn't last frame.
        /// @param mouseButton The index of the mouse button as defined in MouseButton.
        /// @return True if pressed and not pressed last frame.
        bool MouseWasPressed(const unsigned int &mouseButton) const;
        /// @brief Checks if a mouse button is not pressed in this frame and was last frame.
        /// @param mouseButton The index of the mouse button as defined in MouseButton.
        /// @return True if not pressed and was pressed last frame.
        bool MouseWasReleased(const unsigned int &mouseButton) const;
        /// @brief Gets the mouse position in this frame in screen coordinates.
        /// @return A vector with the x and y position
        Vector2f GetMousePosition() const;
        /// @brief Gets the difference in position between last frames mouse position and this frame mouse position.
        /// @return A vector with the difference vector.
        Vector2f GetMouseDelta() const;
        /// @brief Gets the value of the axis of the gamepad, between -1 and 1.
        /// @param axis The axis to check.
        /// @return The value.
        float GetGamepadAxis(const GamepadAxis &axis) const;
        /// @brief Checks if a gamepad button is down.
        /// @param button The button.
        /// @return True if pressed.
        bool GamepadButtonIsDown(const GamepadButton &button) const;
        /// @brief Checks if a gamepad button is down and wasn't last frame.
        /// @param button The button.
        /// @return True if pressed and wasn't pressed last frame.
        bool GamepadButtonWasPressed(const GamepadButton &button) const;
        /// @brief Checks if a gamepad button is not down and was down last frame.
        /// @param button The button.
        /// @return True if not pressed and was pressed last frame.
        bool GamepadButtonWasReleased(const GamepadButton &button) const;
    };
}

#endif