#include "SDLKeyboardProvider.h"

#include <SDL3/SDL.h>

namespace Mochi::Input
{
    SDLKeyboardProvider::SDLKeyboardProvider()
    {
    }

    SDLKeyboardProvider::~SDLKeyboardProvider()
    {
    }

    const bool *SDLKeyboardProvider::GetState() const
    {
        const bool *keyboardState = SDL_GetKeyboardState(NULL);
        return keyboardState;
    }

    size_t SDLKeyboardProvider::GetMaxKeys() const
    {
        return SDL_SCANCODE_COUNT;
    }
}