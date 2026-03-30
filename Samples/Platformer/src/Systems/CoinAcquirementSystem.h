#ifndef HDEF_COINACQUIREMENTSYSTEM
#define HDEF_COINACQUIREMENTSYSTEM

#include "ECS/IECSSystem.h"

#include "ECS/Events/ECSCollisionEvent.h"

namespace Mochi::Platformer
{
    class CoinAcquirementSystem : public ECS::IECSSystem
    {
    public:
        CoinAcquirementSystem(entt::registry &registry, entt::dispatcher &dispatcher);
        virtual ~CoinAcquirementSystem();
        virtual void Update(const float &dt) override;
        virtual int GetPriority() const override;

    private:
        void OnCollision(const ECS::CollisionEvent &e);
    };
}

#endif