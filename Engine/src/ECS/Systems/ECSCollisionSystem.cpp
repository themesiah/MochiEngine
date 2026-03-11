#include "ECSCollisionSystem.h"
#include "../Components/ECSTransform.h"
#include "../Components/ECSCollider.h"
#include "../Events/ECSCollisionEvent.h"

#include "../../Physics/Shapes.h"

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

        view.each([view](entt::entity entity, const TransformComponent &t, ColliderComponent &c)
                  { view.each([&](entt::entity entity2, const TransformComponent &t2, ColliderComponent &c2)
                              {
                if ((c.CollisionLayerMask & c.Layer) == c.Layer && CheckCollision(c, t, c2, t2))
                {
                    mDispatcher.enqueue<CollisionEvent>(entity, entity2, c2.Trigger);
                } }); });
    }

    int CollisionSystem::GetPriority() const
    {
        return 2;
    }

    bool CollisionSystem::CheckCollision(ColliderComponent &c1, const TransformComponent &t1, ColliderComponent &c2, const TransformComponent &t2)
    {
        switch (c1.ShapeType)
        {
        case ColliderShapeType::Point:
            c1.Point.Position = t1.Position;
            break;
        case ColliderShapeType::Line:
            c1.Line.End *= t1.Scale;
            c1.Line.Position = t1.Position;
            c1.Line.End += t1.Position;
            break;
        case ColliderShapeType::Circle:
            c1.Circle.Position = t1.Position;
            c1.Circle.Radius *= t1.Scale;
            break;
        case ColliderShapeType::Rectangle:
            c1.Rectangle.Position = t1.Position;
            c1.Rectangle.Extents *= t1.Scale;
            break;
        }
        switch (c2.ShapeType)
        {
        case ColliderShapeType::Point:
            c2.Point.Position = t2.Position;
            break;
        case ColliderShapeType::Line:
            c2.Line.End *= t2.Scale;
            c2.Line.Position = t2.Position;
            c2.Line.End += t2.Position;
            break;
        case ColliderShapeType::Circle:
            c2.Circle.Position = t2.Position;
            c2.Circle.Radius *= t2.Scale;
            break;
        case ColliderShapeType::Rectangle:
            c2.Rectangle.Position = t2.Position;
            c2.Rectangle.Extents *= t2.Scale;
            break;
        }
        return CollisionFnMatrix[(int)c1.ShapeType][(int)c2.ShapeType](c1, c2);
    }
}