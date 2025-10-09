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
    class AnimationFactory;
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
    class Enemy;
    class GameLayer : public Layer
    {
    private:
        std::shared_ptr<Player> mPlayer;
        std::unique_ptr<PointsSystem> mPointsSystem;
        std::vector<std::unique_ptr<Enemy>> mEnemies;
        std::vector<Enemy *> mEnemiesMarkedForDestruction;
        std::vector<std::unique_ptr<Graphics::OneshotAnimation>> mVFXList;
        std::vector<Graphics::OneshotAnimation *> mVFXMarkedForDestruction;

        std::shared_ptr<Graphics::AbstractTextureFactory> mTextureFactory;
        std::shared_ptr<Graphics::AnimationFactory> mAnimationFactory;

        // Subscriptions
        Event::SubscriptionHandler mEnemyDestroyedSubscription;

    public:
        GameLayer();
        virtual ~GameLayer();
        virtual bool Update(const float &dt) override;
        virtual void Render() const override;
        virtual void GUI() const override;
#if DEBUG
        virtual void Debug() const override;
#endif
    };
}

#endif