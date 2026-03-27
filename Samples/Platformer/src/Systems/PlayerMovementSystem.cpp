#include "PlayerMovementSystem.h"

#include <entt/entt.hpp>

#include "Engine.h"
#include "Input/IActionManager.h"
#include "Utils/Logger.h"
#include "Utils/MathUtils.h"
#include "ECS/Components/ECSTransform.h"
#include "ECS/Components/CharacterController.hpp"
#include "ECS/Events/ECSCollisionEvent.h"

#include "../Components/PlayerComponent.h"
#include "../Components/EnemyComponent.h"

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
                        cc.Jump(10.0f, false);
                    } });
    }

    int PlayerMovementSystem::GetPriority() const
    {
        return 5;
    }

    void PlayerMovementSystem::OnCollision(const ECS::CollisionEvent &e)
    {
        if (!mRegistry.any_of<EnemyComponent>(e.Other) || !mRegistry.any_of<PlayerComponent>(e.Entity))
            return;

        if (e.CollisionNormal.y >= 0.9f)
        {
            LOG_INFO(std::format("Enemy dead with normal x {} and y {}!", e.CollisionNormal.x, e.CollisionNormal.y));
            mRegistry.destroy(e.Other);
            auto &cc = mRegistry.get<ECS::CharacterController>(e.Entity);

            auto &e = Engine::Get();
            Input::IActionManager *actionManager = e.GetActionManager();
            bool jump = actionManager->Performed("JumpStay");
            cc.Jump(jump ? 10.0f : 5.0f, true);
        }
        else
        {
            LOG_INFO("Player should be dead here!");
            mRegistry.destroy(e.Entity);
        }
    }
}