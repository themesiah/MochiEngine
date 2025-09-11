#ifndef HDEF_IGAMEPADPROVIDER
#define HDEF_IGAMEPADPROVIDER

#include "InputCommons.h"

namespace Mochi::Input
{
    class IGamepadProvider
    {
    public:
        ~IGamepadProvider() {}
        virtual GamepadData GetData(const unsigned int &player) const = 0;
        virtual bool HasGamepad(const unsigned int &player) const = 0;
    };
}

#endif