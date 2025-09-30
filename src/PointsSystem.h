#ifndef HDEF_POINTSSYSTEM
#define HDEF_POINTSSYSTEM

#include <memory>

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
        std::shared_ptr<Event::EventBus> mEventBus;
        std::shared_ptr<Graphics::GUI> mGUI;

        Event::SubscriptionHandler mDeadEnemyHandler;
        Event::SubscriptionHandler mPlayerDamageHandler;

        void OnEnemyDestroyed(unsigned int points);

    public:
        PointsSystem(std::shared_ptr<Event::EventBus> eventBus, std::shared_ptr<Graphics::GUI> gui);
        ~PointsSystem();
        void Draw() const;
    };
}

#endif