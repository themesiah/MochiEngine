#ifndef HDEF_ANIMATIONFACTORY
#define HDEF_ANIMATIONFACTORY

#include "AnimationData.h"

#include <memory>
#include <unordered_map>
#include <string>

namespace Mochi
{
    namespace FS
    {
        class PackCatalog;
    }
    namespace Graphics
    {
        class AnimationFactory
        {
        private:
            std::unordered_map<std::string, std::shared_ptr<AnimationsData>> mAnimationDataMap;
            std::shared_ptr<FS::PackCatalog> mCatalog;

        public:
            AnimationFactory(std::shared_ptr<FS::PackCatalog>);
            ~AnimationFactory();
            std::shared_ptr<AnimationsData> GetAnimationsData(const std::string &animationDataPath);
            void DeleteCache();
            void DeleteAnimation(const std::string &texturePath);
        };

    }
}
#endif