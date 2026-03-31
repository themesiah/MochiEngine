#ifndef HDEF_ECSCHARACTERCONTROLLEREVENTS
#define HDEF_ECSCHARACTERCONTROLLEREVENTS

#include "../ECSWorld.h"

namespace Mochi::ECS
{
    struct CCJumpEvent
    {
        EntityType Entity;
    };

    struct CCGroundedEvent
    {
        EntityType Entity;
    };
}

#endif