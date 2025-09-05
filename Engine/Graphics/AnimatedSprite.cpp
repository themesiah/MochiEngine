#include "AnimatedSprite.h"

#include <filesystem>
#include <SDL3_image/SDL_image.h>
#include <format>

#include "../Packer/PackCatalog.h"
#include "json.hpp"
#include "../Utils/Assert.h"
#include "../Utils/Logger.h"
#include "TextureFactory.h"
#include "AnimationFactory.h"

#include "../Input/ActionManager.h"

using json = nlohmann::json;

namespace Mochi
{
    namespace Graphics
    {
        AnimatedSprite::AnimatedSprite(std::shared_ptr<AnimationFactory> animationFactory, std::shared_ptr<TextureFactory> textureFactory, const std::string &animationPath, const std::string &mainAnimation) : mTimer(0.0f),
                                                                                                                                                                                                                 mCurrentFrame(0),
                                                                                                                                                                                                                 mDestRect(),
                                                                                                                                                                                                                 mSrcRect()
        {
            mAnimationsData = animationFactory->GetAnimationsData(animationPath);
            ASSERT("Animations data was not loaded", mAnimationsData != nullptr);
            mTexture = textureFactory->GetTexture(mAnimationsData->TexturePath.string());
            ASSERT("Texture data was not loaded", mTexture != nullptr);

            ASSERT(std::format("Animated sprite does not have the default animation \"{}\"", mainAnimation), mAnimationsData->Animations.find(mainAnimation) != mAnimationsData->Animations.end());
            mCurrentAnimation = mAnimationsData->Animations[mainAnimation].Name;

            mSize.x = mAnimationsData->Frames[0].Frame.w;
            mSize.y = mAnimationsData->Frames[0].Frame.h;

            mScale = 5;
            mDestRect.x = 0;
            mDestRect.y = 0;
        }

        AnimatedSprite::~AnimatedSprite()
        {
        }
        RenderCommand AnimatedSprite::GetRenderData() const
        {
            RenderCommand rc;
            rc.texture = mTexture;
            rc.sourceRect = mSrcRect;
            rc.destRect = mDestRect;
            rc.zindex = 1; // TEMP
            return rc;
        }

        void AnimatedSprite::UpdateAnimation(const float &dt)
        {
            mTimer += dt;
            FrameTag frameTag = mAnimationsData->Animations[mCurrentAnimation];
            FrameData currentFrame = mAnimationsData->Frames[mCurrentFrame];

            if (mTimer >= currentFrame.Duration)
            {
                // If direction forward. Only this for now
                {
                    mCurrentFrame++;
                    if (mCurrentFrame > frameTag.To)
                    {
                        mCurrentFrame = frameTag.From;
                    }
                }

                mTimer = mTimer - currentFrame.Duration;
            }

            mSrcRect = currentFrame.Frame;

            mDestRect.w = currentFrame.Frame.w * mScale;
            mDestRect.h = currentFrame.Frame.h * mScale;
        }

        void AnimatedSprite::PlayAnimation(const std::string &animationName)
        {
            ASSERT(std::format("Animated sprite does not have the animation \"{}\"", animationName), mAnimationsData->Animations.find(animationName) != mAnimationsData->Animations.end());

            mCurrentAnimation = animationName;
            FrameTag frameTag = mAnimationsData->Animations[mCurrentAnimation];
            mTimer = 0.0f;
            mCurrentFrame = frameTag.From;
        }
    }
}