#ifndef HDEF_IMOUSEPROVIDER
#define HDEF_IMOUSEPROVIDER

#include <array>

namespace Mochi::Input
{
    class IMouseProvider
    {
    public:
        virtual ~IMouseProvider() {}
        virtual std::array<bool, 5> GetState(float *x, float *y) = 0;
    };
}

#endif