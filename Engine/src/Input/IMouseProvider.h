#ifndef HDEF_IMOUSEPROVIDER
#define HDEF_IMOUSEPROVIDER

#include <array>
#include "InputCommons.h"

namespace Mochi::Input
{
    /**
     * Interface used to get per-frame data of the connected mouse.
     */
    class IMouseProvider
    {
    public:
        virtual ~IMouseProvider() {}
        /// @brief Gets the state of the mouse, consisting of the pointer position in the screen and each of its buttons, defined in InputCommons.h in the MouseButtons enum.
        /// @param x Reference to a value that will be updated in the method with the X position.
        /// @param y Reference to a value that will be updated in the method with the Y position.
        /// @return An array of booleans for each mouse button, up to InputCommons.h in the MouseButtons enum.
        virtual std::array<bool, MouseButton::MouseButtonsCount> GetState(float *x, float *y) const = 0;
    };
}

#endif