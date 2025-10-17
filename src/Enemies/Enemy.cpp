#include "Enemy.h"

#include "Graphics/SpriteBase.h"
#include "Graphics/AbstractTextureFactory.h"

#include "../ShooterEvents.h"
#include "../Utils/Conversion.hpp"
#include "../ZIndexEnum.h"

namespace Mochi::Shooter
{
    inline const std::string ENEMY_SPRITE_PATH = "Enemy1.png";

    Enemy::Enemy(Event::EventBus *eventBus, Graphics::AbstractTextureFactory *textureFactory)
        : Graphics::SpriteBase(textureFactory, ENEMY_SPRITE_PATH), mHealth(10), mPoints(40), mEventBus(eventBus), mCollider(PixelsToMeters(Rectf({0.0f, 0.0f}, {32.0f, 32.0f})))
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
}