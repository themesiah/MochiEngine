#ifndef HDEF_TRANSFORM
#define HDEF_TRANSFORM

#include "Types.hpp"

namespace Mochi
{
    struct Transform
    {
        Vector2f Position;
        float Scale;
        void Move(const Vector2f &delta) { Position += delta; }
        Transform() : Position({0.0f, 0.0f}), Scale(1.0f) {}
    };
}

#endif