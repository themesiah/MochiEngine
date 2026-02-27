#ifndef HDEF_INPUTCOMMONS
#define HDEF_INPUTCOMMONS

#include <array>

namespace Mochi::Input
{

    enum ActionTrigger
    {
        Never = 0,
        Down = 1,
        Pressed = 2,
        Released = 3,
        ActionTriggerCount = 4
    };

    enum MouseButton
    {
        MouseButtonInvalid = -1,
        MouseButtonLeft = 0,
        MouseButtonCenter = 1,
        MouseButtonRight = 2,
        MouseButtonSide1 = 3,
        MouseButtonSide2 = 4,
        MouseButtonsCount = 5
    };

    enum MouseAxis
    {
        MouseAxisInvalid = -1,
        MouseAxisX = 0,
        MouseAxisY,
        MouseAxisDeltaX,
        MouseAxisDeltaY,
        MouseAxisCount
    };

    enum GamepadAxis
    {
        GAMEPAD_AXIS_INVALID = -1,
        GAMEPAD_AXIS_LEFTX,
        GAMEPAD_AXIS_LEFTY,
        GAMEPAD_AXIS_RIGHTX,
        GAMEPAD_AXIS_RIGHTY,
        GAMEPAD_AXIS_LEFT_TRIGGER,
        GAMEPAD_AXIS_RIGHT_TRIGGER,
        GAMEPAD_AXIS_COUNT
    };

    enum GamepadButton
    {
        GAMEPAD_BUTTON_INVALID = -1,
        GAMEPAD_BUTTON_SOUTH, /**< Bottom face button (e.g. Xbox A button) */
        GAMEPAD_BUTTON_EAST,  /**< Right face button (e.g. Xbox B button) */
        GAMEPAD_BUTTON_WEST,  /**< Left face button (e.g. Xbox X button) */
        GAMEPAD_BUTTON_NORTH, /**< Top face button (e.g. Xbox Y button) */
        GAMEPAD_BUTTON_BACK,
        GAMEPAD_BUTTON_GUIDE,
        GAMEPAD_BUTTON_START,
        GAMEPAD_BUTTON_LEFT_STICK,
        GAMEPAD_BUTTON_RIGHT_STICK,
        GAMEPAD_BUTTON_LEFT_SHOULDER,
        GAMEPAD_BUTTON_RIGHT_SHOULDER,
        GAMEPAD_BUTTON_DPAD_UP,
        GAMEPAD_BUTTON_DPAD_DOWN,
        GAMEPAD_BUTTON_DPAD_LEFT,
        GAMEPAD_BUTTON_DPAD_RIGHT,
        GAMEPAD_BUTTON_MISC1,         /**< Additional button (e.g. Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button, Google Stadia capture button) */
        GAMEPAD_BUTTON_RIGHT_PADDLE1, /**< Upper or primary paddle, under your right hand (e.g. Xbox Elite paddle P1) */
        GAMEPAD_BUTTON_LEFT_PADDLE1,  /**< Upper or primary paddle, under your left hand (e.g. Xbox Elite paddle P3) */
        GAMEPAD_BUTTON_RIGHT_PADDLE2, /**< Lower or secondary paddle, under your right hand (e.g. Xbox Elite paddle P2) */
        GAMEPAD_BUTTON_LEFT_PADDLE2,  /**< Lower or secondary paddle, under your left hand (e.g. Xbox Elite paddle P4) */
        GAMEPAD_BUTTON_TOUCHPAD,      /**< PS4/PS5 touchpad button */
        GAMEPAD_BUTTON_MISC2,         /**< Additional button */
        GAMEPAD_BUTTON_MISC3,         /**< Additional button */
        GAMEPAD_BUTTON_MISC4,         /**< Additional button */
        GAMEPAD_BUTTON_MISC5,         /**< Additional button */
        GAMEPAD_BUTTON_MISC6,         /**< Additional button */
        GAMEPAD_BUTTON_COUNT
    };

    /// @brief Data for Axis and Buttos of a single gamepad.
    struct GamepadData
    {
        std::array<float, GamepadAxis::GAMEPAD_AXIS_COUNT> AxisData;
        std::array<bool, GamepadButton::GAMEPAD_BUTTON_COUNT> ButtonsData;
    };
}

#endif