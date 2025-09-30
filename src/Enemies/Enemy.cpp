#include "Enemy.h"

#include "Graphics/SpriteBase.h"
#include "Graphics/TextureFactory.h"

#include "../ShooterEvents.h"

namespace Mochi::Shooter
{
    inline const std::string ENEMY_SPRITE_PATH = "Enemy1.png";

    Enemy::Enemy(std::shared_ptr<Event::EventBus> eventBus, std::shared_ptr<Graphics::TextureFactory> textureFactory)
        : mHealth(10), mPoints(40), mEventBus(eventBus), mCollider({0.0f, 0.0f}, {1.0f, 1.0f}), Graphics::SpriteBase(textureFactory, ENEMY_SPRITE_PATH)
    {
    }

    Enemy::~Enemy()
    {
    }

    void Enemy::ReceiveDamage(const int &damage)
    {
        mHealth -= damage;
        if (mHealth <= 0)
        {
            Die();
        }
    }

    bool Enemy::CheckCollision(const Physics::Shape &shape) const
    {
        return false;
    }

    void Enemy::Die()
    {
        mEventBus->Publish<EnemyDestroyedEvent>({mPoints});
    }

}