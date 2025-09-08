#ifndef HDEF_INPUTMANAGER
#define HDEF_INPUTMANAGER

#include "../Types/Types.hpp"

namespace Mochi::Input
{
    class InputManager
    {
    private:
        bool mKeyboardState[SDL_SCANCODE_COUNT];
        bool mKeyboardLastState[SDL_SCANCODE_COUNT];
        uint32_t mMouseState;
        uint32_t mMouseLastState;

        float mMouseX;
        float mMouseY;

    public:
        InputManager();
        bool IsDown(const unsigned int &key) const;
        bool WasPressed(const unsigned int &key) const;
        bool WasReleased(const unsigned int &key) const;
        void Update(const bool *keyboardState, const uint32_t &mouseFlags, const float &x, const float &y);
        bool MouseIsDown(const unsigned int &mouseButton) const;
        bool MouseWasPressed(const unsigned int &mouseButton) const;
        bool MouseWasReleased(const unsigned int &mouseButton) const;
        Vector2f GetMousePosition() const;
    };
}

#endif