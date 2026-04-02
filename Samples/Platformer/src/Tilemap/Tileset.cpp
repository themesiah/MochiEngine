#include "Tileset.h"

#include <string>
#include <vector>

#include "Graphics/RenderCommand.h"
#include "Graphics/AnimationData.h"
#include "Graphics/IAnimationFactory.h"
#include "Graphics/AbstractTextureFactory.h"

namespace Mochi::Platformer
{
    const std::vector<int> StatusToIndex{
        10, 16, 13, 9, 11, 7, 12, 8, 14, 15, 3, 6, 1, 4, 2, 5};

    Tileset::Tileset(const std::string &tilesetData, Graphics::IAnimationFactory *animationFactory, Graphics::AbstractTextureFactory *textureFactory) : mTextureFactory(textureFactory)
    {
        mAnimationData = animationFactory->GetAnimationsData(tilesetData);
    }

    Tileset::~Tileset()
    {
    }

    Graphics::RenderCommand Tileset::GetTileCommand(TileSpaceStatus status)
    {
        // 0: Empty / Unused
        // 1: Top-left corner
        // 2: Top
        // 3: Top-right corner
        // 4: Left
        // 5: Center (no borders)
        // 6: Right
        // 7: Bottom-left corner
        // 8: Bottom
        // 9: Bottom-right corner
        // 10: Center (all borders)
        // 11: Left-top-bottom
        // 12: Top-bottom
        // 13: Right-top-bottom
        // 14: Top-left-right
        // 15: Left-right
        // 16: Bottom-left-right

        int index = StatusToIndex[status];
        Graphics::RenderCommand rc;
        rc.texture = mTextureFactory->GetTexture(mAnimationData->TexturePath.string()).get();
        rc.sourceRect = mAnimationData->Frames[index].Frame;
        return rc;
    }
}