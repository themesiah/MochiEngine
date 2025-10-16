#include "AnimatedSprite.h"

#include <filesystem>
#include <SDL3_image/SDL_image.h>
#include <format>
#include <nlohmann/json.hpp>
#include <vector>

#include "../Packer/PackCatalog.h"
#include "../Utils/Assert.h"
#include "../Utils/Logger.h"
#include "AbstractTextureFactory.h"
#include "IAnimationFactory.h"

#include "../Input/ActionManager.h"

namespace Mochi::Graphics
{
    AnimatedSprite::AnimatedSprite(
        IAnimationFactory *animationFactory,
        AbstractTextureFactory *textureFactory,
        const std::string &animationPath,
        const std::string &mainAnimation)
        : Spritesheet(animationFactory, textureFactory, animationPath, 0),
          mPlaying(true),
          mTimer(0.0f),
          mLoops(0)
    {
        ASSERT(std::format("Animated sprite does not have the default animation \"{}\"", mainAnimation), mAnimationsData->Animations.find(mainAnimation) != mAnimationsData->Animations.end());
        PlayAnimation(mainAnimation);
    }

    AnimatedSprite::~AnimatedSprite()
    {
    }

    void AnimatedSprite::Update(const float &dt)
    {
        if (!mPlaying)
            return;
        mTimer += dt;
        int currentFrame = GetFrame();
        int lastFrame = currentFrame;
        FrameTag frameTag = mAnimationsData->Animations[mCurrentAnimation];
        FrameData currentFrameData = mAnimationsData->Frames[currentFrame];

        if (mTimer >= currentFrameData.Duration && (frameTag.Repeat == 0 || mLoops < frameTag.Repeat))
        {
            bool looped = false;
            switch (frameTag.Direction)
            {
            case AnimationDirection::Forward:
                currentFrame++;
                if (currentFrame > frameTag.To)
                {
                    currentFrame = frameTag.From;
                    looped = true;
                }
                break;
            case AnimationDirection::Backward:
                currentFrame--;
                if (currentFrame < frameTag.From)
                {
                    currentFrame = frameTag.To;
                    looped = true;
                }
                break;
            case AnimationDirection::Pingpong:
                if (mForward)
                    currentFrame++;
                else
                    currentFrame--;
                if (currentFrame == frameTag.To || currentFrame == frameTag.From)
                {
                    mForward = !mForward;
                    looped = true;
                }
                break;
            case AnimationDirection::BackwardPingPong:
                if (mForward)
                    currentFrame++;
                else
                    currentFrame--;
                if (currentFrame == frameTag.To || currentFrame == frameTag.From)
                {
                    mForward = !mForward;
                    looped = true;
                }
                break;
            }

            mTimer = mTimer - currentFrameData.Duration;
            if (looped && frameTag.Repeat != 0)
            {
                mLoops++;
                if (mLoops >= frameTag.Repeat)
                {
                    currentFrame = lastFrame;
                }
            }
        }

        if (lastFrame != currentFrame)
        {
            SetFrame(currentFrame);
        }
    }

    void AnimatedSprite::PlayAnimation(const std::string &animationName)
    {
        ASSERT(std::format("Animated sprite does not have the animation \"{}\"", animationName), mAnimationsData->Animations.find(animationName) != mAnimationsData->Animations.end());

        mCurrentAnimation = animationName;
        FrameTag frameTag = mAnimationsData->Animations[mCurrentAnimation];
        mTimer = 0.0f;
        SetFrame(frameTag.From);

        mForward = true;
        if (frameTag.Direction == AnimationDirection::Backward || frameTag.Direction == AnimationDirection::BackwardPingPong)
        {
            mForward = false;
        }
        mLoops = 0;
        mPlaying = true;
    }

    void AnimatedSprite::StopAnimation()
    {
        mPlaying = false;
    }

    std::string AnimatedSprite::GetCurrentAnimation() const
    {
        return mCurrentAnimation;
    }
}
