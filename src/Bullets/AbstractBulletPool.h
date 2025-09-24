#ifndef HDEF_ABSTRACTBULLETPOOL
#define HDEF_ABSTRACTBULLETPOOL

#include <vector>
#include <memory>

#include "Types/Types.hpp"
#include "Graphics/RenderCommand.h"

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

    public:
        AbstractBulletPool(std::shared_ptr<Graphics::SpriteBase> sprite, const int &capacity, const float &lifetime);
        ~AbstractBulletPool();
        void Update(const float &dt);
        virtual void OnUpdate(const float &dt) = 0;
        std::vector<Graphics::RenderCommand> GetRenderData() const;
        void ReleaseAllBullets();
        bool AddBullet(const Vector2f &pos);
        size_t GetBulletCount() const;
    };
}

#endif