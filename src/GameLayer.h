#ifndef HDEF_GAMELAYER
#define HDEF_GAMELAYER

#include "Layer.h"

#include <memory>

#include "Event/EventBus.h"

namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Scripting
{
    class ScriptingManager;
}
namespace Mochi::Graphics
{
    class Camera;
    class GUI;
    class AbstractTextureFactory;
    class IAnimationFactory;
    class Renderer;
    class OneshotAnimation;
}
namespace Mochi::Input
{
    class ActionManager;
}
namespace Mochi::Audio
{
    class IAudioManager;
}
namespace Mochi::Shooter
{
    class Player;
    class PointsSystem;
    class AbstractEnemy;
    class AbstractBulletPool;
    class Background;
    class GameLayer : public Layer
    {
    protected:
        std::shared_ptr<Player> mPlayer;
        std::unique_ptr<PointsSystem> mPointsSystem;
        std::shared_ptr<Background> mBackground;
        std::vector<std::shared_ptr<AbstractEnemy>> mEnemies;
        std::vector<AbstractEnemy *> mEnemiesMarkedForDestruction;
        std::vector<std::shared_ptr<Graphics::OneshotAnimation>> mVFXList;
        std::vector<Graphics::OneshotAnimation *> mVFXMarkedForDestruction;

        std::unique_ptr<Graphics::AbstractTextureFactory> mTextureFactory;
        std::unique_ptr<Graphics::IAnimationFactory> mAnimationFactory;
        std::vector<std::unique_ptr<AbstractBulletPool>> mEnemyBulletPools;

        // Subscriptions
        Event::SubscriptionHandler mEnemyDestroyedSubscription;
        Event::SubscriptionHandler mPlayerDestroyedSubscription;

        void BindLuaTypesAndFunctions();

    public:
        GameLayer();
        virtual ~GameLayer();
        virtual void InitLayer() override;
        virtual bool Update(const float &dt) override;
        virtual void Render() const override;
        virtual void GUI() const override;
#if DEBUG
        virtual void Debug() const override;
#endif

        // Game specific
        std::shared_ptr<AbstractEnemy> CreateEnemy(const int &enemyType);
        void DeleteEnemy(std::shared_ptr<AbstractEnemy> enemyToDelete);
        int ShotBullet(const int &bulletPoolIndex, const Vector2f &bulletPosition);
        void SetBulletDirection(const int &bulletPoolIndex, const int &bulletIndex, const Vector2f &direction);
    };
}

#endif