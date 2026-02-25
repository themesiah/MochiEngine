#ifndef HDEF_POINTSSYSTEM
#define HDEF_POINTSSYSTEM

#include "Event/EventBus.h"

namespace Mochi::Graphics
{
    class AbstractGUI;
}
namespace Mochi::Shooter
{
    class PointsSystem
    {
    private:
        unsigned int mPoints;
        unsigned int mMultiplier;
        unsigned int mMaxMultiplier;
        Event::EventBus *mEventBus;
        Graphics::AbstractGUI *mGUI;

        Event::SubscriptionHandler mDeadEnemyHandler;
        Event::SubscriptionHandler mPlayerDamageHandler;
        Event::SubscriptionHandler mPlayerDeadHandler;

        void OnEnemyDestroyed(unsigned int points);

    public:
        PointsSystem(Event::EventBus *eventBus, Graphics::AbstractGUI *gui);
        virtual ~PointsSystem();
        void Draw() const;
        unsigned int GetPoints() const;
    };
}

#endif