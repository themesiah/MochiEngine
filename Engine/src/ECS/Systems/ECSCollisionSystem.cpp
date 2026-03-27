#include "ECSCollisionSystem.h"

#include <variant>

#include "../Components/ECSTransform.h"
#include "../Components/ECSCollider.h"
#include "../Events/ECSCollisionEvent.h"

#include "../../Physics/Shapes.h"
#include "../../Utils/Logger.h"

/*

class TriggerSystem
{
public:

    TriggerSystem(entt::dispatcher& dispatcher)
    {
        dispatcher.sink<CollisionEvent>().connect<&TriggerSystem::OnCollision>(this);
    }

    void OnCollision(const CollisionEvent& e)
    {
        if(IsTrigger(e.a) || IsTrigger(e.b))
        {
            ActivateTrigger(e.a, e.b);
        }
    }
};
*/

namespace Mochi::ECS
{
    CollisionSystem::CollisionSystem(entt::registry &registry, entt::dispatcher &dispatcher) : IECSSystem(registry, dispatcher)
    {
    }

    CollisionSystem::~CollisionSystem()
    {
    }

    void CollisionSystem::Update(const float &dt)
    {
        auto view = mRegistry.view<const TransformComponent, ColliderComponent>();

        view.each([view, this](entt::entity entity, const TransformComponent &t, ColliderComponent &c)
                  { view.each([&](entt::entity entity2, const TransformComponent &t2, ColliderComponent &c2)
                              {
                if (entity != entity2 && (c.CollisionLayerMask & c2.Layer) == c2.Layer && CheckCollision(c, t, c2, t2))
                {
                    mDispatcher.enqueue<CollisionEvent>(entity, entity2, c2.Trigger, (t2.Position - t.Position).Normalized(), c2.Layer);
                } }); });
    }

    int CollisionSystem::GetPriority() const
    {
        return 2;
    }

    bool CollisionSystem::CheckCollision(ColliderComponent &c1, const TransformComponent &t1, ColliderComponent &c2, const TransformComponent &t2)
    {
        ShapeVariant a2 = c1.Shape;
        ShapeVariant b2 = c2.Shape;
        bool collision = false;
        std::visit(
            [&](auto &shapeA, auto &shapeB)
            {
                shapeA.Position += t1.Position;
                shapeB.Position += t2.Position;
                collision = shapeA.Collides(shapeB);
            },
            a2, b2);

        return collision;
    }
}