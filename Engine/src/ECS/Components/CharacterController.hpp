#ifndef HDEF_CHARACTERCONTROLLER
#define HDEF_CHARACTERCONTROLLER

#include <entt/entt.hpp>

#include "../ECSWorld.h"
#include "../Events/ECSCollisionEvent.h"
#include "ECSTransform.h"
#include "../../Types/Types.hpp"
#include "../../Utils/MathUtils.h"
#include "../../Utils/Logger.h"

namespace Mochi::ECS
{
    const float PENETRATION_SEPARATION = 0.05f;
    const float GROUNDED_CHECK_DISTANCE = 0.1f;
    const float MAX_COYOTE_TIME = 0.2f;

    /// @brief This class exemplifies how a CharacterController could be done for a simple 2D platformer using **MochiEngine**.
    /// This might not be enough for an actual production environment, but has the minimal needs to understand the pattern behind character controllers
    /// and prove that the engine is prepared to run them.
    class CharacterController
    {
    private:
        Vector2f mVelocity;
        Vector2f mCurrentDirection;
        bool mGrounded;
        bool mJumped;
        float mCoyoteTime;

    public:
        float MaxSpeed;
        float Acceleration;
        float DecelerationRate;
        float Gravity;
        float MaxFallSpeed;
        float MaxPenetration;
        bool AirControl;
        uint32_t LayerMask;

        CharacterController(float maxSpeed, float acceleration, float decelerationRate, float gravity, float maxFallSpeed, float maxPenetration, bool airControl, uint32_t layerMask)
            : MaxSpeed(maxSpeed), Acceleration(acceleration), DecelerationRate(decelerationRate), Gravity(gravity), MaxFallSpeed(maxFallSpeed),
              MaxPenetration(maxPenetration), AirControl(airControl), LayerMask(layerMask)
        {
            mVelocity = Vector2f::Zero;
            mCurrentDirection = Vector2f::Zero;
            mGrounded = false;
            mJumped = false;
            mCoyoteTime = 0.0f;
        }

        /// @brief The movement to apply this frame to the entity. This can be called multiple times, but only last one will be considered. The
        /// movement will be normalized, so any vector2f is fine here.
        /// @param direction The direction of the movement. In most cases, a movement with only the X axis would be enough, unless there are aerial entities.
        inline void Move(const Vector2f &direction)
        {
            mCurrentDirection = direction.Normalized();
        }

        /// @brief Makes the character controller jump with an instant force, that will decrease over time thanks to the gravity, if any.
        /// A character can't jump if it has already jumped. Also can't jump if too much time has passed since being grounded (coyote time).
        /// @param force The instant Y force to jump with.
        inline void Jump(float force, bool reset)
        {
            if (!reset && (mJumped || mCoyoteTime > MAX_COYOTE_TIME))
                return;
            mVelocity.y = force;
            mGrounded = false;
            mJumped = true;
        }

