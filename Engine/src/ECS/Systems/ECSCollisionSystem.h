#ifndef HDEF_ECSCOLLISIONSYSTEM
#define HDEF_ECSCOLLISIONSYSTEM

#include "../IECSSystem.h"
#include "../Components/ECSCollider.h"
#include "../Components/ECSTransform.h"

namespace Mochi::ECS
{
    class CollisionSystem : public IECSSystem
    {
    public:
        CollisionSystem(entt::registry &registry, entt::dispatcher &dispatcher);
        ~CollisionSystem();
        virtual void Update(const float &dt) override;
        virtual int GetPriority() const override;

    private:
        bool CheckCollision(ColliderComponent &c1, const TransformComponent &t1, ColliderComponent &c2, const TransformComponent &t2);
    };
}

#endif