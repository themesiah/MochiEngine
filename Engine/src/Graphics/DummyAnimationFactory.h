#ifndef HDEF_DUMMYANIMATIONFACTORY
#define HDEF_DUMMYANIMATIONFACTORY

#include "IAnimationFactory.h"
#include "AnimationData.h"

namespace Mochi::Graphics
{
    class DummyAnimationFactory : public IAnimationFactory
    {
    public:
        DummyAnimationFactory() {}
        virtual ~DummyAnimationFactory() {}
        virtual std::shared_ptr<AnimationsData> GetAnimationsData(const std::string &animationDataPath) override
        {
            AnimationsData *ad = new AnimationsData();
            return std::shared_ptr<AnimationsData>(ad);
        }
        virtual void DeleteCache() override {};
        virtual void DeleteAnimation(const std::string &texturePath) override {};
    };
}

#endif