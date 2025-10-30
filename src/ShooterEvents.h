#ifndef HDEF_EVENTS
#define HDEF_EVENTS

namespace Mochi::Shooter
{
    class Enemy;
    struct EnemyDestroyedEvent
    {
        unsigned int Points;
        Enemy *Enemy;
    };

    struct PlayerDamageReceivedEvent
    {
    };

    class Player;
    struct PlayerDeadEvent
    {
        Player *Player;
    };

    struct PlayerContinueEvent
    {
    };
}

#endif