        /// @brief Update the entity with the last velocity, its grounded status and whatever movement has been applied this frame.
        /// This method has to be called ONCE per frame and only once. It modifies the TransformComponent of the entity with the new
        /// values. This is already called on the default CharacterControllerSystem.
        /// @param dt The delta time.
        /// @param entity The entity with the character controller.
        /// @param registry The ecs registry.
        inline void Update(const float &dt, const EntityType &entity, entt::registry &registry, entt::dispatcher &dispatcher)
        {
            auto t = registry.try_get<TransformComponent>(entity);
            if (!t)
            {
                throw EngineError("An entity with a CharacterController needs a TransformComponent");
            }

            // Don't do additional movement if we are in the air without air control. So, free fall.
            if (!mGrounded && !AirControl)
            {
                mCurrentDirection = Vector2f::Zero;
            }

            float lastYSpeed = mVelocity.y;
            // Decelerate only if we are not moving
            if (Math::Approx(mCurrentDirection.Distance(), 0.0f))
            {
                if (mGrounded)
                {
                    mVelocity *= (1.0f - (DecelerationRate * dt));
                    if (mVelocity.Distance() < 0.01f)
                    {
                        mVelocity = Vector2f::Zero;
                    }
                }
                else
                {
                    mVelocity.x *= (1.0f - (DecelerationRate * dt));
                    if (mVelocity.x < 0.01f)
                    {
                        mVelocity.x = 0.0f;
                    }
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
                mVelocity.y = Math::Min(lastYSpeed + Gravity * dt, MaxFallSpeed);
            }

            // Compute x and y axis separately to improve collision behaviour.
            Vector2f delta = mVelocity * dt;
            t->Position.x += delta.x;
            ComputeCollisions(entity, t, registry, Vector2fAxis::X, dispatcher);
            t->Position.y += delta.y;
            ComputeCollisions(entity, t, registry, Vector2fAxis::Y, dispatcher);

            // After all movement, check if we are grounded for the next frame.
            ComputeGrounded(entity, t, registry, dt);

            // Reset current direction to avoid continuous movement if Move is not called on a frame
            mCurrentDirection = Vector2f::Zero;
        }

        /// @brief Modifies velocity and transform->position if collides with anything that is moving towards.
        /// Velocity is reduced to 0 in the selected axis if it is colliding with that axis in the movement direction.
        /// Position is modified so it separates from the entity its colliding using a bounding box for each collider.
        /// @param entity The entity with the current character controller
        /// @param tc The valid transform component of the entity
        /// @param registry The world registry
        /// @param axis The axis we want to check (x or y)
        inline void ComputeCollisions(const EntityType &entity, TransformComponent *tc, entt::registry &registry, Vector2fAxis axis, entt::dispatcher &dispatcher)
        {
            if (Math::Approx(axis == Vector2fAxis::X ? mVelocity.x : mVelocity.y, 0.0f))
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
                        auto shapeCopy = shape;
                        shapeCopy.Position = tc->Position;
                        A = shapeCopy.GetAABB(); }, cc->Shape);

            view.each(
                [&](const auto &entity2, const auto &tc2, const auto &cc2)
                {
                    if (entity != entity2 && (LayerMask & cc2.Layer) == cc2.Layer && !collided && !cc2.Trigger)
                    {
                        std::visit([&](auto &shape2)
                                   {
                        auto shape2copy = shape2;
                        shape2copy.Position = tc2.Position;
                        Physics::AABB B = shape2copy.GetAABB();
                        if (A.Max.x > B.Min.x && A.Min.x < B.Max.x
                            && A.Max.y > B.Min.y && A.Min.y < B.Max.y)
                        {
                            if ((axis == Vector2fAxis::X ? mVelocity.x : mVelocity.y) > 0.0f)
                            {
                                penetration = axis == Vector2fAxis::X ? (A.Max.x - B.Min.x) : (A.Max.y - B.Min.y);
                            } else {
                                penetration = axis == Vector2fAxis::X ? (A.Min.x - B.Max.x) : (A.Min.y - B.Max.y);
                            }
                            collided = true;
                            dispatcher.enqueue<CollisionEvent>(entity, entity2, cc2.Trigger, -(mVelocity.Normalized()), cc2.Layer);
                        } }, cc2.Shape);
                    }
                });

            if (collided)
            {
                if (axis == Vector2fAxis::X)
                {
                    mVelocity.x = 0.0f;
                    tc->Position.x -= Math::Sign(penetration) * Math::Min(Math::Abs(penetration), MaxPenetration) + Math::Sign(penetration) * PENETRATION_SEPARATION;
                }
                else
                {
                    mVelocity.y = 0.0f;
                    tc->Position.y -= Math::Sign(penetration) * Math::Min(Math::Abs(penetration), MaxPenetration) + Math::Sign(penetration) * PENETRATION_SEPARATION;
                }
            }
        }

        /// @brief Modifies the mGrounded member variable to the value of this frame.
        /// An entity is grounded if it has floor underneath, it is not otherwise.
        /// To check that, we lower the bottom part of the bounding box and check for collisions again. A collision means that we are grounded.
        /// @param entity The entity with the character controller.
        /// @param tc The valid transform component of the entity.
        /// @param registry The world registry.
        inline void ComputeGrounded(const EntityType &entity, TransformComponent *tc, entt::registry &registry, const float &dt)
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
            mCoyoteTime += dt;
            view.each(
                [&](const auto &entity2, const auto &tc2, const auto &cc2)
                {
                    if (entity != entity2 && (LayerMask & cc2.Layer) == cc2.Layer && !mGrounded)
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
                            mCoyoteTime = 0.0f;
                            mJumped = false;
                        } }, cc2.Shape);
                    }
                });
        }
    };
}

#endif