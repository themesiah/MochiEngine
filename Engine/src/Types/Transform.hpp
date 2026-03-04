#ifndef HDEF_TRANSFORM
#define HDEF_TRANSFORM

#include "Types.hpp"

namespace Mochi
{
    /**
     * @defgroup Types Custom utility types
     * @{
     */

    /// @brief Minimal transform with a position and scale.
    /// Rotation is deliberately omited because of the incompatibility with pixel art.
    /// This class is meant to represent the space occupied by an entity, and reused for several elements of
    /// that same entity, as the game logic, sprite or collider.
    struct Transform
    {
        Vector2f Position;
        float Scale;
        void Move(const Vector2f &delta) { Position += delta; }
        Transform() : Position({0.0f, 0.0f}), Scale(1.0f) {}
    };

    /**@} */
}

#endif