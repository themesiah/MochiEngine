#ifndef HDEF_PLAYERBULLETPOOL
#define HDEF_PLAYERBULLETPOOL

#include "AbstractBulletPool.h"

namespace Mochi::Graphics
{
    class Sprite;
}
namespace Mochi::Shooter
{
    class PlayerBulletPool : public AbstractBulletPool
    {
    private:
        float mHorizontalSpeed;

    public:
        PlayerBulletPool(std::shared_ptr<Graphics::SpriteBase> sprite, const int &capacity, const float &lifetime, const float &horizontalSpeed);
        ~PlayerBulletPool();
        virtual void OnUpdate(const float &dt) override;
    };
}

#endif