#include "PointsSystem.h"

#include <string>
#include <algorithm>

#include "GUI/AbstractGUI.h"
#include "Event/EventBus.h"
#include "ShooterEvents.h"
#include "Constants.h"

namespace Mochi::Shooter
{
    PointsSystem::PointsSystem(Event::EventBus *eventBus, Graphics::AbstractGUI *gui)
        : mPoints(0),
          mMultiplier(1),
          mMaxMultiplier(60),
          mEventBus(eventBus),
          mGUI(gui)
    {

        mDeadEnemyHandler = mEventBus->Subscribe<EnemyDestroyedEvent>(
            [&](const EnemyDestroyedEvent &e)
            { OnEnemyDestroyed(e.Points); });

        mPlayerDamageHandler = mEventBus->Subscribe<PlayerDamageReceivedEvent>(
            [&](const PlayerDamageReceivedEvent &e)
            { mMultiplier = 1; });

        mPlayerDeadHandler = mEventBus->Subscribe<PlayerDeadEvent>(
            [&](const PlayerDeadEvent &e)
            { mPoints = 0; });
    }

    PointsSystem::~PointsSystem()
    {
        mEventBus->Unsubscribe<EnemyDestroyedEvent>(mDeadEnemyHandler);
        mEventBus->Unsubscribe<PlayerDamageReceivedEvent>(mPlayerDamageHandler);
        mEventBus->Unsubscribe<PlayerDeadEvent>(mPlayerDeadHandler);
    }

    void PointsSystem::OnEnemyDestroyed(unsigned int points)
    {
        mPoints += points * mMultiplier;
        if (mPoints > 999999999)
        {
            mPoints = 999999999;
        }
        if (mMultiplier < mMaxMultiplier)
        {
            mMultiplier++;
        }
    }

    void PointsSystem::Draw() const
    {
        auto pointsString = std::to_string(mPoints);
        const size_t numberOfZeroes = 9;
        pointsString = std::string(numberOfZeroes - std::min(numberOfZeroes, pointsString.length()), '0') + pointsString;
        auto multiplierString = std::format("x{}", std::to_string(mMultiplier));

        mGUI->Text(pointsString.c_str(), 20.0f, {CONST_RENDER_LOGICAL_X - 112.0f, 0.0f}, {255, 255, 255, 255});
        mGUI->Text(multiplierString.c_str(), 20.0f, {CONST_RENDER_LOGICAL_X - 40.0f, 24.0f}, {255, 255, 255, 255});
    }
}