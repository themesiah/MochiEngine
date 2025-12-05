#include "Enemy.h"

#include "Graphics/Spritesheet.h"
#include "Graphics/AbstractTextureFactory.h"
#include "Graphics/IAnimationFactory.h"

#include "../ShooterEvents.h"
#include "../Utils/Conversion.hpp"
#include "../ZIndexEnum.h"
#include "Utils/MathUtils.h"

namespace Mochi::Shooter
{
    inline const std::string ENEMY_ANIMATION_PATH = "Enemy1.json";
    inline const float ENEMY_TILT_SPEED = 5.0f;

    Enemy::Enemy(Event::EventBus *eventBus, Graphics::AbstractTextureFactory *textureFactory, Graphics::IAnimationFactory *animationFactory)
        : AbstractEnemy(eventBus, 10, 40), mTilt(0.0f), mTiltSpeed(ENEMY_TILT_SPEED)
    {
        mGraphic = std::make_unique<Graphics::Spritesheet>(animationFactory, textureFactory, ENEMY_ANIMATION_PATH, 0);
        mGraphic->SetTransform(mTransform);
        mGraphic->SetZIndex(ZINDEX_ENEMY);
        mTransform->SetScale(2.0f);
        mCollider = Physics::Rectangle(PixelsToMeters(Rectf({0.0f, 0.0f}, {32.0f, 32.0f})));
    }

    Enemy::~Enemy()
    {
    }

    void Enemy::Update(const float &dt)
    {
        AbstractEnemy::Update(dt);

        auto delta = mTransform->GetPosition() - mLastPosition;
        float tiltDirection = 0.0f;
        if (delta.y > 0.0f)
            tiltDirection = 1.0f;
        else if (delta.y < 0.0f)
            tiltDirection = -1.0f;
        mTilt = Math::MoveTowards(mTilt, tiltDirection, dt, mTiltSpeed);

        if (mTilt == 0)
        {
            mGraphic->SetFrame(0);
        }
        else if (mTilt == 1.0f)
        {
            mGraphic->SetFrame(4);
        }
        else if (mTilt == -1.0f)
        {
            mGraphic->SetFrame(2);
        }
        else if (mTilt > 0.0f)
        {
            mGraphic->SetFrame(3);
        }
        else if (mTilt < 0.0f)
        {
            mGraphic->SetFrame(1);
        }

        mLastPosition = mTransform->GetPosition();
    }
}