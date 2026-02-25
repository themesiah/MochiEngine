#ifndef HDEF_ENEMYBULLETPOOLDIRECTION
#define HDEF_ENEMYBULLETPOOLDIRECTION

#include "AbstractBulletPool.h"

namespace Mochi::Shooter
{
    class Player;
    class EnemyBulletPoolDirection : public AbstractBulletPool
    {
    private:
        float mSpeed;
        std::weak_ptr<Player> mPlayer;
        std::vector<Vector2f> mBulletDirections;

    public:
        EnemyBulletPoolDirection(std::shared_ptr<Graphics::SpriteBase> sprite, const int &capacity, const float &lifetime, const float &speed, std::weak_ptr<Player> player);
        virtual ~EnemyBulletPoolDirection();
        virtual void OnUpdate(const float &dt) override;
        void SetBulletDirection(const int &bulletIndex, const Vector2f &direction);
    };
}

#endif