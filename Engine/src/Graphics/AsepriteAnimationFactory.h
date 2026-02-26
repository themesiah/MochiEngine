#ifndef HDEF_ANIMATIONFACTORY
#define HDEF_ANIMATIONFACTORY

#include "AnimationData.h"
#include "IAnimationFactory.h"

#include <memory>
#include <unordered_map>
#include <string>

namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Graphics
{
    /**
     * @brief This AnimationFactory takes Aseprites spritesheet export format and automatically generates all needed animation data for that spritesheet.
     *
     * Animation data is expected as a JSON with specific configuration:
     *
     * Sheet Type: Packed
     *
     * Split Layers: False
     *
     * Split Tags: False
     *
     * Array, Meta Tags
     *
     * Item Filename: {tag}_{tagframe}
     *
     * Item Tagname: {tag}
     *
     * Check AsepriteExport.png
     */
    class AsepriteAnimationFactory : public IAnimationFactory
    {
    private:
        std::unordered_map<std::string, std::shared_ptr<AnimationsData>> mAnimationDataMap;
        FS::PackCatalog *mCatalog;

    public:
        AsepriteAnimationFactory(FS::PackCatalog *);
        virtual ~AsepriteAnimationFactory();
        virtual std::shared_ptr<AnimationsData> GetAnimationsData(const std::string &animationDataPath) override;
        virtual void DeleteCache() override;
        virtual void DeleteAnimation(const std::string &texturePath) override;
    };

}

#endif