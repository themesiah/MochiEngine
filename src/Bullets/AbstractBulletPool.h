#ifndef HDEF_ABSTRACTBULLETPOOL
#define HDEF_ABSTRACTBULLETPOOL

#include <vector>
#include <memory>

#include "Types/Types.hpp"
#include "Entity/IEntity.h"
#include "Entity/IUpdateable.h"
#include "Entity/IRenderable.h"

namespace Mochi::Shooter
{
    class AbstractBulletPool : public IEntity, public IUpdateable, public IRenderable
    {
    private:
        std::shared_ptr<IRenderable> mRenderable;
        float mLifetime;
        size_t mActiveCount;

    protected:
        std::vector<Vector2f> mBulletPositions;
        std::vector<bool> mBulletActives;
        std::vector<float> mBulletTimers;

    public:
        AbstractBulletPool(std::shared_ptr<IRenderable> renderable, const int &capacity, const float &lifetime);
        ~AbstractBulletPool();
        virtual void Update(const float &dt, std::shared_ptr<Input::ActionManager>) override;
        virtual void OnUpdate(const float &dt) = 0;
        virtual std::vector<Graphics::RenderCommand> GetRenderData() const override;
        void ReleaseAllBullets();
        bool AddBullet(const Vector2f &pos);
        size_t GetBulletCount() const;
    };
}

#endif