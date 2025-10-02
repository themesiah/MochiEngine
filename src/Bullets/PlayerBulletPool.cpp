#include "PlayerBulletPool.h"
#include "AbstractBulletPool.h"

#include "Graphics/SpriteBase.h"
#include "../ZIndexEnum.h"
#include "Physics/Shapes.h"

#include <memory>

namespace Mochi::Shooter
{
    const Physics::Rectangle SHAPE = Physics::Rectangle({0.0f, 0.0f}, {0.3f, 0.1f});

    PlayerBulletPool::PlayerBulletPool(std::shared_ptr<Graphics::SpriteBase> sprite, const int &capacity, const float &lifetime, const float &horizontalSpeed)
        : AbstractBulletPool(sprite, capacity, lifetime, SHAPE), mHorizontalSpeed(horizontalSpeed)
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