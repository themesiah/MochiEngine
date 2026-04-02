#ifndef HDEF_TILESET
#define HDEF_TILESET

#include <string>
#include <vector>
#include <memory>

#include "Graphics/RenderCommand.h"
#include "Graphics/AnimationData.h"

namespace Mochi::Graphics
{
    class IAnimationFactory;
    class AbstractTextureFactory;
}
namespace Mochi::Platformer
{
    using TileSpaceStatus = uint8_t;
    class Tileset
    {
    private:
        std::shared_ptr<Graphics::AnimationsData> mAnimationData;
        Graphics::AbstractTextureFactory *mTextureFactory;

    public:
        Tileset(const std::string &tilesetData, Graphics::IAnimationFactory *animationFactory, Graphics::AbstractTextureFactory *textureFactory);
        virtual ~Tileset();
        Graphics::RenderCommand GetTileCommand(TileSpaceStatus status);
    };
}

#endif