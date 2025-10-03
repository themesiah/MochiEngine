#ifndef HDEF_PLAYER
#define HDEF_PLAYER

#include <memory>
#include "Graphics/RenderCommand.h"
#include "Graphics/Spritesheet.h"
#include "Types/Types.hpp"

namespace Mochi::Input
{
    class ActionManager;
}
namespace Mochi::Graphics
{
    class Camera;
}
namespace Mochi::Shooter
{
    class PlayerBulletPool;
    class Player : public Mochi::Graphics::Spritesheet
    {
    private:
        float mSpeed;
        float mTilt;
        float mTiltSpeed;
        std::shared_ptr<Mochi::Graphics::Camera> mCamera;
        Mochi::Rectf mBounds;
        std::shared_ptr<PlayerBulletPool> mBulletPool;
        float mShotDelay;
        float mShotTimer;

    public:
        Player(std::shared_ptr<Mochi::Graphics::AnimationFactory> animationFactory,
               std::shared_ptr<Mochi::Graphics::TextureFactory> textureFactory,
               std::shared_ptr<Mochi::Graphics::Camera> camera);
        ~Player();
        virtual void Update(const float &dt, Mochi::Input::ActionManager *actionManager);
        std::shared_ptr<PlayerBulletPool> GetBulletPool() const;
    };
}

#endif