#ifndef HDEF_BREAKABLESYSTEM
#define HDEF_BREAKABLESYSTEM

#include "ECS/IECSSystem.h"

#include "ECS/Events/ECSCollisionEvent.h"

namespace Mochi::Platformer
{
    class BreakableSystem : public ECS::IECSSystem
    {
    public:
        BreakableSystem(entt::registry &registry, entt::dispatcher &dispatcher);
        virtual ~BreakableSystem();
        virtual void Update(const float &dt) override;
        virtual int GetPriority() const override;

    private:
        void OnCollision(const ECS::CollisionEvent &e);
    };
}

#endif