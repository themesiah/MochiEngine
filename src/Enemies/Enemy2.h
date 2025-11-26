#ifndef HDEF_ENEMY2
#define HDEF_ENEMY2

#include "Graphics/Spritesheet.h"

#include "Event/EventBus.h"
#include "Physics/Shapes.h"

namespace Mochi::Graphics
{
    class AbstractTextureFactory;
    class IAnimationFactory;
}
namespace Mochi::Shooter
{
    class Enemy2 : public Mochi::Graphics::Spritesheet
    {
    protected:
        int mHealth;
        unsigned int mPoints;

        Event::EventBus *mEventBus;
        Physics::Rectangle mCollider;

        Vector2f mLastPosition;
        float mTilt;
        float mTiltSpeed;

        void Die();

    public:
        Enemy(Event::EventBus *eventBus, Graphics::AbstractTextureFactory *textureFactory, Graphics::IAnimationFactory *animationFactory);
        virtual ~Enemy();
        bool ReceiveDamage(const int &damage);
        Physics::Rectangle GetCollider() const;
        virtual void Update(const float &dt) override;
        bool IsDead() const;
    };
}

#endif