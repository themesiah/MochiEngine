#include "InputManager.h"

#include <iostream>

#include "../Event/EventBus.h"

#include "../Utils/Logger.h"

#include "IKeyboardProvider.h"
#include "IMouseProvider.h"
#include "IGamepadProvider.h"

#include "../Exception.hpp"

namespace Mochi::Input
{
    InputManager::InputManager(std::shared_ptr<IKeyboardProvider> keyboardProvider,
                               std::shared_ptr<IMouseProvider> mouseProvider,
                               std::shared_ptr<IGamepadProvider> gamepadProvider) : mKeyboardProvider(keyboardProvider),
                                                                                    mMouseProvider(mouseProvider),
                                                                                    mGamepadProvider(gamepadProvider)
    {
        memset(mKeyboardState, false, sizeof(mKeyboardState));
        memset(mKeyboardLastState, false, sizeof(mKeyboardLastState));
    }

    InputManager::~InputManager()
    {
    }

    bool InputManager::IsDown(const int &key) const
    {
        return mKeyboardState[key];
    }

    bool InputManager::WasPressed(const int &key) const
    {
        return mKeyboardState[key] && !mKeyboardLastState[key];
    }

    bool InputManager::WasReleased(const int &key) const
    {
        return !mKeyboardState[key] && mKeyboardLastState[key];
    }

    void InputManager::Update()
    {
        // Keyboard
        const bool *keyboardState = mKeyboardProvider->GetState();
        const size_t keyCount = mKeyboardProvider->GetMaxKeys();
        memcpy(mKeyboardLastState, mKeyboardState, keyCount);
        memcpy(mKeyboardState, keyboardState, keyCount);

        // Mouse
        float x, y;
        auto newState = mMouseProvider->GetState(&x, &y);
        mMouseLastState = mMouseState;
        mMouseState = newState;
        mDeltaMouseX = x - mMouseX;
        mDeltaMouseY = y - mMouseY;
        mMouseX = x;
        mMouseY = y;

        // Gamepad
        mLastGamepadData = mGamepadData;
        mGamepadData = {};
        if (mGamepadProvider->HasGamepad(0))
        {
            GamepadData gdata = mGamepadProvider->GetData(0);
            mGamepadData = gdata;
        }
    }

    bool InputManager::MouseIsDown(const unsigned int &mouseButton) const
    {
        if (mouseButton >= mMouseState.size())
            throw EngineError(std::format("Mouse buttons are {} at max", mMouseState.size()));
        return mMouseState[mouseButton];
    }

    bool InputManager::MouseWasPressed(const unsigned int &mouseButton) const
    {
        if (mouseButton >= mMouseState.size())
            throw EngineError(std::format("Mouse buttons are {} at max", mMouseState.size()));
        return mMouseState[mouseButton] && !mMouseLastState[mouseButton];
    }

    bool InputManager::MouseWasReleased(const unsigned int &mouseButton) const
    {
        if (mouseButton >= mMouseState.size())
            throw EngineError(std::format("Mouse buttons are {} at max", mMouseState.size()));
        return !mMouseState[mouseButton] && mMouseLastState[mouseButton];
    }

    Vector2f InputManager::GetMousePosition() const
    {
        return {mMouseX, mMouseY};
    }

    Vector2f InputManager::GetMouseDelta() const
    {
        return {mDeltaMouseX, mDeltaMouseY};
    }

    float InputManager::GetGamepadAxis(const GamepadAxis &axis) const
    {
        return mGamepadData.AxisData[axis];
    }

    bool InputManager::GamepadButtonIsDown(const GamepadButton &button) const
    {
        return mGamepadData.ButtonsData[button];
    }
    bool InputManager::GamepadButtonWasPressed(const GamepadButton &button) const
    {
        return mGamepadData.ButtonsData[button] && !mLastGamepadData.ButtonsData[button];
    }
    bool InputManager::GamepadButtonWasReleased(const GamepadButton &button) const
    {
        return !mGamepadData.ButtonsData[button] && mLastGamepadData.ButtonsData[button];
    }
}
