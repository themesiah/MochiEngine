#ifndef HDEF_ABSTRACTENEMY
#define HDEF_ABSTRACTENEMY

#include <memory>

#include "Event/EventBus.h"
#include "Physics/Shapes.h"
#include "Types/Transform.hpp"

namespace Mochi::Graphics
{
    class Spritesheet;
    struct RenderCommand;
}
namespace Mochi::Shooter
{
    class AbstractEnemy
    {
    protected:
        int mHealth;
        unsigned int mPoints;
        std::shared_ptr<Transform> mTransform;

        Event::EventBus *mEventBus;
        Physics::Rectangle mCollider;

        Vector2f mLastPosition;

        void Die();
        std::unique_ptr<Graphics::Spritesheet> mGraphic;

    public:
        AbstractEnemy(Event::EventBus *eventBus, const int &health, const unsigned int &points);
        virtual ~AbstractEnemy();
        bool ReceiveDamage(const int &damage);
        Physics::Rectangle GetCollider() const;
        bool IsDead() const;
        virtual void Update(const float &dt);
        virtual std::vector<Graphics::RenderCommand> GetRenderData() const;
        std::shared_ptr<Transform> GetTransform() const;
        void SetTransform(std::shared_ptr<Transform> tr);
        void SetHealth(const int &health);
    };
}

#endif