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
        Mochi::Graphics::Camera *mCamera;
        Mochi::Rectf mBounds;
        std::shared_ptr<PlayerBulletPool> mBulletPool;
        float mShotDelay;
        float mShotTimer;

    public:
        Player(Mochi::Graphics::AnimationFactory *animationFactory,
               Mochi::Graphics::TextureFactory *textureFactory,
               Mochi::Graphics::Camera *camera);
        ~Player();
        virtual void Update(const float &dt, Mochi::Input::ActionManager *actionManager);
        std::shared_ptr<PlayerBulletPool> GetBulletPool() const;
        virtual void SetScale(const float &scale) override;
    };
}

#endif