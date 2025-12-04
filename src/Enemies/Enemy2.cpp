#include "Enemy2.h"

#include "Graphics/Spritesheet.h"
#include "Utils/Conversion.hpp"
#include "Utils/MathUtils.h"

#include "../ZIndexEnum.h"

namespace Mochi::Shooter
{
    inline const std::string ENEMY2_ANIMATION_PATH = "Enemy2.json";
    inline const float ENEMY_TILT_SPEED = 5.0f;

    Enemy2::Enemy2(Event::EventBus *eventBus, Graphics::AbstractTextureFactory *textureFactory, Graphics::IAnimationFactory *animationFactory)
        : AbstractEnemy(eventBus, 50, 40), mTilt(0.0f), mTiltSpeed(ENEMY_TILT_SPEED)
    {
        mGraphic = std::make_unique<Graphics::Spritesheet>(animationFactory, textureFactory, ENEMY2_ANIMATION_PATH, 0);
        mGraphic->SetZIndex(ZINDEX_ENEMY);
        mGraphic->SetScale(2.0f);
        mCollider = Physics::Rectangle(PixelsToMeters(Rectf({-12.0f, 0.0f}, {48.0f, 80.0f})));
    }

    Enemy2::~Enemy2()
    {
    }

    void Enemy2::Update(const float &dt)
    {
        AbstractEnemy::Update(dt);

        auto deltaDistance = (mGraphic->GetPosition() - mLastPosition).Distance();
        float tiltDirection = 0.0f;
        if (deltaDistance > 0.0f)
            tiltDirection = 1.0f;
        else
            tiltDirection = 0.0f;
        mTilt = Math::MoveTowards(mTilt, tiltDirection, dt, mTiltSpeed);

        if (mTilt == 0)
        {
            mGraphic->SetFrame(0);
        }
        else if (mTilt == 1.0f)
        {
            mGraphic->SetFrame(2);
        }
        else
        {
            mGraphic->SetFrame(1);
        }

        mLastPosition = mGraphic->GetPosition();
    }
}