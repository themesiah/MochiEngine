#ifndef HDEF_PLAYERMOVEMENTSYSTEM
#define HDEF_PLAYERMOVEMENTSYSTEM

#include "ECS/IECSSystem.h"
#include "ECS/Events/ECSCollisionEvent.h"

namespace Mochi::Platformer
{
    class PlayerMovementSystem : public ECS::IECSSystem
    {
    public:
        PlayerMovementSystem(entt::registry &registry, entt::dispatcher &dispatcher);
        ~PlayerMovementSystem();
        virtual void Update(const float &dt) override;
        virtual int GetPriority() const override;

    private:
        void OnCollision(const ECS::CollisionEvent &e);
    };
}

#endif