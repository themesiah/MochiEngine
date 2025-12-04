#include "EnemyBulletPoolDirection.h"

#include "Graphics/SpriteBase.h"
#include "Utils/Assert.h"
#include "Utils/Conversion.hpp"

#include "../ZIndexEnum.h"

namespace Mochi::Shooter
{
    const Physics::Circle EnemyBulletPoolDirection_SHAPE = Physics::Circle({0.0f, 0.0f}, PixelsToMeters(16.0f));

    EnemyBulletPoolDirection::EnemyBulletPoolDirection(std::shared_ptr<Graphics::SpriteBase> sprite, const int &capacity, const float &lifetime, const float &speed, std::weak_ptr<Player> player)
        : AbstractBulletPool(sprite, capacity, lifetime, EnemyBulletPoolDirection_SHAPE), mSpeed(speed), mPlayer(player)
    {
        sprite->SetZIndex(ZINDEX_ENEMYBULLET);
        mBulletDirections.resize(capacity);
    }

    EnemyBulletPoolDirection::~EnemyBulletPoolDirection()
    {
    }

    void EnemyBulletPoolDirection::OnUpdate(const float &dt)
    {
        float movementMagnitude = mSpeed * dt;
        for (size_t i = 0; i < mBulletActives.size(); ++i)
        {
            if (mBulletActives[i])
            {
                mBulletPositions[i] += mBulletDirections[i] * movementMagnitude;
            }
        }
    }

    void EnemyBulletPoolDirection::SetBulletDirection(const int &bulletIndex, const Vector2f &direction)
    {
        ASSERT("Bullet index out of bounds", mBulletDirections.size() > bulletIndex);
        mBulletDirections[bulletIndex] = direction.Normalized();
    }
}