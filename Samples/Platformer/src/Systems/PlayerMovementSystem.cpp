#include "PlayerMovementSystem.h"

#include <entt/entt.hpp>

#include "Engine.h"
#include "Input/IActionManager.h"
#include "Utils/Logger.h"
#include "ECS/Components/ECSTransform.h"
#include "ECS/Events/ECSCollisionEvent.h"

#include "../Components/PlayerComponent.h"

namespace Mochi::Platformer
{

    PlayerMovementSystem::PlayerMovementSystem(entt::registry &registry, entt::dispatcher &dispatcher) : ECS::IECSSystem(registry, dispatcher)
    {
        mDispatcher.sink<ECS::CollisionEvent>().connect<&PlayerMovementSystem::OnCollision>(this);
    }

    PlayerMovementSystem::~PlayerMovementSystem()
    {
    }

    void PlayerMovementSystem::Update(const float &dt)
    {
        auto &e = Engine::Get();
        Input::IActionManager *actionManager = e.GetActionManager();
        auto view = mRegistry.view<ECS::TransformComponent, const PlayerComponent>();
        Vector2f mov = actionManager->CompoundValue("Horizontal", "Vertical");
        view.each([mov, dt](entt::entity entity, ECS::TransformComponent &t, const PlayerComponent &c)
                  { t.Position = t.Position + mov * c.Speed * dt; });
    }

    int PlayerMovementSystem::GetPriority() const
    {
        return 5;
    }

    void PlayerMovementSystem::OnCollision(const ECS::CollisionEvent &e)
    {
        LOG_INFO("Two entities collided!");
    }
}