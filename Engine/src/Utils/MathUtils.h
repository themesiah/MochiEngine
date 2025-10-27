#ifndef HDEF_MATHUTILS
#define HDEF_MATHUTILS

#define F_EPSILON 0.00001f
#define F_PI 3.14159265359f

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

    inline float Clamp(const float &value, const float &min, const float &max)
    {
        if (value <= min)
            return min;
        else if (value >= max)
            return max;
        else
            return value;
    }

    inline float Clamp01(const float &value)
    {
        return Clamp(value, 0.0f, 1.0f);
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

    inline float Repeat(const float &t, const float &length)
    {
        if (length == 0)
            return t;
        float div = static_cast<int>(t / length);
        return t - (length * div);
    }
}
#endif