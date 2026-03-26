#ifndef HDEF_MATHUTILS
#define HDEF_MATHUTILS

#define F_EPSILON 0.00001f
#define F_PI 3.14159265359f

namespace Mochi::Math
{
    /**
     * @defgroup Math Math utilities
     *
     * List of useful math methods useful in many contexts.
     *
     * Utils/MathUtils.h
     * @{
     */

    /// @brief Checks if two floats are the "same" for logic purposes, comparing their difference with a really small number.
    /// @param a First number
    /// @param b Second number
    /// @return True if they are the same or almost the same.
    inline bool Approx(const float &a, const float &b)
    {
        return std::fabs(a - b) < F_EPSILON;
    }

    /// @brief Returns 1, 0 or -1 depending on the sign of the provided number.
    /// @param a The number to check
    /// @return 1, 0 or -1
    inline float Sign(const float &a)
    {
        if (a < 0.0f)
            return -1.0f;
        else if (a > 0.0f)
            return 1.0f;
        else
            return 0.0f;
    }

    /// @brief Returns the absolute value of a float.
    /// @param a The float
    /// @return The absolute value
    inline float Abs(const float &a)
    {
        if (a < 0.0f)
            return -a;
        else
            return a;
    }

    /// @brief Returns the maximum number between two numbers.
    /// @param a First number.
    /// @param b Second number.
    /// @return The biggest of the two numbers.
    inline float Max(const float &a, const float &b)
    {
        if (a >= b)
            return a;
        else
            return b;
    }

    /// @brief Returns the minimum number between two numbers.
    /// @param a First number.
    /// @param b Second number.
    /// @return The smallest of the two numbers.
    inline float Min(const float &a, const float &b)
    {
        if (a <= b)
            return a;
        else
            return b;
    }

    /// @brief Returns a value between min and max, the closest to value.
    /// @param value The reference value.
    /// @param min The minimum result.
    /// @param max The maximum result.
    /// @return Min if value was smaller than min, max if it was bigger than max, an value otherwise.
    inline float Clamp(const float &value, const float &min, const float &max)
    {
        if (value <= min)
            return min;
        else if (value >= max)
            return max;
        else
            return value;
    }

    /// @brief Does a Clamp having 0 as min and 1 and max.
    /// @param value The reference value.
    /// @return 0 if value was smaller than 0, 1 if the value was bigger than 1, the value otherwise.
    inline float Clamp01(const float &value)
    {
        return Clamp(value, 0.0f, 1.0f);
    }

    /// @brief Increases or reduces a value to approach a second value, with a defined speed.
    /// @param a The base value
    /// @param b The target value
    /// @param dt How much time passes moving a to b for the calculations of this operation.
    /// @param speed The speed at which a moves.
    /// @return b if the operation equals of exceeds b value, or the result of a moved.
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

    /// @brief Similar to doing a module, but with floats.
    /// @param t The value to check.
    /// @param length The module.
    /// @return The final value.
    inline float Repeat(const float &t, const float &length)
    {
        if (length == 0)
            return t;
        float div = static_cast<float>(static_cast<int>(t / length));
        return t - (length * div);
    }

    /// @brief Gives a number between a range defined by a proportion t
    /// @param a Start of the range.
    /// @param b End of the range.
    /// @param t The proportion, between 0 and 1
    /// @return The proportional value in the range. It can be outside the range of if t is less than 0 or greater than 1.
    inline float Lerp(const float &a, const float &b, const float &t)
    {
        return (b - a) * t + a;
    }

    /// @brief Gives the proportional position of a value in a range
    /// @param a Start of the range.
    /// @param b End of the range.
    /// @param val Value in the range.
    /// @return The proportional position of the value between 0 and 1. It can be less than 0 or greater than 1 if val was outside the range.
    inline float InverseLerp(const float &a, const float &b, const float &val)
    {
        if (a == b)
            return val;
        if (val < a)
            return 0;
        if (val > b)
            return 1;
        return (val - a) / (b - a);
    }

    /**@} */
}
#endif