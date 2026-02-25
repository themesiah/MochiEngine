#ifndef HDEF_ENEMY
#define HDEF_ENEMY

#include "AbstractEnemy.h"

namespace Mochi::Graphics
{
    class AbstractTextureFactory;
    class IAnimationFactory;
}
namespace Mochi::Shooter
{
    class Enemy : public AbstractEnemy
    {
    private:
        float mTilt;
        float mTiltSpeed;

    public:
        Enemy(Event::EventBus *eventBus, Graphics::AbstractTextureFactory *textureFactory, Graphics::IAnimationFactory *animationFactory);
        virtual ~Enemy();
        virtual void Update(const float &dt) override;
    };
}

#endif