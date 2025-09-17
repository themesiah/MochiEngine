#include "PerformableActions.h"

#include <nlohmann/json.hpp>

#include "InputManager.h"
#include "../Utils/Assert.h"
#include "../Exception.hpp"
#include "../Utils/Logger.h"

namespace Mochi::Input
{
    PerformableActionKeyboard::PerformableActionKeyboard(const nlohmann::json &json)
    {
        mKey = -1;
        mTrigger = ActionTrigger::Never;
        mValue = 0.0f;

        if (!json.contains("Trigger") || !json.at("Trigger").is_number_integer() || json.at("Trigger") < 0 || json.at("Trigger") >= ActionTrigger::ActionTriggerCount)
            throw MalformedInputAction(std::source_location::current());
        mTrigger = (ActionTrigger)json.at("Trigger");

        if (!json.contains("Keycode") || !json.at("Keycode").is_number_integer() || json.at("Keycode") < 0)
            throw MalformedInputAction(std::source_location::current());
        mKey = json.at("Keycode");

        if (!json.contains("Value") || !json.at("Value").is_number_integer())
            throw MalformedInputAction(std::source_location::current());
        mValue = json.at("Value");
    }

    PerformableActionKeyboard::~PerformableActionKeyboard()
    {
    }

    bool PerformableActionKeyboard::IsPerformed(std::shared_ptr<InputManager> input) const
    {
        switch (mTrigger)
        {
        case ActionTrigger::Down:
            return input->IsDown(mKey);
        case ActionTrigger::Pressed:
            return input->WasPressed(mKey);
        case ActionTrigger::Released:
            return input->WasReleased(mKey);
        default:
            return false;
        }
    }

    float PerformableActionKeyboard::GetValue(std::shared_ptr<InputManager> input) const
    {
        if (IsPerformed(input))
            return mValue;
        else
            return 0.0f;
    }

    PerformableActionMouse::PerformableActionMouse(const nlohmann::json &json)
    {
        mButton = MouseButton::MouseButtonInvalid;
        mValue = 0.0f;
        mTrigger = ActionTrigger::Never;

        if (!json.contains("Trigger") || !json.at("Trigger").is_number_integer() || json.at("Trigger") < 0 || json.at("Trigger") >= ActionTrigger::ActionTriggerCount)
            throw MalformedInputAction(std::source_location::current());
        mTrigger = (ActionTrigger)json.at("Trigger");

        if (!json.contains("Button") || !json.at("Button").is_number_integer() || json.at("Button") < 0 || json.at("Button") >= MouseButton::MouseButtonsCount)
            throw MalformedInputAction(std::source_location::current());
        mButton = (MouseButton)json.at("Button");

        if (!json.contains("Value") || !json.at("Value").is_number_integer())
            throw MalformedInputAction(std::source_location::current());
        mValue = json.at("Value");
    }
    PerformableActionMouse::~PerformableActionMouse()
    {
    }
    bool PerformableActionMouse::IsPerformed(std::shared_ptr<InputManager> input) const
    {
        switch (mTrigger)
        {
        case ActionTrigger::Down:
            return input->MouseIsDown(mButton);
        case ActionTrigger::Pressed:
            return input->MouseWasPressed(mButton);
        case ActionTrigger::Released:
            return input->MouseWasReleased(mButton);
        default:
            return false;
        }
    }
    float PerformableActionMouse::GetValue(std::shared_ptr<InputManager> input) const
    {
        if (IsPerformed(input))
            return mValue;
        else
            return 0.0f;
    }

