#include "PlayerMovementSystem.h"

#include <entt/entt.hpp>

#include "Engine.h"
#include "Input/IActionManager.h"
#include "Utils/Logger.h"
#include "Utils/MathUtils.h"
#include "ECS/Components/ECSTransform.h"
#include "ECS/Components/CharacterController.hpp"
#include "ECS/Components/ECSAnimation.h"
#include "ECS/Components/ECSSprite.h"
#include "ECS/Events/ECSCollisionEvent.h"
#include "ECS/Events/ECSCharacterControllerEvents.h"

#include "../Components/PlayerComponent.h"
#include "../Components/EnemyComponent.h"

namespace Mochi::Platformer
{

    PlayerMovementSystem::PlayerMovementSystem(entt::registry &registry, entt::dispatcher &dispatcher) : ECS::IECSSystem(registry, dispatcher)
    {
        mDispatcher.sink<ECS::CollisionEvent>().connect<&PlayerMovementSystem::OnCollision>(this);
        mDispatcher.sink<ECS::CCGroundedEvent>().connect<&PlayerMovementSystem::OnGrounded>(this);
        mDispatcher.sink<ECS::CCJumpEvent>().connect<&PlayerMovementSystem::OnJumped>(this);
    }

    PlayerMovementSystem::~PlayerMovementSystem()
    {
        mDispatcher.sink<ECS::CollisionEvent>().disconnect<&PlayerMovementSystem::OnCollision>(this);
        mDispatcher.sink<ECS::CCGroundedEvent>().disconnect<&PlayerMovementSystem::OnGrounded>(this);
        mDispatcher.sink<ECS::CCJumpEvent>().disconnect<&PlayerMovementSystem::OnJumped>(this);
    }

    void PlayerMovementSystem::Update(const float &dt)
    {
        auto &e = Engine::Get();
        Input::IActionManager *actionManager = e.GetActionManager();
        auto view = mRegistry.view<ECS::TransformComponent, const PlayerComponent, ECS::CharacterController, ECS::AnimationComponent, ECS::SpriteComponent>();
        float horizontal = actionManager->Value("Horizontal");
        bool jump = actionManager->Performed("Jump");
        view.each([horizontal, dt, jump, this](entt::entity entity, ECS::TransformComponent &t, const PlayerComponent &c, ECS::CharacterController &cc, ECS::AnimationComponent &ac, ECS::SpriteComponent &sc)
                  {
                    cc.Move(Vector2f::Right * horizontal);
                    if (jump)
                    {
                        cc.Jump(10.0f, false, mDispatcher, entity);
                    }
                    
                    if (!cc.IsJumping() && cc.IsGrounded())
                    {
                        if (Math::Abs(cc.GetVelocity().x) > 0.0f) 
                        {
                            ac.SetCurrentAnimation("Walk");
                        } else {
                            ac.SetCurrentAnimation("Idle");
                        }
                    }
                    if (!Math::Approx(cc.GetVelocity().x, 0.0f)) {
                        sc.Flip = cc.GetVelocity().x > 0.0f ? Graphics::RenderCommandFlipMode::None : Graphics::RenderCommandFlipMode::Horizontal;
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

            auto &engine = Engine::Get();
            Input::IActionManager *actionManager = engine.GetActionManager();
            bool jump = actionManager->Performed("JumpStay");
            cc.Jump(jump ? 10.0f : 5.0f, true, mDispatcher, e.Entity);
        }
        else
        {
            LOG_INFO("Player should be dead here!");
            mRegistry.destroy(e.Entity);
        }
    }

    void PlayerMovementSystem::OnGrounded(const ECS::CCGroundedEvent &e)
    {
        LOG_INFO("Grounded");
        if (mRegistry.all_of<PlayerComponent>(e.Entity))
        {
            auto &anim = mRegistry.get<ECS::AnimationComponent>(e.Entity);
            auto cc = mRegistry.get<ECS::CharacterController>(e.Entity);
            if (Math::Abs(cc.GetVelocity().x) > 0.0f)
            {
                anim.SetCurrentAnimation("Walk");
            }
            else
            {
                anim.SetCurrentAnimation("Idle");
            }
        }
    }

    void PlayerMovementSystem::OnJumped(const ECS::CCJumpEvent &e)
    {
        LOG_INFO("Jumped");
        if (mRegistry.all_of<PlayerComponent>(e.Entity))
        {
            auto &anim = mRegistry.get<ECS::AnimationComponent>(e.Entity);

            anim.SetCurrentAnimation("Jump");
        }
    }
}