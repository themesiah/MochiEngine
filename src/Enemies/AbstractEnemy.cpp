#include "AbstractEnemy.h"

#include "Event/EventBus.h"
#include "Graphics/Spritesheet.h"
#include "Graphics/RenderCommand.h"

#include "../ShooterEvents.h"

namespace Mochi::Shooter
{
    AbstractEnemy::AbstractEnemy(Event::EventBus *eventBus, const int &health, const unsigned int &points)
        : mEventBus(eventBus),
          mHealth(health),
          mPoints(points),
          mLastPosition({0.0f, 0.0f}),
          mCollider({})
    {
    }

    AbstractEnemy::~AbstractEnemy()
    {
    }

    bool AbstractEnemy::ReceiveDamage(const int &damage)
    {
        if (IsDead())
            return false; // Already dead, don't trigger things again. Probably marked for destruction already
        mHealth -= damage;
        if (mHealth <= 0)
        {
            Die();
            return true;
        }
        return false;
    }

    Physics::Rectangle AbstractEnemy::GetCollider() const
    {
        auto collider = mCollider;
        collider.Position = mGraphic->GetPosition();
        return collider;
    }

    void AbstractEnemy::Die()
    {
        mEventBus->Publish<EnemyDestroyedEvent>({mPoints, this});
    }

    bool AbstractEnemy::IsDead() const
    {
        return mHealth <= 0;
    }

    void AbstractEnemy::Update(const float &dt)
    {
        mGraphic->Update(dt);
    }

    std::vector<Graphics::RenderCommand> AbstractEnemy::GetRenderData() const
    {
        return mGraphic->GetRenderData();
    }

    Vector2f AbstractEnemy::GetPosition() const { return mGraphic->GetPosition(); }
    void AbstractEnemy::SetPosition(const Vector2f &position) { mGraphic->SetPosition(position); }
    float AbstractEnemy::GetScale() const { return mGraphic->GetScale(); }
    void AbstractEnemy::SetScale(const float &scale) { mGraphic->SetScale(scale); }
}