#include "Spritesheet.h"

#include "../Utils/Assert.h"
#include "AnimationFactory.h"
#include "TextureFactory.h"

namespace Mochi::Graphics
{
    Spritesheet::Spritesheet(
        std::shared_ptr<AnimationFactory> animationFactory,
        std::shared_ptr<TextureFactory> textureFactory,
        const std::string &animationPath,
        const int &startingFrame)
        : mDestRect(),
          mSrcRect(),
          mScale(1.0f)
    {
        mAnimationsData = animationFactory->GetAnimationsData(animationPath);
        ASSERT("Animations data was not loaded", mAnimationsData != nullptr);
        mTexture = textureFactory->GetTexture(mAnimationsData->TexturePath.string());
        ASSERT("Texture data was not loaded", mTexture != nullptr);

        SetFrame(startingFrame);

        mDestRect.x = 0;
        mDestRect.y = 0;
    }

    Spritesheet::~Spritesheet()
    {
    }

    std::vector<Graphics::RenderCommand> Spritesheet::GetRenderData() const
    {
        RenderCommand rc;
        rc.texture = mTexture;
        rc.sourceRect = mSrcRect;
        rc.destRect = mDestRect;
        rc.zindex = 1; // TEMP
        return {rc};
    }

    void Spritesheet::SetFrame(const int &frameIndex)
    {
        mFrameIndex = frameIndex;
        ApplyFrameData();
    }

    int Spritesheet::GetFrame() const
    {
        return mFrameIndex;
    }

    void Spritesheet::SetScale(const float &scale)
    {
        mScale = scale;
        ApplyFrameData();
    }

    float Spritesheet::GetScale() const
    {
        return mScale;
    }

    void Spritesheet::ApplyFrameData()
    {
        auto frame = mAnimationsData->Frames[mFrameIndex];
        mSrcRect = frame.Frame;

        mDestRect.w = frame.Frame.w * mScale;
        mDestRect.h = frame.Frame.h * mScale;
    }

    void Spritesheet::SetPosition(const Vector2f &position)
    {
        mDestRect.SetPosition(position);
    }

    Vector2f Spritesheet::GetPosition() const
    {
        return mDestRect.GetPosition();
    }

}