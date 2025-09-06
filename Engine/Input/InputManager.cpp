#include "InputManager.h"

#include <iostream>

namespace Mochi::Input
{
    InputManager::InputManager()
    {
        memset(mKeyboardState, false, sizeof(mKeyboardState));
        memset(mKeyboardLastState, false, sizeof(mKeyboardLastState));
    }

    bool InputManager::IsDown(const unsigned int &key) const
    {
        return mKeyboardState[key];
    }

    bool InputManager::WasPressed(const unsigned int &key) const
    {
        return mKeyboardState[key] && !mKeyboardLastState[key];
    }

    bool InputManager::WasReleased(const unsigned int &key) const
    {
        return !mKeyboardState[key] && mKeyboardLastState[key];
    }

    void InputManager::Update(const bool *keyboardState, const uint32_t &mouseFlags, const float &x, const float &y)
    {
        memcpy(mKeyboardLastState, mKeyboardState, SDL_SCANCODE_COUNT);
        memcpy(mKeyboardState, keyboardState, SDL_SCANCODE_COUNT);

        mMouseLastState = mMouseState;
        mMouseState = mouseFlags;
        mMouseX = x;
        mMouseY = y;
    }

    bool InputManager::MouseIsDown(const unsigned int &mouseButton) const
    {
        return mMouseState == SDL_BUTTON_MASK(mouseButton);
    }

    bool InputManager::MouseWasPressed(const unsigned int &mouseButton) const
    {
        return mMouseState == SDL_BUTTON_MASK(mouseButton) && mMouseLastState != SDL_BUTTON_MASK(mouseButton);
    }

    bool InputManager::MouseWasReleased(const unsigned int &mouseButton) const
    {
        return mMouseState != SDL_BUTTON_MASK(mouseButton) && mMouseLastState == SDL_BUTTON_MASK(mouseButton);
    }

    SDL_FPoint InputManager::GetMousePosition() const
    {
        return {mMouseX, mMouseY};
    }

}
