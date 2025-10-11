#ifndef HDEF_IANIMATIONFACTORY
#define HDEF_IANIMATIONFACTORY

#include "AnimationData.h"

#include <memory>
#include <string>

namespace Mochi::Graphics
{
    class IAnimationFactory
    {
    public:
        virtual ~IAnimationFactory() {}
        virtual std::shared_ptr<AnimationsData> GetAnimationsData(const std::string &animationDataPath) = 0;
        virtual void DeleteCache() = 0;
        virtual void DeleteAnimation(const std::string &texturePath) = 0;
    };
}

#endif