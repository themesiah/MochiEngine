#include "PlayerBulletPool.h"
#include "AbstractBulletPool.h"

#include "Graphics/SpriteBase.h"
#include "../ZIndexEnum.h"

#include <memory>

namespace Mochi::Shooter
{
    PlayerBulletPool::PlayerBulletPool(std::shared_ptr<Graphics::SpriteBase> sprite, const int &capacity, const float &lifetime, const float &horizontalSpeed)
        : AbstractBulletPool(sprite, capacity, lifetime), mHorizontalSpeed(horizontalSpeed)
    {
        sprite->SetZIndex(ZINDEX_PLAYERBULLET);
    }

    PlayerBulletPool::~PlayerBulletPool()
    {
    }

    void PlayerBulletPool::OnUpdate(const float &dt)
    {
        Vector2f speed(mHorizontalSpeed * dt, 0.0f);
        for (size_t i = 0; i < mBulletActives.size(); ++i)
        {
            if (mBulletActives[i])
            {
                mBulletPositions[i] += speed;
            }
        }
    }
}