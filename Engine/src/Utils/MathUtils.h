#ifndef HDEF_MATHUTILS
#define HDEF_MATHUTILS

#include <stdint.h>

namespace Mochi::Math
{
    inline bool Approx(float a, float b)
    {
        return std::fabs(a - b) < __FLT_EPSILON__;
    }
}
#endif