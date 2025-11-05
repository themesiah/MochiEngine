#include "PointsSystem.h"

#include <string>
#include <algorithm>

#include "GUI/AbstractGUI.h"
#include "GUI/GUICommon.hpp"
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

        mPlayerDeadHandler = mEventBus->Subscribe<PlayerContinueEvent>(
            [&](const PlayerContinueEvent &e)
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
        const Graphics::GUITextOptions pointsTextOptions{
            .DstRect = {Rectf({-5.0f, 5.0f}, {})},
            .ScreenAnchor = Graphics::GUI_TOP_RIGHT,
            .TextPivot = Graphics::GUI_TOP_RIGHT,
            .TextSize = 20.0f};
        const Graphics::GUITextOptions multiplierTextOptions{
            .DstRect = {Rectf({-5.0f, 25.0f}, {})},
            .ScreenAnchor = Graphics::GUI_TOP_RIGHT,
            .TextPivot = Graphics::GUI_TOP_RIGHT,
            .TextSize = 20.0f};

        auto pointsString = std::to_string(mPoints);
        const size_t numberOfZeroes = 9;
        pointsString = std::string(numberOfZeroes - std::min(numberOfZeroes, pointsString.length()), '0') + pointsString;
        auto multiplierString = std::format("x{}", std::to_string(mMultiplier));

        mGUI->Text(pointsString.c_str(), pointsTextOptions);
        mGUI->Text(multiplierString.c_str(), multiplierTextOptions);
    }
}