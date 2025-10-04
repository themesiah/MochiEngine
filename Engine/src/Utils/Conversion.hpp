#ifndef HDEF_CONVERSION
#define HDEF_CONVERSION

#include "../Constants.h"
#include "../Types/Types.hpp"

inline constexpr float CONST_PIXEL_PER_METER = 20.0f;

namespace Mochi
{
    inline float PixelsToMeters(float px)
    {
        return px / CONST_PIXEL_PER_METER;
    }

    inline Vector2f PixelsToMeters(const Vector2f &v)
    {
        return v / CONST_PIXEL_PER_METER;
    }

    inline Rectf PixelsToMeters(const Rectf &r)
    {
        return {r.x / CONST_PIXEL_PER_METER, r.y / CONST_PIXEL_PER_METER, r.w / CONST_PIXEL_PER_METER, r.h / CONST_PIXEL_PER_METER};
    }

    inline float MetersToPixels(float m)
    {
        return m * CONST_PIXEL_PER_METER;
    }

    inline Vector2f MetersToPixels(const Vector2f &v)
    {
        return v * CONST_PIXEL_PER_METER;
    }

    inline Rectf MetersToPixels(const Rectf &r)
    {
        return {r.x * CONST_PIXEL_PER_METER, r.y * CONST_PIXEL_PER_METER, r.w * CONST_PIXEL_PER_METER, r.h * CONST_PIXEL_PER_METER};
    }

    inline Vector2f FlipY(const Vector2f &v)
    {
        return {v.x, -v.y};
    }
}

#endif