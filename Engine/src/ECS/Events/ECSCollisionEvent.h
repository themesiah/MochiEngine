#ifndef HDEF_ECSCOLLISIONEVENT
#define HDEF_ECSCOLLISIONEVENT

#include "../ECSWorld.h"

namespace Mochi::ECS
{
    struct CollisionEvent
    {
        EntityType Entity;
        EntityType Other;
        bool Trigger;
        Vector2f CollisionNormal;
        uint32_t CollisionLayer;
    };
}

#endif