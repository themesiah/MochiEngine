#ifndef HDEF_IMOUSEPROVIDER
#define HDEF_IMOUSEPROVIDER

#include <array>
#include "InputCommons.h"

namespace Mochi::Input
{
    class IMouseProvider
    {
    public:
        virtual ~IMouseProvider() {}
        virtual std::array<bool, MouseButton::MouseButtonsCount> GetState(float *x, float *y) const = 0;
    };
}

#endif