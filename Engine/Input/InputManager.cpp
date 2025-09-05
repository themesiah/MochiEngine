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

    void InputManager::Update(const bool *keyboardState)
    {
        memcpy(mKeyboardLastState, mKeyboardState, SDL_SCANCODE_COUNT);
        memcpy(mKeyboardState, keyboardState, SDL_SCANCODE_COUNT);
    }
}
