#ifndef HDEF_ECSRENDERSYSTEM
#define HDEF_ECSRENDERSYSTEM

#include "../IECSSystem.h"

namespace Mochi::ECS
{
    class RenderSystem : public IECSSystem
    {
    public:
        RenderSystem(entt::registry &registry, entt::dispatcher &dispatcher);
        virtual ~RenderSystem();
        virtual void Update(const float &dt) override;
        virtual int GetPriority() const override;
    };
}

#endif