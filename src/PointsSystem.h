#ifndef HDEF_POINTSSYSTEM
#define HDEF_POINTSSYSTEM

#include "Event/EventBus.h"

namespace Mochi::Graphics
{
    class GUI;
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
        Graphics::GUI *mGUI;

        Event::SubscriptionHandler mDeadEnemyHandler;
        Event::SubscriptionHandler mPlayerDamageHandler;

        void OnEnemyDestroyed(unsigned int points);

    public:
        PointsSystem(Event::EventBus *eventBus, Graphics::GUI *gui);
        ~PointsSystem();
        void Draw() const;
    };
}

#endif