    PerformableActionMouseAxis::PerformableActionMouseAxis(const nlohmann::json &json)
    {
        mAxis = MouseAxis::MouseAxisInvalid;
        mScale = 1.0f;

        if (!json.contains("Axis") || !json.at("Axis").is_number_integer() || json.at("Axis") < 0 || json.at("Axis") >= MouseAxis::MouseAxisCount)
            throw MalformedInputAction(std::source_location::current());
        mAxis = (MouseAxis)json.at("Axis");

        if (json.contains("Scale") && json.at("Scale").is_number())
        {
            mScale = json.at("Scale");
        }
    }
    PerformableActionMouseAxis::~PerformableActionMouseAxis()
    {
    }
    bool PerformableActionMouseAxis::IsPerformed(std::shared_ptr<InputManager> input) const
    {
        return true;
    }
    float PerformableActionMouseAxis::GetValue(std::shared_ptr<InputManager> input) const
    {
        switch (mAxis)
        {
        case MouseAxis::MouseAxisX:
            return input->GetMousePosition().x;
        case MouseAxis::MouseAxisY:
            return input->GetMousePosition().y;
        case MouseAxis::MouseAxisDeltaX:
            return input->GetMouseDelta().x * mScale;
        case MouseAxis::MouseAxisDeltaY:
            return input->GetMouseDelta().y * mScale;
        default:
            return 0.0f;
        }
    }

    PerformableActionGamepad::PerformableActionGamepad(const nlohmann::json &json)
    {
        mButton = GamepadButton::GAMEPAD_BUTTON_INVALID;
        mValue = 0.0f;
        mTrigger = ActionTrigger::Never;

        if (!json.contains("Trigger") || !json.at("Trigger").is_number_integer() || json.at("Trigger") < 0 || json.at("Trigger") >= ActionTrigger::ActionTriggerCount)
            throw MalformedInputAction(std::source_location::current());
        mTrigger = (ActionTrigger)json.at("Trigger");

        if (!json.contains("Button") || !json.at("Button").is_number_integer() || json.at("Button") < 0 || json.at("Button") >= GamepadButton::GAMEPAD_BUTTON_COUNT)
            throw MalformedInputAction(std::source_location::current());
        mButton = (GamepadButton)json.at("Button");

        if (!json.contains("Value") || !json.at("Value").is_number_integer())
            throw MalformedInputAction(std::source_location::current());
        mValue = json.at("Value");
    }
    PerformableActionGamepad::~PerformableActionGamepad()
    {
    }
    bool PerformableActionGamepad::IsPerformed(std::shared_ptr<InputManager> input) const
    {
        switch (mTrigger)
        {
        case ActionTrigger::Down:
            return input->GamepadButtonIsDown(mButton);
        case ActionTrigger::Pressed:
            return input->GamepadButtonWasPressed(mButton);
        case ActionTrigger::Released:
            return input->GamepadButtonWasReleased(mButton);
        default:
            return false;
        }
    }
    float PerformableActionGamepad::GetValue(std::shared_ptr<InputManager> input) const
    {
        if (IsPerformed(input))
            return mValue;
        else
            return 0.0f;
    }

    PerformableActionGamepadAxis::PerformableActionGamepadAxis(const nlohmann::json &json)
    {
        mAxis = GamepadAxis::GAMEPAD_AXIS_INVALID;
        mScale = 1.0f;
        mThreshold = 0.5f;

        if (!json.contains("Axis") || !json.at("Axis").is_number_integer() || json.at("Axis") < 0 || json.at("Axis") >= GamepadAxis::GAMEPAD_AXIS_COUNT)
            throw MalformedInputAction(std::source_location::current());
        mAxis = (GamepadAxis)json.at("Axis");

        if (json.contains("Scale") && json.at("Scale").is_number())
        {
            mScale = json.at("Scale");
        }

        if (json.contains("Threshold") && json.at("Threshold").is_number())
        {
            mThreshold = json.at("Threshold");
        }
    }
    PerformableActionGamepadAxis::~PerformableActionGamepadAxis()
    {
    }
    bool PerformableActionGamepadAxis::IsPerformed(std::shared_ptr<InputManager> input) const
    {
        return std::fabs(GetValue(input)) >= mThreshold;
    }
    float PerformableActionGamepadAxis::GetValue(std::shared_ptr<InputManager> input) const
    {
        return input->GetGamepadAxis(mAxis) * mScale;
    }

}