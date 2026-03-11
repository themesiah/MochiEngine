#ifndef HDEF_ECSCOLLISIONEVENT
#define HDEF_ECSCOLLISIONEVENT

#include <entt/entt.hpp>

namespace Mochi::ECS
{
    struct CollisionEvent
    {
        entt::entity Entity;
        entt::entity Other;
        bool Trigger;
    };
}

#endif