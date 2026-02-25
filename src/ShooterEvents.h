#ifndef HDEF_EVENTS
#define HDEF_EVENTS

namespace Mochi::Shooter
{
    class AbstractEnemy;
    struct EnemyDestroyedEvent
    {
        unsigned int Points;
        AbstractEnemy *Enemy;
        int ExplosionScale;
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