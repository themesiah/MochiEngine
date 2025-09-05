#ifndef HDEF_INPUTMANAGER
#define HDEF_INPUTMANAGER

#include <SDL3/SDL.h>

namespace Mochi::Input
{
    class InputManager
    {
    private:
        bool mKeyboardState[SDL_SCANCODE_COUNT];
        bool mKeyboardLastState[SDL_SCANCODE_COUNT];

    public:
        InputManager();
        bool IsDown(const unsigned int &key) const;
        bool WasPressed(const unsigned int &key) const;
        bool WasReleased(const unsigned int &key) const;
        void Update(const bool *keyboardState);
    };
}

#endif