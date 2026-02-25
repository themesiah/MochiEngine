#include "Boss.h"

#include <memory>

#include "Graphics/Spritesheet.h"
#include "Utils/Conversion.hpp"

#include "../ZIndexEnum.h"

namespace Mochi::Shooter
{
    Boss::Boss(Event::EventBus *eventBus, Graphics::AbstractTextureFactory *textureFactory, Graphics::IAnimationFactory *animationFactory)
        : AbstractEnemy(eventBus, 4000, 50000), mState(BossState_Background)
    {
        mGraphic = std::make_unique<Graphics::Spritesheet>(animationFactory, textureFactory, "Boss.json", 0);
        mGraphic->SetTransform(mTransform);
        SetState(BossState_Background);
        SetExplosionScale(5);
    }

    Boss::~Boss()
    {
    }

    void Boss::Update(const float &dt)
    {
        AbstractEnemy::Update(dt);
    }

    void Boss::SetState(const BossState &state)
    {
        mState = state;

        switch (mState)
        {
        case BossState_Background:

            mCollider = Physics::Rectangle(PixelsToMeters(Rectf({50000.0f, 50000.0f}, {0.0f, 0.0f})));
            mGraphic->SetZIndex(ZINDEX_BOSSBACKGROUND);
            mGraphic->SetFrame(2);
            break;
        case BossState_Vertical:
            mCollider = Physics::Rectangle(PixelsToMeters(Rectf({0.0f, 0.0f}, {60.0f, 400.0f})));
            mGraphic->SetZIndex(ZINDEX_ENEMY);
            mGraphic->SetFrame(0);
            break;
        case BossState_Horizontal:
            mCollider = Physics::Rectangle(PixelsToMeters(Rectf({0.0f, 0.0f}, {400.0f, 60.0f})));
            mGraphic->SetZIndex(ZINDEX_ENEMY);
            mGraphic->SetFrame(1);
            break;
        }
    }
}