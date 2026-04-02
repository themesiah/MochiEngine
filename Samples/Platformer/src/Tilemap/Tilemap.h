#ifndef HDEF_TILEMAP
#define HDEF_TILEMAP

#include <vector>
#include <nlohmann/json.hpp>

#include "Types/Types.hpp"
#include "Graphics/RenderCommand.h"

#include "Tileset.h"
#include "Tile.h"

namespace Mochi::ECS
{
    class ECSWorld;
}
namespace Mochi::Graphics
{
    class Camera;
    class AbstractTextureFactory;
    class IAnimationFactory;
}
namespace Mochi::FS
{
    class PackCatalog;
}
#if DEBUG
namespace Mochi::Debug
{
    class IGizmos;
}
#endif
namespace Mochi::Platformer
{
    class Tilemap
    {
    private:
        std::vector<Tileset> mTilesets;
        std::vector<Tile> mTiles;
        std::vector<Graphics::RenderCommand> mRenderCommandCache;
        ECS::ECSWorld *mWorld;
        Graphics::AbstractTextureFactory *mTextureFactory;
        Graphics::IAnimationFactory *mAnimationFactory;
        FS::PackCatalog *mCatalog;
        int mWidth;
        int mHeight;
        float mSquareSize;
        float mSquareSizePixels;

        void LoadTilesets(const nlohmann::json tilesets);
        void LoadScenario(const nlohmann::json scenario, const nlohmann::json zindexes);
        void LoadEnemies(const nlohmann::json enemyTypes, const nlohmann::json enemies);
        void LoadCoins(const nlohmann::json coins);
        void LoadBreakables(const nlohmann::json breakableTypes, const nlohmann::json breakables);
        void InitPlayer(const nlohmann::json playerStart);

    public:
        Tilemap(ECS::ECSWorld *world, FS::PackCatalog *catalog, Graphics::AbstractTextureFactory *textureFactory, Graphics::IAnimationFactory *animationFactory);
        ~Tilemap();
        Rectf GetTile(const int &i, const int &j) const;
        bool HasTileAt(const int &i, const int &j) const;
        void LoadTilemap(const std::string &tilemapData);
        void Render() const;
#if DEBUG
        void DebugGizmos(Debug::IGizmos *gizmos) const;
#endif
    };
}

#endif