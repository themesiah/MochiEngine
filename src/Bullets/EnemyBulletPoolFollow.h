#ifndef HDEF_ENEMYBULLETPOOLFOLLOW
#define HDEF_ENEMYBULLETPOOLFOLLOW

#include "AbstractBulletPool.h"

#include <memory>

namespace Mochi::Graphics
{
    class Sprite;
}
namespace Mochi::Shooter
{
    class Player;
    class EnemyBulletPoolFollow : public AbstractBulletPool
    {
    private:
        float mSpeed;
        std::weak_ptr<Player> mPlayer;
        std::vector<Vector2f> mBulletDirections;

    protected:
        virtual void OnBulletAdded(const size_t &index) override;

    public:
        EnemyBulletPoolFollow(std::shared_ptr<Graphics::SpriteBase> sprite, const int &capacity, const float &lifetime, const float &speed, std::weak_ptr<Player> player);
        ~EnemyBulletPoolFollow();
        virtual void OnUpdate(const float &dt) override;
    };
}

#endif