#include "BreakableSystem.h"

#include "ECS/Events/ECSCollisionEvent.h"
#include "Utils/Logger.h"

#include "../Components/BreakableComponent.h"

namespace Mochi::Platformer
{

    BreakableSystem::BreakableSystem(entt::registry &registry, entt::dispatcher &dispatcher) : ECS::IECSSystem(registry, dispatcher)
    {
        mDispatcher.sink<ECS::CollisionEvent>().connect<&BreakableSystem::OnCollision>(this);
    }

    BreakableSystem::~BreakableSystem()
    {
        mDispatcher.sink<ECS::CollisionEvent>().disconnect<&BreakableSystem::OnCollision>(this);
    }

    void BreakableSystem::Update(const float &dt)
    {
    }

    int BreakableSystem::GetPriority() const
    {
        return 5;
    }

    void BreakableSystem::OnCollision(const ECS::CollisionEvent &e)
    {
        if (mRegistry.any_of<BreakableComponent>(e.Other) && e.CollisionNormal.y < -0.8f)
        {
            mRegistry.destroy(e.Other);
            LOG_INFO("Block broken");
        }
    }
}