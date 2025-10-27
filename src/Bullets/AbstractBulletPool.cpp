#include "AbstractBulletPool.h"

#include <vector>

#include "Graphics/SpriteBase.h"

namespace Mochi::Shooter
{
    AbstractBulletPool::AbstractBulletPool(std::shared_ptr<Graphics::SpriteBase> sprite, const int &capacity, const float &lifetime, const Physics::Shape &shape)
        : mSprite(sprite), mLifetime(lifetime), mActiveCount(0), mBulletShape(shape.Clone())
    {
        mBulletActives.resize(capacity);
        mBulletPositions.resize(capacity);
        mBulletTimers.resize(capacity);
        ReleaseAllBullets();
    }

    AbstractBulletPool::~AbstractBulletPool()
    {
    }

    void AbstractBulletPool::Update(const float &dt)
    {
        for (size_t i = 0; i < mBulletActives.size(); ++i)
        {
            mBulletTimers[i] += dt;
            if (mBulletActives[i] && mBulletTimers[i] > mLifetime)
            {
                mBulletActives[i] = false;
                mActiveCount--;
            }
        }
        OnUpdate(dt);
    }

    std::vector<Graphics::RenderCommand> AbstractBulletPool::GetRenderData() const
    {
        auto command = mSprite->GetRenderData();
        std::vector<Graphics::RenderCommand> commands;
        for (size_t i = 0; i < mBulletActives.size(); ++i)
        {
            if (mBulletActives[i])
            {
                Graphics::RenderCommand newCommand = command[0];
                newCommand.destRect.SetPosition(mBulletPositions[i]);
                commands.push_back(newCommand);
            }
        }
        return commands;
    }

    void AbstractBulletPool::ReleaseAllBullets()
    {
        for (size_t i = 0; i < mBulletActives.size(); ++i)
        {
            mBulletActives[i] = false;
        }
        mActiveCount = 0;
    }

    bool AbstractBulletPool::AddBullet(const Vector2f &pos)
    {
        for (size_t i = 0; i < mBulletActives.size(); ++i)
        {
            if (!mBulletActives[i])
            {
                mBulletActives[i] = true;
                mBulletPositions[i] = pos;
                mBulletTimers[i] = 0.0f;
                mActiveCount++;
                return true;
            }
        }
        return false;
    }

    size_t AbstractBulletPool::GetBulletCount() const
    {
        return mActiveCount;
    }

    std::vector<int> AbstractBulletPool::CheckAgainst(const Physics::Shape &shape) const
    {
        std::vector<int> collidedBullets;
        for (size_t i = 0; i < mBulletActives.size(); ++i)
        {
            if (mBulletActives[i])
            {
                mBulletShape->Position = mBulletPositions[i];
                if (mBulletShape->IsColliding(shape))
                    collidedBullets.push_back((int)i);
            }
        }
        return collidedBullets;
    }

    void AbstractBulletPool::ReleaseBullet(const int &index)
    {
        mBulletActives[index] = false;
    }
}