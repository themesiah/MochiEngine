#ifndef HDEF_ENEMY2
#define HDEF_ENEMY2

#include "AbstractEnemy.h"

namespace Mochi::Graphics
{
    class AbstractTextureFactory;
    class IAnimationFactory;
}
namespace Mochi::Shooter
{
    class Enemy2 : public AbstractEnemy
    {
    private:
        float mTilt;
        float mTiltSpeed;

    public:
        Enemy2(Event::EventBus *eventBus, Graphics::AbstractTextureFactory *textureFactory, Graphics::IAnimationFactory *animationFactory);
        virtual ~Enemy2();
        virtual void Update(const float &dt) override;
    };
}

#endif