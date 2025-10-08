#ifndef HDEF_ENEMY
#define HDEF_ENEMY

#include "Graphics/SpriteBase.h"

#include "Event/EventBus.h"
#include "Physics/Shapes.h"

namespace Mochi::Graphics
{
    class AbstractTextureFactory;
}
namespace Mochi::Shooter
{
    class Enemy : public Mochi::Graphics::SpriteBase
    {
    protected:
        int mHealth;
        unsigned int mPoints;

        Event::EventBus *mEventBus;
        Physics::Rectangle mCollider;

        void Die();

    public:
        Enemy(Event::EventBus *eventBus, Graphics::AbstractTextureFactory *textureFactory);
        ~Enemy();
        bool ReceiveDamage(const int &damage);
        Physics::Rectangle GetCollider() const;
    };
}

#endif