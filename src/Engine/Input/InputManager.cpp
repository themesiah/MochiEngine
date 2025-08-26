#include "InputManager.h"

#include <iostream>

InputManager::InputManager()
{
    for (unsigned int i = 0; i < SDL_SCANCODE_COUNT; ++i)
    {
        mKeyboardState[i] = false;
        mKeyboardLastState[i] = false;
    }
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