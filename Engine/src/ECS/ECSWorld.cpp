#include "ECSWorld.h"

#include <algorithm>
#include <variant>
#include <vector>

#include "../Utils/Logger.h"
#include "Systems/ECSRenderSystem.h"
#include "Systems/ECSAnimationSystem.h"
#include "Systems/ECSCollisionSystem.h"
#include "Systems/ECSCharacterControllerSystem.h"

namespace Mochi::ECS
{
    ECSWorld::ECSWorld() : mRegistry(), mDispatcher(), mSystems()
    {
        RegisterSystem<RenderSystem>();
        RegisterSystem<AnimationSystem>();
        RegisterSystem<CollisionSystem>();
        RegisterSystem<CharacterControllerSystem>(*this);
    }

    ECSWorld::~ECSWorld()
    {
    }

    EntityType ECSWorld::CreateEntity()
    {
        return mRegistry.create();
    }

    void ECSWorld::Update(const float &dt)
    {
        for (auto &s : mSystems)
        {
            s->Update(dt);
        }
        mDispatcher.update();
    }

    std::vector<RaycastHit> ECSWorld::Raycast(entt::registry &registry, Vector2f startingPosition, Vector2f direction, float distance, uint32_t layerMask, bool hitTriggers)
    {
        return Raycast(registry, startingPosition, startingPosition + direction.Normalized() * distance, layerMask, hitTriggers);
    }

    std::vector<RaycastHit> ECSWorld::Raycast(entt::registry &registry, Vector2f startingPosition, Vector2f endPosition, uint32_t layerMask, bool hitTriggers)
    {
        Physics::Line l{startingPosition, endPosition};
        std::vector<RaycastHit> hits;

        auto view = registry.view<const ColliderComponent, const TransformComponent>();

        view.each([&](entt::entity entity, const ColliderComponent &c, const TransformComponent &t)
                  {
            if ((layerMask & c.Layer) == c.Layer)
            {
                auto c2 = c;
                std::visit([&](auto &shape)
                {
                    shape.Position = t.Position;
                    if (l.Collides(shape))
                    {
                        hits.push_back(RaycastHit{entity, c});
                    } }, c2.Shape);
            } });

        return hits;
    }
}