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
          mExplosionScale(1),
          mInvincible(false),
          mLastPosition({0.0f, 0.0f}),
          mCollider({}),
          mTransform(std::make_shared<Transform>())
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
            if (mInvincible)
            {
                mHealth = 1;
            }
            else
            {
                Die();
                return true;
            }
        }
        return false;
    }

    Physics::Rectangle AbstractEnemy::GetCollider() const
    {
        auto collider = mCollider;
        collider.Position = collider.Position * mTransform->Scale + mTransform->Position;
        collider.Extents = collider.Extents * mTransform->Scale;
        return collider;
    }

    void AbstractEnemy::Die()
    {
        mEventBus->Publish<EnemyDestroyedEvent>({mPoints, this, mExplosionScale});
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

    std::shared_ptr<Transform> AbstractEnemy::GetTransform() const
    {
        return mTransform;
    }

    void AbstractEnemy::SetTransform(std::shared_ptr<Transform> tr)
    {
        mTransform = tr;
    }

    void AbstractEnemy::SetHealth(const int &health)
    {
        mHealth = health;
    }

    void AbstractEnemy::SetExplosionScale(const int &explosionScale)
    {
        mExplosionScale = explosionScale;
    }

    void AbstractEnemy::SetInvincible(const bool &invincible)
    {
        mInvincible = invincible;
    }
}