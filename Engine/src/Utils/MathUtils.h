#ifndef HDEF_MATHUTILS
#define HDEF_MATHUTILS

#define F_EPSILON 0.00001f

namespace Mochi::Math
{
    inline bool Approx(const float &a, const float &b)
    {
        return std::fabs(a - b) < F_EPSILON;
    }

    inline float Max(const float &a, const float &b)
    {
        if (a >= b)
            return a;
        else
            return b;
    }

    inline float Min(const float &a, const float &b)
    {
        if (a <= b)
            return a;
        else
            return b;
    }

    inline float MoveTowards(const float &a, const float &b, const float &dt, const float &speed)
    {
        if (a > b)
        {
            return Max(a - speed * dt, b);
        }
        else if (a < b)
        {
            return Min(a + speed * dt, b);
        }
        else
        {
            return a;
        }
    }
}
#endif