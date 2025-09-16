#ifndef HDEF_MATHUTILS
#define HDEF_MATHUTILS

#define F_EPSILON 0.00001f

namespace Mochi::Math
{
    inline bool Approx(float a, float b)
    {
        return std::fabs(a - b) < F_EPSILON;
    }
}
#endif