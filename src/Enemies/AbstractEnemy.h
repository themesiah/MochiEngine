#ifndef HDEF_ABSTRACTENEMY
#define HDEF_ABSTRACTENEMY

#include <memory>

#include "Event/EventBus.h"
#include "Physics/Shapes.h"

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
        Vector2f GetPosition() const;
        void SetPosition(const Vector2f &position);
        float GetScale() const;
        void SetScale(const float &scale);
    };
}

#endif