#ifndef HDEF_EVENTS
#define HDEF_EVENTS

namespace Mochi::Shooter
{
    struct EnemyDestroyedEvent
    {
        unsigned int Points;
    };

    struct PlayerDamageReceivedEvent
    {
    };
}

#endif