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
                bool changed = false;
                if (ac.CurrentAnimation != ac.NewAnimation)
                {
                    changed = true;
                    ac.CurrentAnimation = ac.NewAnimation;
                    auto newAnim = ad.Animations.at(ac.CurrentAnimation);
                    ac.CurrentFrame = newAnim.From;
                    ac.Timer = Time::Timer(ad.Frames[ac.CurrentFrame].Duration);
                    ac.CurrentLoop = 0;
                    sc.SrcRect = ad.Frames[ac.CurrentFrame].Frame;
                }
                Graphics::FrameTag current = ad.Animations.at(ac.CurrentAnimation);
                ac.Timer.Tick(dt);
                if (ac.Timer.IsDone())
                {
                    NextFrame(ac, current);
                    sc.SrcRect = ad.Frames[ac.CurrentFrame].Frame;
                    ac.Timer = Time::Timer(ad.Frames[ac.CurrentFrame].Duration);
                }
            });
    }

    int AnimationSystem::GetPriority() const
    {
        return 2;
    }

    void AnimationSystem::NextFrame(AnimationComponent &current, const Graphics::FrameTag &tag) const
    {
        if (current.CurrentFrame == tag.To && tag.Repeat != 0 && current.CurrentLoop + 1 == tag.Repeat)
            return;
        switch (tag.Direction)
        {
        case Graphics::AnimationDirection::Forward:
            current.CurrentFrame++;
            if (current.CurrentFrame > tag.To)
            {
                current.CurrentFrame = tag.From;
                current.CurrentLoop++;
            }
            break;
        case Graphics::AnimationDirection::Backward:
            current.CurrentFrame--;
            if (current.CurrentFrame < tag.From)
            {
                current.CurrentFrame = tag.To;
                current.CurrentLoop++;
            }
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
                    current.CurrentLoop++;
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
                    current.CurrentLoop++;
                }
            }
            break;
        }
    }
}