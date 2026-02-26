#ifndef HDEF_IANIMATIONFACTORY
#define HDEF_IANIMATIONFACTORY

#include "AnimationData.h"

#include <memory>
#include <string>

namespace Mochi::Graphics
{
    /// @brief This interface is to implement different ways to generate the animation data depending on the source of the animation data.
    class IAnimationFactory
    {
    public:
        virtual ~IAnimationFactory() {}
        /// @brief Gets the processed animation data of the file on the input path.
        /// @param animationDataPath Path or unique identifier of the file containing the animation data in the file system.
        /// @return A shared pointer to the data. This is meant to be saved as a cache.
        virtual std::shared_ptr<AnimationsData> GetAnimationsData(const std::string &animationDataPath) = 0;
        /// @brief Deletes the animations cache. This should be safe to call anytime because of the shared pointers.
        virtual void DeleteCache() = 0;
        /// @brief Deletes a specific animation in the cache. This should be safe to call anytime because of the shared pointers.
        /// @param texturePath Path or unique identifier of the file in the file system.
        virtual void DeleteAnimation(const std::string &texturePath) = 0;
    };
}

#endif