#ifndef HDEF_ENEMY
#define HDEF_ENEMY

#include "Graphics/SpriteBase.h"

#include <memory>

#include "Event/EventBus.h"
#include "Physics/Shapes.h"

namespace Mochi::Graphics
{
    class TextureFactory;
}
namespace Mochi::Shooter
{
    class Enemy : public Mochi::Graphics::SpriteBase
    {
    protected:
        int mHealth;
        unsigned int mPoints;

        std::shared_ptr<Event::EventBus> mEventBus;
        Physics::Rectangle mCollider;

        void Die();

    public:
        Enemy(std::shared_ptr<Event::EventBus> eventBus, std::shared_ptr<Graphics::TextureFactory> textureFactory);
        ~Enemy();
        void ReceiveDamage(const int &damage);
        bool CheckCollision(const Physics::Shape &shape) const;
    };
}

#endif