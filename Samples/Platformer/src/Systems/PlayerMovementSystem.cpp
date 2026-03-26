#include "PlayerMovementSystem.h"

#include <entt/entt.hpp>

#include "Engine.h"
#include "Input/IActionManager.h"
#include "Utils/Logger.h"
#include "ECS/Components/ECSTransform.h"
#include "ECS/Components/CharacterController.hpp"
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
        mDispatcher.sink<ECS::CollisionEvent>().disconnect<&PlayerMovementSystem::OnCollision>(this);
    }

    void PlayerMovementSystem::Update(const float &dt)
    {
        auto &e = Engine::Get();
        Input::IActionManager *actionManager = e.GetActionManager();
        auto view = mRegistry.view<ECS::TransformComponent, const PlayerComponent, ECS::CharacterController>();
        float horizontal = actionManager->Value("Horizontal");
        bool jump = actionManager->Performed("Jump");
        view.each([horizontal, dt, jump](entt::entity entity, ECS::TransformComponent &t, const PlayerComponent &c, ECS::CharacterController &cc)
                  {
                    cc.Move(Vector2f::Right * horizontal);
                    if (jump)
                    {
                        cc.Jump(10.0f);
                    } });
    }

    int PlayerMovementSystem::GetPriority() const
    {
        return 5;
    }

    void PlayerMovementSystem::OnCollision(const ECS::CollisionEvent &e)
    {
        // LOG_INFO("Two entities collided!");
    }
}