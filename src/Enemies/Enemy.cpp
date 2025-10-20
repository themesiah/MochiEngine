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
        : Graphics::Spritesheet(animationFactory, textureFactory, ENEMY_ANIMATION_PATH, 0), mHealth(10), mPoints(40), mEventBus(eventBus),
          mCollider(PixelsToMeters(Rectf({0.0f, 0.0f}, {32.0f, 32.0f}))), mLastPosition({0.0f, 0.0f}), mTilt(0.0f), mTiltSpeed(ENEMY_TILT_SPEED)
    {
        SetZIndex(ZINDEX_ENEMY);
        SetScale(2.0f);
    }

    Enemy::~Enemy()
    {
    }

    bool Enemy::ReceiveDamage(const int &damage)
    {
        if (IsDead())
            return false; // Already dead, don't trigger things again. Probably marked for destruction already
        mHealth -= damage;
        if (mHealth <= 0)
        {
            Die();
            return true;
        }
        return false;
    }

    Physics::Rectangle Enemy::GetCollider() const
    {
        auto collider = mCollider;
        collider.Position = GetPosition();
        return collider;
    }

    void Enemy::Die()
    {
        mEventBus->Publish<EnemyDestroyedEvent>({mPoints, this});
    }

    bool Enemy::IsDead() const
    {
        return mHealth <= 0;
    }

    void Enemy::Update(const float &dt)
    {
        Graphics::Spritesheet::Update(dt);

        auto delta = GetPosition() - mLastPosition;
        float tiltDirection = 0.0f;
        if (delta.y > 0.0f)
            tiltDirection = 1.0f;
        else if (delta.y < 0.0f)
            tiltDirection = -1.0f;
        mTilt = Math::MoveTowards(mTilt, tiltDirection, dt, mTiltSpeed);

        if (mTilt == 0)
        {
            SetFrame(0);
        }
        else if (mTilt == 1.0f)
        {
            SetFrame(4);
        }
        else if (mTilt == -1.0f)
        {
            SetFrame(2);
        }
        else if (mTilt > 0.0f)
        {
            SetFrame(3);
        }
        else if (mTilt < 0.0f)
        {
            SetFrame(1);
        }

        mLastPosition = GetPosition();
    }
}