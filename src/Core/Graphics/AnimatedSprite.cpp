#include "AnimatedSprite.h"

#include <filesystem>
#include <SDL3_image/SDL_image.h>
#include <format>

#include "../Packer/PackCatalog.h"
#include "json.hpp"
#include "../Assert.h"
#include "../Logger.h"

using json = nlohmann::json;

AnimatedSprite::AnimatedSprite(std::shared_ptr<PackCatalog> catalog, SDL_Renderer *renderer, const std::string &animationPath, const std::string &mainAnimation) : mTimer(0.0f),
                                                                                                                                                                   mCurrentFrame(0),
                                                                                                                                                                   mDestRect(),
                                                                                                                                                                   mSrcRect()
{
    auto animationFileBuffer = catalog->GetFile(animationPath);
    json data = json::parse(animationFileBuffer);
    ASSERT("Animations file are exported by Aseprite and need a \"meta\" and a \"frames\" object. Frames should be an array", data.contains("frames"));
    ASSERT("Animations file are exported by Aseprite and need a \"meta\" and a \"frames\" object. Frames should be an array", data["frames"].is_array());
    ASSERT("Animations file are exported by Aseprite and need a \"meta\" and a \"frames\" object. Frames should be an array", !data["frames"].empty());
    ASSERT("Animations file are exported by Aseprite and need a \"meta\" and a \"frames\" object. Frames should be an array", data.contains("meta"));
    auto meta = data["meta"];
    auto frames = data["frames"];

    ASSERT("The \"meta\" object of an animation requires an \"image\" path", meta.contains("image") && meta["image"].is_string());
    std::string textureName = meta["image"];
    std::filesystem::path animationJsonPath = animationPath;
    std::filesystem::path texturePath = animationJsonPath.parent_path() / textureName;

    auto textureBuffer = catalog->GetFile(texturePath.string());
    mTexture = std::shared_ptr<SDL_Texture>(IMG_LoadTexture_IO(renderer, SDL_IOFromConstMem(textureBuffer.data(), textureBuffer.size()), true), SDL_DestroyTexture);
    if (!mTexture)
    {
        LOG_ERROR(std::format("Image on path {} not loaded. Error is: {}", texturePath.string(), SDL_GetError()));
    }
    SDL_SetTextureScaleMode(mTexture.get(), SDL_ScaleMode::SDL_SCALEMODE_NEAREST); // SUPER IMPORTANT!

    ASSERT("The \"meta\" object of an animation requires a \"size\" object with the w and h values.", meta.contains("size") && meta["size"].contains("w") && meta["size"].contains("h"));
    mAnimationsData.Size.x = meta["size"]["w"];
    mAnimationsData.Size.y = meta["size"]["h"];
    mAnimationsData.TexturePath = texturePath.string();

    ASSERT("Frame tags inside \"meta\" should be an array with at least a tag (animation)", meta.contains("frameTags") && meta["frameTags"].is_array() && !meta["frameTags"].empty());
    for (auto &frameTag : meta["frameTags"])
    {
        ASSERT("A frametag requires at least a name, from, to and a direction",
               frameTag.contains("name") && frameTag.contains("from") && frameTag.contains("to") && frameTag.contains("direction"));

        FrameTag frameTagAnimation;
        frameTagAnimation.Name = frameTag["name"];
        frameTagAnimation.From = frameTag["from"];
        frameTagAnimation.To = frameTag["to"];
        frameTagAnimation.Direction = frameTag["direction"];

        frameTagAnimation.Repeat = 0;
        frameTagAnimation.UserData = "";

        if (frameTag.contains("repeat"))
        {
            std::string repeat = frameTag["repeat"];
            // frameTagAnimation.Repeat = frameTag["repeat"].get<int>();
        }
        if (frameTag.contains("userdata"))
        {
            frameTagAnimation.UserData = frameTag["userdata"];
        }

        mAnimationsData.Animations[frameTagAnimation.Name] = frameTagAnimation;
    }

    for (auto &frame : frames)
    {
        ASSERT("Frame data requires at least the filename, the frame, the spriteSourceSize and the duration values",
               frame.contains("filename") && frame.contains("frame") && frame.contains("spriteSourceSize") && frame.contains("duration"));

        FrameData frameData;
        frameData.Filename = frame["filename"];
        frameData.Duration = frame["duration"];
        frameData.Duration /= 1000.0f; // Millis
        frameData.Frame.x = frame["frame"]["x"];
        frameData.Frame.y = frame["frame"]["y"];
        frameData.Frame.w = frame["frame"]["w"];
        frameData.Frame.h = frame["frame"]["h"];
        frameData.SpriteSourceSize.x = frame["spriteSourceSize"]["x"];
        frameData.SpriteSourceSize.y = frame["spriteSourceSize"]["y"];
        frameData.SpriteSourceSize.w = frame["spriteSourceSize"]["w"];
        frameData.SpriteSourceSize.h = frame["spriteSourceSize"]["h"];

        mAnimationsData.Frames.push_back(frameData);
    }

    ASSERT(std::format("Animated sprite does not have the default animation \"{}\"", mainAnimation), mAnimationsData.Animations.find(mainAnimation) != mAnimationsData.Animations.end());
    mCurrentAnimation = mAnimationsData.Animations[mainAnimation].Name;

    mSize.x = mAnimationsData.Frames[0].Frame.w;
    mSize.y = mAnimationsData.Frames[0].Frame.h;

    int w = 0;
    int h = 0;
    SDL_RendererLogicalPresentation *rlp = NULL;
    SDL_GetRenderLogicalPresentation(renderer, &w, &h, rlp);

    mScale = 5;
    mDestRect.x = 160 - mSize.x * mScale / 2;
    mDestRect.y = 90 - mSize.y * mScale / 2;

    LOG_INFO(std::format("W: {}, H: {}, mDestRect.w: {}, mDestRect.h: {}", w, h, mDestRect.w, mDestRect.h));
}

AnimatedSprite::~AnimatedSprite()
{
}

void AnimatedSprite::Render(SDL_Renderer *renderer) const
{
    SDL_SetRenderScale(renderer, 1, 1);
    SDL_RenderTexture(renderer, mTexture.get(), &mSrcRect, &mDestRect);
}

void AnimatedSprite::UpdateAnimation(const float &dt)
{
    mTimer += dt;
    FrameTag frameTag = mAnimationsData.Animations[mCurrentAnimation];
    FrameData currentFrame = mAnimationsData.Frames[mCurrentFrame];

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
    ASSERT(std::format("Animated sprite does not have the animation \"{}\"", animationName), mAnimationsData.Animations.find(animationName) != mAnimationsData.Animations.end());

    mCurrentAnimation = animationName;
    FrameTag frameTag = mAnimationsData.Animations[mCurrentAnimation];
    mTimer = 0.0f;
    mCurrentFrame = frameTag.From;
}
