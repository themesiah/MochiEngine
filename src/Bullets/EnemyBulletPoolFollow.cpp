#include "EnemyBulletPoolFollow.h"

#include "Utils/Conversion.hpp"
#include "Physics/Shapes.h"
#include "../ZIndexEnum.h"
#include "Graphics/SpriteBase.h"

#include "../Player.h"

namespace Mochi::Shooter
{
    const Physics::Circle SHAPE = Physics::Circle({0.0f, 0.0f}, PixelsToMeters(16.0f));

    EnemyBulletPoolFollow::EnemyBulletPoolFollow(std::shared_ptr<Graphics::SpriteBase> sprite, const int &capacity, const float &lifetime, const float &speed, std::weak_ptr<Player> player)
        : AbstractBulletPool(sprite, capacity, lifetime, SHAPE), mSpeed(speed), mPlayer(player)
    {
        sprite->SetZIndex(ZINDEX_ENEMYBULLET);
        mBulletDirections.resize(capacity);
    }

    EnemyBulletPoolFollow::~EnemyBulletPoolFollow()
    {
    }

    void EnemyBulletPoolFollow::OnUpdate(const float &dt)
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

    void EnemyBulletPoolFollow::OnBulletAdded(const size_t &index)
    {
        if (std::shared_ptr<Player> player = mPlayer.lock())
        {
            mBulletDirections[index] = (player->GetPosition() - mBulletPositions[index]).Normalized();
        }
    }
}