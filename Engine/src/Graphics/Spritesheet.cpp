#include "Spritesheet.h"

#include "../Utils/Assert.h"
#include "IAnimationFactory.h"
#include "AbstractTextureFactory.h"

namespace Mochi::Graphics
{
    Spritesheet::Spritesheet(
        IAnimationFactory *animationFactory,
        AbstractTextureFactory *textureFactory,
        const std::string &animationPath,
        const int &startingFrame)
        : SpriteBase()
    {
        mAnimationsData = animationFactory->GetAnimationsData(animationPath);
        ASSERT("Animations data was not loaded", mAnimationsData != nullptr);
        LoadTexture(textureFactory, mAnimationsData->TexturePath.string());

        SetFrame(startingFrame);
    }

    Spritesheet::~Spritesheet()
    {
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

    void Spritesheet::ApplyFrameData()
    {
        auto frame = mAnimationsData->Frames[mFrameIndex];
        SetSrcRect(frame.Frame);

        mSize = Vector2f(frame.Frame.w, frame.Frame.h);
    }

}