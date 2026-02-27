#ifndef HDEF_IKEYBOARDPROVIDER
#define HDEF_IKEYBOARDPROVIDER

#include <cstddef>

namespace Mochi::Input
{
    /**
     * Interface used to get per-frame data of the connected keyboard.
     */
    class IKeyboardProvider
    {
    public:
        virtual ~IKeyboardProvider() {}
        /// @brief Gets an array of boolean values defining if each keyboard key is being pressed this frame or not.
        /// Key mapping is responsability of each implementation, but currently should follow the [SDL keycode definitions](https://wiki.libsdl.org/SDL3/SDL_Keycode)
        /// @return A pointer to a bool array.
        virtual const bool *GetState() const = 0;
        /// @brief Gets the maximum amount of keys, which is the size of the GetState array of bools.
        /// @return The amount of keys the keyboard has.
        virtual size_t GetMaxKeys() const = 0;
    };
}

#endif