#ifndef HDEF_ANIMATIONFACTORY
#define HDEF_ANIMATIONFACTORY

#include "AnimationData.h"

#include <memory>
#include <unordered_map>
#include <string>

namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Graphics
{
    class AnimationFactory
    {
    private:
        std::unordered_map<std::string, std::shared_ptr<AnimationsData>> mAnimationDataMap;
        FS::PackCatalog *mCatalog;

    public:
        AnimationFactory(FS::PackCatalog *);
        virtual ~AnimationFactory();
        std::shared_ptr<AnimationsData> GetAnimationsData(const std::string &animationDataPath);
        void DeleteCache();
        void DeleteAnimation(const std::string &texturePath);
    };

}

#endif