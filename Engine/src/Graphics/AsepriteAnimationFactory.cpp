#include "AsepriteAnimationFactory.h"

#include <nlohmann/json.hpp>

#include "../Utils/StringUtils.h"
#include "../Utils/Logger.h"
#include "../Utils/Assert.h"
#include "../Packer/PackCatalog.h"

#include "../Exception.hpp"

namespace Mochi::Graphics
{
    AsepriteAnimationFactory::AsepriteAnimationFactory(FS::PackCatalog *catalog) : mCatalog(catalog)
    {
    }

    AsepriteAnimationFactory::~AsepriteAnimationFactory()
    {
    }

    std::shared_ptr<AnimationsData> AsepriteAnimationFactory::GetAnimationsData(const std::string &animationDataPath)
    {
        try
        {
            std::string normalizedPath = Utils::NormalizePath(animationDataPath);
            if (mAnimationDataMap.find(normalizedPath) != mAnimationDataMap.end())
            {
                return mAnimationDataMap[normalizedPath];
            }

            auto animationFileBuffer = mCatalog->GetFile(normalizedPath);
            nlohmann::json data = nlohmann::json::parse(animationFileBuffer);
            ASSERT("Animations file are exported by Aseprite and need a \"meta\" and a \"frames\" object. Frames should be an array", data.contains("frames"));
            ASSERT("Animations file are exported by Aseprite and need a \"meta\" and a \"frames\" object. Frames should be an array", data["frames"].is_array());
            ASSERT("Animations file are exported by Aseprite and need a \"meta\" and a \"frames\" object. Frames should be an array", !data["frames"].empty());
            ASSERT("Animations file are exported by Aseprite and need a \"meta\" and a \"frames\" object. Frames should be an array", data.contains("meta"));
            auto meta = data["meta"];
            auto frames = data["frames"];

            std::shared_ptr<AnimationsData> animationsData = std::make_shared<AnimationsData>();
            ASSERT("The \"meta\" object of an animation requires an \"image\" path", meta.contains("image") && meta["image"].is_string());
            std::string textureName = meta["image"];
            std::filesystem::path animationJsonPath = normalizedPath;
            std::filesystem::path texturePath = animationJsonPath.parent_path() / textureName;

            ASSERT("The \"meta\" object of an animation requires a \"size\" object with the w and h values.", meta.contains("size") && meta["size"].contains("w") && meta["size"].contains("h"));
            animationsData->Size.x = meta["size"]["w"];
            animationsData->Size.y = meta["size"]["h"];
            animationsData->TexturePath = texturePath;

            ASSERT("Frame tags inside \"meta\" should be an array with at least a tag (animation)", meta.contains("frameTags") && meta["frameTags"].is_array() && !meta["frameTags"].empty());
            for (auto &frameTag : meta["frameTags"])
            {
                ASSERT("A frametag requires at least a name, from, to and a direction",
                       frameTag.contains("name") && frameTag.contains("from") && frameTag.contains("to") && frameTag.contains("direction"));

                FrameTag frameTagAnimation;
                frameTagAnimation.Name = frameTag["name"];
                frameTagAnimation.From = frameTag["from"];
                frameTagAnimation.To = frameTag["to"];
                std::string direction = frameTag["direction"];
                if (direction == "Forward")
                {
                    frameTagAnimation.Direction = AnimationDirection::Forward;
                }
                else if (direction == "Backward")
                {
                    frameTagAnimation.Direction = AnimationDirection::Backward;
                }
                else if (direction == "Pingpong")
                {
                    frameTagAnimation.Direction = AnimationDirection::Pingpong;
                }
                else if (direction == "BackwardPingpong")
                {
                    frameTagAnimation.Direction = AnimationDirection::BackwardPingPong;
                }
                else
                {
                    frameTagAnimation.Direction = AnimationDirection::Forward;
                }

                frameTagAnimation.Repeat = 0;
                frameTagAnimation.UserData = "";

                if (frameTag.contains("repeat"))
                {
                    std::string repeat = frameTag["repeat"];
                    frameTagAnimation.Repeat = std::atoi(repeat.c_str());
                }
                if (frameTag.contains("userdata"))
                {
                    frameTagAnimation.UserData = frameTag["userdata"];
                }

                animationsData->Animations[frameTagAnimation.Name] = frameTagAnimation;
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

                animationsData->Frames.push_back(frameData);
            }
            return animationsData;
        }
        catch (const std::runtime_error &e)
        {
            throw EngineError(e.what());
        }
    }

    void AsepriteAnimationFactory::DeleteCache()
    {
        mAnimationDataMap.clear();
    }

    void AsepriteAnimationFactory::DeleteAnimation(const std::string &animationPath)
    {
        std::string normalizedPath = Utils::NormalizePath(animationPath);
        if (mAnimationDataMap.find(normalizedPath) != mAnimationDataMap.end())
        {
            mAnimationDataMap.erase(normalizedPath);
        }
        else
        {
            LOG_WARNING(std::format("Trying to delete animation with path {} that was not cached.", animationPath));
        }
    }
}
