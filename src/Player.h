#ifndef HDEF_PLAYER
#define HDEF_PLAYER

#include <memory>
#include "Graphics/RenderCommand.h"
#include "Graphics/Spritesheet.h"
#include "Types/Types.hpp"

namespace Mochi::Input
{
    class IActionManager;
}
namespace Mochi::Graphics
{
    class Camera;
    class AbstractTextureFactory;
    class IAnimationFactory;
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
        Input::IActionManager *mActionManager;

    public:
        Player(Mochi::Graphics::IAnimationFactory *animationFactory,
               Mochi::Graphics::AbstractTextureFactory *textureFactory,
               Mochi::Graphics::Camera *camera,
               Input::IActionManager *actionManager);
        virtual ~Player();
        virtual void Update(const float &dt) override;
        std::shared_ptr<PlayerBulletPool> GetBulletPool() const;
        virtual void SetScale(const float &scale) override;
    };
}

#endif