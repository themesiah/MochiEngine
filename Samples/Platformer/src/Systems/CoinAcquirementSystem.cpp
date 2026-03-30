#include "CoinAcquirementSystem.h"

#include "Utils/Logger.h"
#include "ECS/Events/ECSCollisionEvent.h"

#include "../Components/CoinComponent.h"
#include "../Components/GlobalDataComponent.h"

namespace Mochi::Platformer
{
    CoinAcquirementSystem::CoinAcquirementSystem(entt::registry &registry, entt::dispatcher &dispatcher) : ECS::IECSSystem(registry, dispatcher)
    {
        mDispatcher.sink<ECS::CollisionEvent>().connect<&CoinAcquirementSystem::OnCollision>(this);
    }

    CoinAcquirementSystem::~CoinAcquirementSystem()
    {
        mDispatcher.sink<ECS::CollisionEvent>().disconnect<&CoinAcquirementSystem::OnCollision>(this);
    }

    void CoinAcquirementSystem::Update(const float &dt)
    {
    }

    int CoinAcquirementSystem::GetPriority() const
    {
        return 5;
    }

    void CoinAcquirementSystem::OnCollision(const ECS::CollisionEvent &e)
    {
        if (mRegistry.any_of<CoinComponent>(e.Other))
        {
            auto &cc = mRegistry.get<CoinComponent>(e.Other);
            auto &global = mRegistry.ctx().get<GlobalDataComponent>();
            global.Coins += cc.Points;
            mRegistry.destroy(e.Other);

            LOG_INFO("Coin acquired");
        }
    }
}