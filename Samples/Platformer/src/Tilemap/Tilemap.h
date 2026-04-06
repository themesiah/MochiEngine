#ifndef HDEF_TILEMAP
#define HDEF_TILEMAP

#include <vector>
#include <memory>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "Types/Types.hpp"
#include "Graphics/RenderCommand.h"
#include "ECS/ECSWorld.h"

#include "Tileset.h"
#include "TilemapDataStructures.h"

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
        // Templates
        std::unordered_map<std::string, std::shared_ptr<Tileset>> mTilesets;
        std::unordered_map<std::string, EnemyType> mEnemyTypes;
        std::unordered_map<std::string, BreakableType> mBreakableTypes;

        // Tilemap json data
        TilemapProperties mProperties;
        std::vector<std::string> mTilesetIds;
        std::vector<TilemapTile> mTiles;
        ZIndexes mZindexes;
        std::vector<TilemapCoin> mCoins;
        std::vector<TilemapEnemy> mEnemies;
        std::vector<TilemapBreakable> mBreakables;
        TilemapPlayerStart mPlayerStart;

        // Entities
        std::vector<ECS::EntityType> mTileEntities;
        std::vector<ECS::EntityType> mCoinEntities;
        std::vector<ECS::EntityType> mEnemyEntities;
        std::vector<ECS::EntityType> mBreakableEntities;
        ECS::EntityType mPlayerEntity;

        std::vector<Graphics::RenderCommand> mRenderCommandCache;
        ECS::ECSWorld *mWorld;
        Graphics::AbstractTextureFactory *mTextureFactory;
        Graphics::IAnimationFactory *mAnimationFactory;
        FS::PackCatalog *mCatalog;
        float mRealSquareSize;

        void LoadProperties(const nlohmann::json properties);
        void LoadTilesets(const nlohmann::json tilesets);
        void LoadZIndexes(const nlohmann::json zindexes);
        void LoadEnemyTypes(const nlohmann::json enemyTypes);
        void LoadBreakableTypes(const nlohmann::json breakableTypes);
        void LoadScenario(const nlohmann::json scenario);
        void LoadEnemies(const nlohmann::json enemies);
        void LoadCoins(const nlohmann::json coins);
        void LoadBreakables(const nlohmann::json breakables);
        void LoadPlayer(const nlohmann::json playerStart);
        void InitScenario();
        void InitEnemies();
        void InitCoins();
        void InitBreakables();
        void InitPlayer();

    public:
        Tilemap(ECS::ECSWorld *world, FS::PackCatalog *catalog, Graphics::AbstractTextureFactory *textureFactory, Graphics::IAnimationFactory *animationFactory);
        ~Tilemap();
        Rectf GetTile(const int &i, const int &j) const;
        bool HasTileAt(const int &i, const int &j) const;

        void SetProperties(const TilemapProperties &properties);
        void AddTileset(const std::string &tilesetPath);
        void RemoveTileset(const int &tilesetIndex);
        void ResetTilesets();
        void AddTile(const int &i, const int &j, const int &tileset);
        void RemoveTile(const int &tileIndex);
        void ResetTiles();
        void AddEnemy(const int &i, const int &j, const std::string &type);
        void RemoveEnemy(const int &enemyIndex);
        void ResetEnemies();
        void AddCoin(const int &i, const int &j);
        void RemoveCoin(const int &coinIndex);
        void ResetCoins();
        void AddBreakable(const int &i, const int &j, const std::string &type);
        void RemoveBreakable(const int &breakableIndex);
        void ResetBreakables();
        void SetPlayerStartingPosition(const int &i, const int &j);

        void LoadTilemap(const std::string &tilemapData);
        void SaveTilemap(const std::string &path);
        void InitMap();
        void Render() const;
#if DEBUG
        void DebugGizmos(Debug::IGizmos *gizmos) const;
#endif
    };
}

#endif