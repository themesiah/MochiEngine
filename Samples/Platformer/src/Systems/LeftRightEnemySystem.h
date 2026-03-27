#ifndef HDEF_LEFTRIGHTENEMYSYSTEM
#define HDEF_LEFTRIGHTENEMYSYSTEM

#include "ECS/IECSSystem.h"
#include "ECS/Events/ECSCollisionEvent.h"

namespace Mochi::Platformer
{
    class LeftRightEnemySystem : public ECS::IECSSystem
    {
    public:
        LeftRightEnemySystem(entt::registry &registry, entt::dispatcher &dispatcher);
        virtual ~LeftRightEnemySystem();
        virtual void Update(const float &dt) override;
        virtual int GetPriority() const override;
        void OnCollision(const ECS::CollisionEvent &e);
    };
}

#endif