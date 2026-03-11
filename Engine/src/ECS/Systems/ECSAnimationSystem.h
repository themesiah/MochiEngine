#ifndef HDEF_ECSANIMATIONSYSTEM
#define HDEF_ECSANIMATIONSYSTEM

#include "../IECSSystem.h"

#include "../../Graphics/AnimationData.h"
#include "../Components/ECSAnimation.h"

namespace Mochi::ECS
{
    class AnimationSystem : public IECSSystem
    {
    public:
        AnimationSystem(entt::registry &registry, entt::dispatcher &dispatcher);
        virtual ~AnimationSystem();
        virtual void Update(const float &dt) override;
        virtual int GetPriority() const override;

    private:
        void NextFrame(AnimationComponent &current, const Graphics::FrameTag &tag) const;
    };
}

#endif