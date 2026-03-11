#include "ECSAnimationSystem.h"

#include "../../Engine.h"
#include "../../Graphics/AnimationData.h"
#include "../Components/ECSSprite.h"
#include "../Components/ECSAnimation.h"

namespace Mochi::ECS
{
    AnimationSystem::AnimationSystem(entt::registry &registry, entt::dispatcher &dispatcher) : IECSSystem(registry, dispatcher)
    {
    }

    AnimationSystem::~AnimationSystem()
    {
    }

    void AnimationSystem::Update(const float &dt)
    {
        auto view = mRegistry.view<const Graphics::AnimationsData, AnimationComponent, SpriteComponent>();

        view.each(
            [&](const Graphics::AnimationsData &ad, AnimationComponent &ac, SpriteComponent &sc)
            {
                Graphics::FrameTag current = ad.Animations.at(ac.CurrentAnimation);
                ac.Timer.Tick(dt);
                if (ac.Timer.IsDone())
                {
                    NextFrame(ac, current);
                    sc.SrcRect = ad.Frames[ac.CurrentFrame].Frame;
                    ac.Timer = Time::Timer(ad.Frames[ac.CurrentFrame].Duration);
                }

                if (ac.Timer.GetRemainingTime() == ac.Timer.GetTime())
                {
                    ac.Timer = Time::Timer(ad.Frames[ac.CurrentFrame].Duration);
                }
            });
    }

    int AnimationSystem::GetPriority() const
    {
        return 1;
    }

    void AnimationSystem::NextFrame(AnimationComponent &current, const Graphics::FrameTag &tag) const
    {
        switch (tag.Direction)
        {
        case Graphics::AnimationDirection::Forward:
            current.CurrentFrame++;
            if (current.CurrentFrame > tag.To)
                current.CurrentFrame = tag.From;
            break;
        case Graphics::AnimationDirection::Backward:
            current.CurrentFrame--;
            if (current.CurrentFrame < tag.From)
                current.CurrentFrame = tag.To;
            break;
        case Graphics::AnimationDirection::Pingpong:
            if (current.Forward)
            {
                current.CurrentFrame++;
                if (current.CurrentFrame > tag.To)
                {
                    current.Forward = !current.Forward;
                    current.CurrentFrame--;
                    current.CurrentFrame--;
                }
            }
            else
            {
                current.CurrentFrame--;
                if (current.CurrentFrame < tag.From)
                {
                    current.Forward = !current.Forward;
                    current.CurrentFrame++;
                    current.CurrentFrame++;
                }
            }
            break;
        case Graphics::AnimationDirection::BackwardPingPong:
            if (current.Forward)
            {
                current.CurrentFrame++;
                if (current.CurrentFrame > tag.To)
                {
                    current.Forward = !current.Forward;
                    current.CurrentFrame--;
                    current.CurrentFrame--;
                }
            }
            else
            {
                current.CurrentFrame--;
                if (current.CurrentFrame < tag.From)
                {
                    current.Forward = !current.Forward;
                    current.CurrentFrame++;
                    current.CurrentFrame++;
                }
            }
            break;
        }
    }
}