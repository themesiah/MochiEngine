#ifndef HDEF_IGAMEPADPROVIDER
#define HDEF_IGAMEPADPROVIDER

#include "InputCommons.h"

namespace Mochi::Input
{
    /**
     * Interface used to get per-frame data of the connected gamepads.
     */
    class IGamepadProvider
    {
    public:
        virtual ~IGamepadProvider() {}
        /// @brief Gets data of the gamepad in the selected slot for this frame.
        /// @param player The slot to get the data from.
        /// @return GamepadData of the selected slot for this frame.
        virtual GamepadData GetData(const unsigned int &player) const = 0;
        /// @brief Checks if there is a gamepad connected in the specified player slot.
        /// @param player The slot to check.
        /// @return True if the slot has a gamepad connected.
        virtual bool HasGamepad(const unsigned int &player) const = 0;
    };
}

#endif