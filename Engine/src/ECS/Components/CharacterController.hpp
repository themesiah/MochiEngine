#ifndef HDEF_CHARACTERCONTROLLER
#define HDEF_CHARACTERCONTROLLER

#include <entt/entt.hpp>

#include "../ECSWorld.h"
#include "ECSTransform.h"
#include "../../Types/Types.hpp"
#include "../../Utils/MathUtils.h"
#include "../../Utils/Logger.h"

namespace Mochi::ECS
{
    const float PENETRATION_SEPARATION = 0.05f;
    const float GROUNDED_CHECK_DISTANCE = 0.1f;

    class CharacterController
    {
    private:
        Vector2f mVelocity;
        Vector2f mCurrentDirection;
        bool mGrounded;

    public:
        float MaxSpeed;
        float Acceleration;
        float DecelerationRate;
        float Gravity;
        float MaxPenetration;
        bool AirControl;

        CharacterController(float maxSpeed, float acceleration, float decelerationRate, float gravity, float maxPenetration, bool airControl)
            : MaxSpeed(maxSpeed), Acceleration(acceleration), DecelerationRate(decelerationRate), Gravity(gravity), MaxPenetration(maxPenetration),
              AirControl(airControl)
        {
            mVelocity = Vector2f::Zero;
            mCurrentDirection = Vector2f::Zero;
            mGrounded = false;
        }

        inline void Move(const Vector2f &direction)
        {
            mCurrentDirection = direction.Normalized();
        }

        inline void Jump(float force)
        {
            if (!mGrounded)
                return;
            mVelocity.y = force;
            mGrounded = false;
        }

        inline void Update(const float &dt, const EntityType &entity, ECSWorld &world, entt::registry &registry)
        {
            auto t = registry.try_get<TransformComponent>(entity);
            if (!t)
            {
                throw EngineError("An entity with a CharacterController needs a TransformComponent");
            }

            float lastYSpeed = mVelocity.y;
            // Decelerate only if we are not moving
            if (Math::Approx(mCurrentDirection.Distance(), 0.0f))
            {
                mVelocity *= (1.0f - (DecelerationRate * dt));
                if (mVelocity.Distance() < 0.01f)
                {
                    mVelocity = Vector2f::Zero;
                }
            }

            // Change velocity depending on direction and dt
            mVelocity += mCurrentDirection * Acceleration * dt;

            // Clamp
            if (Math::Abs(mVelocity.x) > MaxSpeed && (mGrounded || AirControl))
            {
                mVelocity.x = Math::Sign(mVelocity.x) * Math::Min(Math::Abs(mVelocity.x), MaxSpeed);
            }
            // Gravity overrides deceleration
            if (!mGrounded)
            {
                mVelocity.y = Math::Min(lastYSpeed + Gravity * dt, MaxSpeed);
            }

            Vector2f delta = mVelocity * dt;

            t->Position.x += delta.x;
            ComputeHorizontalCollisions(entity, t, registry);
            t->Position.y += delta.y;
            ComputeVerticalCollisions(entity, t, registry);

            ComputeGrounded(entity, t, registry);

            // Reset current direction to avoid continuous movement if Move is not called on a frame
            mCurrentDirection = Vector2f::Zero;
        }

        /// @brief Modifies velocity and transform->position if collides with anything that is moving towards.
        /// @param entity
        /// @param tc
        /// @param registry
        inline void ComputeHorizontalCollisions(const EntityType &entity, TransformComponent *tc, entt::registry &registry)
        {
            if (Math::Approx(mVelocity.x, 0.0f))
                return;
            auto view = registry.view<const TransformComponent, const ColliderComponent>();

            auto cc = registry.try_get<ColliderComponent>(entity);
            if (!cc)
                throw EngineError("An entity with a CharacterController needs a TransformComponent");
            float penetration = 0.0f;
            bool collided = false;

            Physics::AABB A{tc->Position, tc->Position};
            std::visit([&](auto &shape)
                       {
                        shape.Position = tc->Position;
                        A = shape.GetAABB(); }, cc->Shape);

            view.each(
                [&](const auto &entity2, const auto &tc2, const auto &cc2)
                {
                    if (entity != entity2 && (cc->CollisionLayerMask & cc2.Layer) == cc2.Layer && !collided)
                    {
                        std::visit([&](auto &shape2)
                                   {
                        auto shape22 = shape2;
                        shape22.Position = tc2.Position;
                        Physics::AABB B = shape22.GetAABB();
                        if (A.Max.x > B.Min.x && A.Min.x < B.Max.x
                            && A.Max.y > B.Min.y && A.Min.y < B.Max.y)
                        {
                            if (mVelocity.x > 0.0f)
                            {
                                penetration = A.Max.x - B.Min.x;
                            } else {
                                penetration = A.Min.x - B.Max.x;
                            }
                            collided = true;
                        } }, cc2.Shape);
                    }
                });

            if (collided)
            {
                mVelocity.x = 0.0f;
                tc->Position.x -= Math::Sign(penetration) * Math::Min(Math::Abs(penetration), MaxPenetration) + Math::Sign(penetration) * PENETRATION_SEPARATION;
            }
        }

