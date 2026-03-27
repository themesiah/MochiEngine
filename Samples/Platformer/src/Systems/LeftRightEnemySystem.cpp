#include "LeftRightEnemySystem.h"

#include "../Components/LeftRightComponent.h"
#include "ECS/Components/CharacterController.hpp"

#include "Utils/MathUtils.h"

namespace Mochi::Platformer
{

    LeftRightEnemySystem::LeftRightEnemySystem(entt::registry &registry, entt::dispatcher &dispatcher)
        : ECS::IECSSystem(registry, dispatcher)
    {
        mDispatcher.sink<ECS::CollisionEvent>().connect<&LeftRightEnemySystem::OnCollision>(this);
    }

    LeftRightEnemySystem::~LeftRightEnemySystem()
    {
        mDispatcher.sink<ECS::CollisionEvent>().disconnect<&LeftRightEnemySystem::OnCollision>(this);
    }

    void LeftRightEnemySystem::Update(const float &dt)
    {
        auto view = mRegistry.view<ECS::CharacterController, LeftRightComponent, const ECS::TransformComponent>();

        view.each([&](auto &cc, auto &lrc, const auto &tc)
                  {
                      if (ECS::ECSWorld::Raycast(mRegistry, tc.Position, tc.Position + Vector2f::Right * lrc.Direction * lrc.RaycastLength, lrc.LayerMaskCheck, false).size() != 0)
                      {
                          lrc.Direction *= -1.0f;
                      }

                      cc.Move(Vector2f::Right * lrc.Direction); });
    }

    int LeftRightEnemySystem::GetPriority() const
    {
        return 10;
    }

    void LeftRightEnemySystem::OnCollision(const ECS::CollisionEvent &e)
    { /*
         // Check if collided entity had LeftRightComponent and collided with terrain
         LeftRightComponent *lrc = mRegistry.try_get<LeftRightComponent>(e.Entity);
         if (lrc && (lrc->LayerMaskCheck & e.CollisionLayer) == e.CollisionLayer && Math::Abs(e.CollisionNormal.x) > Math::Abs(e.CollisionNormal.y))
         {
             LOG_INFO(std::format("Collided with normal x {}", e.CollisionNormal.x));
             lrc->Direction *= -1.0f;
         }*/
    }
}