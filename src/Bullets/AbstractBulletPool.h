#ifndef HDEF_ABSTRACTBULLETPOOL
#define HDEF_ABSTRACTBULLETPOOL

#include <vector>
#include <memory>
#include <span>

#include "Types/Types.hpp"
#include "Graphics/RenderCommand.h"
#include "Physics/Shapes.h"

namespace Mochi::Graphics
{
    class SpriteBase;
}
namespace Mochi::Shooter
{
    class AbstractBulletPool
    {
    private:
        std::shared_ptr<Graphics::SpriteBase> mSprite;
        float mLifetime;
        size_t mActiveCount;

    protected:
        std::vector<Vector2f> mBulletPositions;
        std::vector<bool> mBulletActives;
        std::vector<float> mBulletTimers;

        std::unique_ptr<Physics::Shape> mBulletShape;
        virtual void OnBulletAdded(const size_t &index) {};

    public:
        AbstractBulletPool(std::shared_ptr<Graphics::SpriteBase> sprite, const int &capacity, const float &lifetime, const Physics::Shape &shape);
        virtual ~AbstractBulletPool();
        void Update(const float &dt);
        virtual void OnUpdate(const float &dt) = 0;
        std::vector<Graphics::RenderCommand> GetRenderData() const;
        void ReleaseAllBullets();
        bool AddBullet(const Vector2f &pos);
        size_t GetBulletCount() const;
        std::vector<int> CheckAgainst(const Physics::Shape &shape) const;
        void ReleaseBullet(const int &index);
        Graphics::SpriteBase *GetRenderable() const { return mSprite.get(); }

        std::vector<Vector2f> GetPositions() const
        {
            std::vector<Vector2f> positions;
            for (size_t i = 0; i < mBulletActives.size(); ++i)
            {
                if (mBulletActives[i])
                    positions.push_back(mBulletPositions[i]);
            }
            return positions;
        }
    };
}

#endif