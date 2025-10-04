#ifndef HDEF_GAMELAYER
#define HDEF_GAMELAYER

#include "Layer.h"

#include <memory>

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
    class TextureFactory;
    class AnimationFactory;
    class Renderer;
}
namespace Mochi::Event
{
    class EventBus;
}
namespace Mochi::Input
{
    class ActionManager;
}
namespace Mochi::Shooter
{
    class Player;
    class PointsSystem;
    class Enemy;
    class GameLayer : public Layer
    {
    private:
        FS::PackCatalog *mCatalog;
        Scripting::ScriptingManager *mScripting;
        Graphics::Camera *mCamera;
        Event::EventBus *mEventBus;
        Graphics::GUI *mGUI;
        Input::ActionManager *mActionManager;

        std::shared_ptr<Player> mPlayer;
        std::unique_ptr<PointsSystem> mPointsSystem;
        std::shared_ptr<Enemy> mEnemy;

        std::shared_ptr<Graphics::TextureFactory> mTextureFactory;
        std::shared_ptr<Graphics::AnimationFactory> mAnimationFactory;

    public:
        GameLayer(FS::PackCatalog *, Scripting::ScriptingManager *, Graphics::Renderer *, Graphics::Camera *, Event::EventBus *, Graphics::GUI *, Input::ActionManager *);
        ~GameLayer();
        virtual bool Update(const float &dt) override;
        virtual void Render() const override;
        virtual void GUI() const override;
    };
}

#endif