        /// @brief Modifies velocity and transform->position if collides with anything that is moving towards.
        /// @param entity
        /// @param tc
        /// @param registry
        inline void ComputeVerticalCollisions(const EntityType &entity, TransformComponent *tc, entt::registry &registry)
        {
            if (Math::Approx(mVelocity.y, 0.0f))
                return;
            auto view = registry.view<const TransformComponent, const ColliderComponent>();

            auto cc = registry.try_get<ColliderComponent>(entity);
            if (!cc)
                throw EngineError("An entity with a CharacterController needs a TransformComponent");
            float penetration = 0.0f;
            bool collided = false;

            Physics::AABB A{tc->Position, tc->Position};
            std::visit([&](auto &shape)
                       {
                        shape.Position = tc->Position;
                        A = shape.GetAABB(); }, cc->Shape);

            view.each(
                [&](const auto &entity2, const auto &tc2, const auto &cc2)
                {
                    if (entity != entity2 && (cc->CollisionLayerMask & cc2.Layer) == cc2.Layer && !collided)
                    {
                        std::visit([&](auto &shape2)
                                   {
                        auto shape22 = shape2;
                        shape22.Position = tc2.Position;
                        Physics::AABB B = shape22.GetAABB();
                        if (A.Max.x > B.Min.x && A.Min.x < B.Max.x
                            && A.Max.y > B.Min.y && A.Min.y < B.Max.y)
                        {
                            if (mVelocity.y > 0.0f)
                            {
                                penetration = A.Max.y - B.Min.y;
                            } else {
                                penetration = A.Min.y - B.Max.y;
                                //mGrounded = true;
                            }
                            collided = true;
                        } }, cc2.Shape);
                    }
                });

            if (collided)
            {
                mVelocity.y = 0.0f;
                tc->Position.y -= Math::Sign(penetration) * Math::Min(Math::Abs(penetration), MaxPenetration) + Math::Sign(penetration) * PENETRATION_SEPARATION;
            }
        }

        inline void ComputeGrounded(const EntityType &entity, TransformComponent *tc, entt::registry &registry)
        {
            if (mVelocity.y > 0.0f)
                return;
            auto view = registry.view<const TransformComponent, const ColliderComponent>();

            auto cc = registry.try_get<ColliderComponent>(entity);
            if (!cc)
                throw EngineError("An entity with a CharacterController needs a TransformComponent");

            Physics::AABB A{tc->Position, tc->Position};
            std::visit([&](auto &shape)
                       {
                        auto shapeCopy = shape;
                        shapeCopy.Position = tc->Position;
                        A = shapeCopy.GetAABB();
                        A.Min.y -= GROUNDED_CHECK_DISTANCE; }, cc->Shape);

            mGrounded = false;
            view.each(
                [&](const auto &entity2, const auto &tc2, const auto &cc2)
                {
                    if (entity != entity2 && (cc->CollisionLayerMask & cc2.Layer) == cc2.Layer && !mGrounded)
                    {
                        std::visit([&](auto &shape2)
                                   {
                        auto shape22 = shape2;
                        shape22.Position = tc2.Position;
                        Physics::AABB B = shape22.GetAABB();
                        if (A.Max.x > B.Min.x && A.Min.x < B.Max.x
                            && A.Max.y > B.Min.y && A.Min.y < B.Max.y)
                        {
                            mGrounded = true;
                        } }, cc2.Shape);
                    }
                });
        }
    };
}

#endif