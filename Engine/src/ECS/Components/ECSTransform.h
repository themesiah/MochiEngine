#ifndef HDEF_ECSTRANSFORM
#define HDEF_ECSTRANSFORM

#include "../../Types/Types.hpp"

namespace Mochi::ECS
{
    struct TransformComponent
    {
        Vector2f Position;
        float Scale;
    };
}

#endif