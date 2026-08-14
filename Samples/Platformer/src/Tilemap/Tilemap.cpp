#include "Tilemap.h"

#include <vector>
#include <nlohmann/json.hpp>

#include "Engine.h"
#include "ECS/ECSWorld.h"
#include "Utils/Logger.h"
#include "Graphics/Camera.h"
#include "Utils/Conversion.hpp"
#include "Packer/PackCatalog.h"
#include "Graphics/AbstractTextureFactory.h"
#include "Graphics/IAnimationFactory.h"
#include "Physics/Shapes.h"
#include "Physics/Collisions.h"

#include "Tileset.h"
#include "TilemapDataStructures.h"

#include "ECS/Components/ECSSprite.h"
#include "ECS/Components/ECSTransform.h"
#include "ECS/Components/ECSAnimation.h"
#include "ECS/Components/ECSCollider.h"
#include "ECS/Components/CharacterController.hpp"
#include "../Components/PlayerComponent.h"
#include "../Components/LeftRightComponent.h"
#include "../Components/EnemyComponent.h"
#include "../Components/GlobalDataComponent.h"
#include "../Components/CoinComponent.h"
#include "../Components/BreakableComponent.h"
#include "../PlatformerLayers.h"

#if DEBUG
#include "Debug/IGizmos.h"
#include "GUI/AbstractGUI.h"
#endif

namespace Mochi::Platformer
{
    Tilemap::Tilemap(ECS::ECSWorld *world, FS::PackCatalog *catalog, Graphics::AbstractTextureFactory *textureFactory, Graphics::IAnimationFactory *animationFactory)
        : mWorld(world), mTilesets({}), mTiles({}), mRenderCommandCache({}), mCatalog(catalog), mTextureFactory(textureFactory), mAnimationFactory(animationFactory),
          mInitialized(false)
    {
    }

    Tilemap::~Tilemap()
    {
    }

    Rectf Tilemap::GetTile(const int &i, const int &j) const
    {
        if (i >= mProperties.Width || j >= mProperties.Height)
            throw EngineError("Tilemap is not that big");

        float x = i * mRealSquareSize + mRealSquareSize / 2.0f;
        float y = j * mRealSquareSize + mRealSquareSize / 2.0f;
        return Rectf{x, y, mRealSquareSize, mRealSquareSize};
    }

    int Tilemap::GetTileIndex(const int &i, const int &j) const
    {
        for (int z = 0; z < mTiles.size(); ++z)
        {
            if (mTiles[z].i == i && mTiles[z].j == j)
            {
                return z;
            }
        }
        return -1;
    }

    bool Tilemap::HasTileAt(const int &i, const int &j) const
    {
        for (auto t : mTiles)
        {
            if (t.i == i && t.j == j)
                return true;
        }
        return false;
    }

    bool Tilemap::IsValidPosition(const int &i, const int &j) const
    {
        return i >= 0 && j >= 0 && i < mProperties.Width && j < mProperties.Height;
    }

    TilemapTile Tilemap::WorldToTile(const Vector2f &worldPos) const
    {
        auto worldPos2 = worldPos;
        Vector2f screenWorldSize = PixelsToMeters(Vector2f{CONST_RENDER_LOGICAL_X, CONST_RENDER_LOGICAL_Y});
        worldPos2 -= screenWorldSize / 2.0f;
        int i = Math::Floor(worldPos2.x / mRealSquareSize);
        int j = Math::Floor(worldPos2.y / mRealSquareSize);

        if (HasTileAt(i, j))
        {
            return mTiles[GetTileIndex(i, j)];
        }
        return {i, j, -1};
    }

    int Tilemap::GetEnemyIndexAt(const int &i, const int &j)
    {
        for (int z = 0; z < mEnemies.size(); ++z)
        {
            if (mEnemies[z].i == i && mEnemies[z].j == j)
            {
                return z;
            }
        }
        return -1;
    }

    int Tilemap::GetCoinIndexAt(const int &i, const int &j)
    {
        for (int z = 0; z < mCoins.size(); ++z)
        {
            if (mCoins[z].i == i && mCoins[z].j == j)
            {
                return z;
            }
        }
        return -1;
    }

    int Tilemap::GetBreakableIndexAt(const int &i, const int &j)
    {
        for (int z = 0; z < mBreakables.size(); ++z)
        {
            if (mBreakables[z].i == i && mBreakables[z].j == j)
            {
                return z;
            }
        }
        return -1;
    }

    void Tilemap::LoadTilemap(const std::string &tilemapData)
    {
        mInitialized = false;
        std::vector<char> raw = mCatalog->GetFile(tilemapData);
        nlohmann::json data = nlohmann::json::parse(raw);

        // Main properties
        auto properties = data.at("properties");
        LoadProperties(properties);

        // Zindexes
        std::vector<char> rawZindex = mCatalog->GetFile("ZIndexes.json");
        nlohmann::json zindexData = nlohmann::json::parse(rawZindex);
        LoadZIndexes(zindexData);

        // Load tilesets
        auto tileSets = data.at("tilesets");
        LoadTilesets(tileSets);

        // Load enemy types
        std::vector<char> rawEnemies = mCatalog->GetFile("EnemyTypes.json");
        nlohmann::json enemyDataTypes = nlohmann::json::parse(rawEnemies);
        LoadEnemyTypes(enemyDataTypes);

        // Load breakable types
        std::vector<char> rawBreakables = mCatalog->GetFile("BreakableTypes.json");
        nlohmann::json breakableDataTypes = nlohmann::json::parse(rawBreakables);
        LoadBreakableTypes(breakableDataTypes);

        // Load main scenario
        auto scenario = data.at("scenario");
        LoadScenario(scenario);

        // Load enemies
        auto enemies = data.at("enemies");
        LoadEnemies(enemies);

        // Load coins
        auto coins = data.at("coins");
        LoadCoins(coins);

        // Load breakables
        auto breakables = data.at("breakables");
        LoadBreakables(breakables);

        // Initialize the player
        auto playerStart = data.at("playerStart");
        LoadPlayer(playerStart);

        // Generate and cache render commands to easily draw the same scenario each frame
        GenerateRenderCommands();
        mInitialized = true;
    }

    void Tilemap::LoadDefault()
    {
        LoadTilemap("DefaultTilemap.json");
    }

    void Tilemap::LoadProperties(const nlohmann::json properties)
    {
        mProperties.Width = properties.at("width");
        mProperties.Height = properties.at("height");
        mProperties.TileSize = properties.at("tileSize");

        mRealSquareSize = PixelsToMeters(mProperties.TileSize);
    }

    void Tilemap::LoadTilesets(const nlohmann::json tilesets)
    {
        for (size_t i = 0; i < tilesets.size(); ++i)
        {
            AddTileset(tilesets[i]);
        }
    }

    void Tilemap::LoadZIndexes(const nlohmann::json zindexes)
    {
        auto zindex = zindexes["zindex"];
        mZindexes.Background = zindex["background"];
        mZindexes.Breakables = zindex["breakables"];
        mZindexes.Coins = zindex["coins"];
        mZindexes.Enemies = zindex["enemies"];
        mZindexes.Foreground = zindex["foreground"];
        mZindexes.Scenario = zindex["scenario"];
        mZindexes.Player = zindex["player"];
    }

    void Tilemap::LoadEnemyTypes(const nlohmann::json enemyTypes)
    {
        auto types = enemyTypes["enemyTypes"];
        for (size_t i = 0; i < types.size(); ++i)
        {
            auto type = types[i];
            Rectf collider;
            auto colliderSize = type["collider"];
            collider.x = colliderSize[0];
            collider.y = colliderSize[1];
            collider.w = colliderSize[2];
            collider.h = colliderSize[3];
            EnemyType et{type["id"], type["sourceType"], type["source"], collider};
            mEnemyTypes[et.Id] = et;
        }
    }

    void Tilemap::LoadBreakableTypes(const nlohmann::json breakableTypes)
    {
        auto types = breakableTypes["breakableTypes"];
        for (size_t i = 0; i < types.size(); ++i)
        {
            auto type = types[i];
            BreakableType bt{type["id"], type["sourceType"], type["source"]};
            mBreakableTypes[bt.Id] = bt;
        }
    }

    void Tilemap::LoadScenario(const nlohmann::json scenario)
    {
        for (size_t i = 0; i < scenario.size(); ++i)
        {
            nlohmann::json tile = scenario[i];
            int set = tile.at("set");
            int x = tile.at("i");
            int y = tile.at("j");
            AddTile(x, y, set);
        }
    }

    void Tilemap::LoadEnemies(const nlohmann::json enemies)
    {
        for (size_t z = 0; z < enemies.size(); ++z)
        {
            auto enemy = enemies[z];
            int i = enemy["i"];
            int j = enemy["j"];
            std::string type = enemy["type"];
            AddEnemy(i, j, type);
        }
    }

    void Tilemap::LoadCoins(const nlohmann::json coins)
    {
        for (size_t z = 0; z < coins.size(); ++z)
        {
            auto coin = coins[z];
            int i = coin["i"];
            int j = coin["j"];
            AddCoin(i, j);
        }
    }

    void Tilemap::LoadBreakables(const nlohmann::json breakables)
    {
        for (size_t z = 0; z < breakables.size(); ++z)
        {
            auto breakable = breakables[z];
            int i = breakable["i"];
            int j = breakable["j"];
            std::string type = breakable["type"];
            AddBreakable(i, j, type);
        }
    }

    void Tilemap::LoadPlayer(const nlohmann::json playerStart)
    {
        int i = playerStart["i"];
        int j = playerStart["j"];
        SetPlayerStartingPosition(i, j);
    }

    void Tilemap::Render() const
    {
        Engine::Get().AddRenderCommands(mRenderCommandCache);
    }

    void Tilemap::SaveTilemap(const std::string &path)
    {
    }

    void Tilemap::SetProperties(const TilemapProperties &properties)
    {
        mProperties = properties;
    }

    void Tilemap::AddTileset(const std::string &tilesetPath)
    {
        mTilesetIds.push_back(tilesetPath);

        std::shared_ptr<Tileset> ts = std::make_shared<Tileset>(tilesetPath, mAnimationFactory, mTextureFactory);
        mTilesets[tilesetPath] = ts;
    }

    void Tilemap::RemoveTileset(const int &tilesetIndex)
    {
        if (mTilesetIds.size() > tilesetIndex)
        {
            mTilesets.erase(mTilesetIds[tilesetIndex]);
            mTilesetIds.erase(mTilesetIds.cbegin() + tilesetIndex);
        }
        else
        {
            LOG_ERROR(std::format("No tileset with index", tilesetIndex));
        }
    }

    void Tilemap::ResetTilesets()
    {
        mTilesetIds.clear();
        mTilesets.clear();
    }

    void Tilemap::AddTile(const int &i, const int &j, const int &tileset)
    {
        if (HasTileAt(i, j))
        {
            RemoveTile(GetTileIndex(i, j));
        }
        TilemapTile tile{i, j, tileset};
        mTiles.push_back(tile);
        if (mInitialized)
        {
            GenerateRenderCommands();
            AddTileEntity(tile);
        }
    }

    void Tilemap::AddTileEntity(const TilemapTile &tile)
    {

        auto blockEntity = mWorld->CreateEntity();
        mTileEntities.push_back(blockEntity);
        Rectf tileRect = GetTile(tile.i, tile.j);
        // TODO: Group colliders in bigger colliders
        mWorld->Set<ECS::TransformComponent>(blockEntity, ECS::TransformComponent{tileRect.GetPosition(), 1.0f});
        mWorld->Set<ECS::ColliderComponent>(blockEntity, ECS::ColliderComponent(
                                                             Physics::Rectangle{Vector2f{0.0f, 0.0f}, tileRect.GetSize() / 2.0f},
                                                             PlatformerLayers::Scenario,
                                                             0,
                                                             false));
    }

    void Tilemap::RemoveTile(const int &tileIndex)
    {
        if (mTiles.size() > tileIndex)
        {
            mTiles.erase(mTiles.cbegin() + tileIndex);
            mWorld->DestroyEntity(mTileEntities[tileIndex]);
            mTileEntities.erase(mTileEntities.cbegin() + tileIndex);
            if (mInitialized)
                GenerateRenderCommands();
        }
        else
        {
            LOG_ERROR(std::format("No tile with index", tileIndex));
        }
    }

    void Tilemap::ResetTiles()
    {
        mTiles.clear();
        if (mInitialized)
            GenerateRenderCommands();
    }

    void Tilemap::AddEnemy(const int &i, const int &j, const std::string &type)
    {
        int enemyIndexAt = GetEnemyIndexAt(i, j);
        if (enemyIndexAt >= 0)
        {
            RemoveEnemy(enemyIndexAt);
        }
        TilemapEnemy enemy{i, j, type};
        mEnemies.push_back(enemy);
        if (mInitialized)
            AddEnemyEntity(enemy);
    }

    void Tilemap::AddEnemyEntity(const TilemapEnemy &enemy)
    {
        auto enemyEntity = mWorld->CreateEntity();
        mEnemyEntities.push_back(enemyEntity);
        EnemyType enemyType = mEnemyTypes[enemy.Type];
        std::string texPath;
        if (enemyType.SourceType == (int)SourceType::Sprite)
        {
            texPath = enemyType.Source;
        }
        else if (enemyType.SourceType == (int)SourceType::Animation)
        {
            auto animation = mAnimationFactory->GetAnimationsData(enemyType.Source);
            texPath = animation->TexturePath.string();
            ECS::AnimationComponent animationComponent{animation->Animations.cbegin()->first};
            mWorld->Set<ECS::AnimationComponent>(enemyEntity, animationComponent);

            mWorld->Set<Mochi::Graphics::AnimationsData>(enemyEntity, *(animation.get()));
        }
        Graphics::ITexture *enemyTex = mTextureFactory->GetTexture(texPath).get();
        auto colliderSize = enemyType.Collider;

        mWorld->Set<ECS::TransformComponent>(enemyEntity, ECS::TransformComponent{GetTile(enemy.i, enemy.j).GetPosition(), 1.0f});
        mWorld->Set<ECS::SpriteComponent>(enemyEntity, ECS::SpriteComponent{enemyTex, mZindexes.Enemies});
        mWorld->Set<ECS::ColliderComponent>(enemyEntity, ECS::ColliderComponent(
                                                             Physics::Rectangle{colliderSize.GetPosition(), PixelsToMeters(colliderSize.GetSize() / 2.0f)},
                                                             PlatformerLayers::Enemy,
                                                             PlatformerLayers::Player,
                                                             false));
        mWorld->Set<ECS::CharacterController>(enemyEntity, ECS::CharacterController(2.0f, 100.0f, 20.0f, -20.0f, 20.0f, 0.1f, false, PlatformerLayers::Scenario));
        mWorld->Set<LeftRightComponent>(enemyEntity, LeftRightComponent{-1.0f, 2, 0.8f});
        mWorld->Set<EnemyComponent>(enemyEntity, EnemyComponent{});
    }

    void Tilemap::RemoveEnemy(const int &enemyIndex)
    {
        if (mEnemies.size() > enemyIndex)
        {
            mEnemies.erase(mEnemies.cbegin() + enemyIndex);
            mWorld->DestroyEntity(mEnemyEntities[enemyIndex]);
            mEnemyEntities.erase(mEnemyEntities.cbegin() + enemyIndex);
        }
        else
        {
            LOG_ERROR(std::format("No tile with index", enemyIndex));
        }
    }

    void Tilemap::ResetEnemies()
    {
        mEnemies.clear();
    }

    void Tilemap::AddCoin(const int &i, const int &j)
    {
        if (GetCoinIndexAt(i, j) >= 0)
            return;
        TilemapCoin coin{i, j};
        mCoins.push_back(coin);
        if (mInitialized)
            AddCoinEntity(coin);
    }

    void Tilemap::AddCoinEntity(const TilemapCoin &coin)
    {
        ECS::AnimationComponent animationComponent{"Idle"};
        auto animation = mAnimationFactory->GetAnimationsData("Coin.json");
        auto coinTex = mTextureFactory->GetTexture(animation->TexturePath.string());
        auto coinEntity = mWorld->CreateEntity();
        mCoinEntities.push_back(coinEntity);
        mWorld->Set<ECS::TransformComponent>(coinEntity, ECS::TransformComponent{GetTile(coin.i, coin.j).GetPosition(), 1.0f});
        mWorld->Set<ECS::SpriteComponent>(coinEntity, ECS::SpriteComponent{coinTex.get(), mZindexes.Coins});
        mWorld->Set<ECS::ColliderComponent>(coinEntity, ECS::ColliderComponent(
                                                            Physics::Circle{Vector2f{0.0f, 0.0f}, PixelsToMeters(12.0f)},
                                                            PlatformerLayers::Coin,
                                                            0,
                                                            false));
        mWorld->Set<CoinComponent>(coinEntity);
        mWorld->Set<ECS::AnimationComponent>(coinEntity, animationComponent);

        mWorld->Set<Mochi::Graphics::AnimationsData>(coinEntity, *(animation.get()));
    }

    void Tilemap::RemoveCoin(const int &coinIndex)
    {
        if (mCoins.size() > coinIndex)
        {
            mCoins.erase(mCoins.cbegin() + coinIndex);
            mWorld->DestroyEntity(mCoinEntities[coinIndex]);
            mCoinEntities.erase(mCoinEntities.cbegin() + coinIndex);
        }
        else
        {
            LOG_ERROR(std::format("No tile with index", coinIndex));
        }
    }

    void Tilemap::ResetCoins()
    {
        mCoins.clear();
    }

    void Tilemap::AddBreakable(const int &i, const int &j, const std::string &type)
    {
        int breakableIndexAt = GetBreakableIndexAt(i, j);
        if (breakableIndexAt >= 0)
        {
            RemoveBreakable(breakableIndexAt);
        }
        TilemapBreakable breakable{i, j, type};
        mBreakables.push_back(breakable);
        if (mInitialized)
            AddBreakableEntity(breakable);
    }

    void Tilemap::AddBreakableEntity(const TilemapBreakable &breakable)
    {
        auto breakableEntity = mWorld->CreateEntity();
        mBreakableEntities.push_back(breakableEntity);
        BreakableType breakableType = mBreakableTypes[breakable.Type];

        std::string texPath;
        if (breakableType.SourceType == SourceType::Sprite)
        {
            texPath = breakableType.Source;
        }
        else if (breakableType.SourceType == SourceType::Animation)
        {
            auto animation = mAnimationFactory->GetAnimationsData(breakableType.Source);
            texPath = animation->TexturePath.string();
            ECS::AnimationComponent animationComponent{animation->Animations.cbegin()->first};
            mWorld->Set<ECS::AnimationComponent>(breakableEntity, animationComponent);

            mWorld->Set<Mochi::Graphics::AnimationsData>(breakableEntity, *(animation.get()));
        }
        auto breakableTex = mTextureFactory->GetTexture(texPath);
        mWorld->Set<ECS::TransformComponent>(breakableEntity, ECS::TransformComponent{GetTile(breakable.i, breakable.j).GetPosition(), 1.0f});
        mWorld->Set<ECS::SpriteComponent>(breakableEntity, ECS::SpriteComponent{breakableTex.get(), mZindexes.Breakables});
        mWorld->Set<ECS::ColliderComponent>(breakableEntity, ECS::ColliderComponent(
                                                                 Physics::Rectangle{Vector2f{0.0f, 0.0f}, PixelsToMeters(breakableTex->GetSize() / 2.0f)},
                                                                 PlatformerLayers::Scenario,
                                                                 0,
                                                                 false));
        mWorld->Set<BreakableComponent>(breakableEntity);
    }

    void Tilemap::RemoveBreakable(const int &breakableIndex)
    {
        if (mBreakables.size() > breakableIndex)
        {
            mBreakables.erase(mBreakables.cbegin() + breakableIndex);
            mWorld->DestroyEntity(mBreakableEntities[breakableIndex]);
            mBreakableEntities.erase(mBreakableEntities.cbegin() + breakableIndex);
        }
        else
        {
            LOG_ERROR(std::format("No tile with index", breakableIndex));
        }
    }

    void Tilemap::ResetBreakables()
    {
        mBreakables.clear();
    }

    void Tilemap::SetPlayerStartingPosition(const int &i, const int &j)
    {
        mPlayerStart.i = i;
        mPlayerStart.j = j;
        if (mInitialized)
            InitPlayer();
    }

    void Tilemap::InitScenario()
    {
        for (size_t i = 0; i < mTileEntities.size(); ++i)
        {
            mWorld->DestroyEntity(mTileEntities[i]);
        }
        mTileEntities.clear();

        for (size_t i = 0; i < mTiles.size(); ++i)
        {
            auto tile = mTiles[i];
            AddTileEntity(tile);
        }
    }

    void Tilemap::InitEnemies()
    {
        for (size_t i = 0; i < mEnemyEntities.size(); ++i)
        {
            mWorld->DestroyEntity(mEnemyEntities[i]);
        }
        mEnemyEntities.clear();

        for (size_t i = 0; i < mEnemies.size(); ++i)
        {
            auto enemy = mEnemies[i];
            AddEnemyEntity(enemy);
        }
    }

    void Tilemap::InitCoins()
    {
        for (size_t i = 0; i < mCoinEntities.size(); ++i)
        {
            mWorld->DestroyEntity(mCoinEntities[i]);
        }
        mCoinEntities.clear();

        ECS::AnimationComponent animationComponent{"Idle"};
        auto animation = mAnimationFactory->GetAnimationsData("Coin.json");
        auto coinTex = mTextureFactory->GetTexture(animation->TexturePath.string());
        for (size_t z = 0; z < mCoins.size(); ++z)
        {
            auto coin = mCoins[z];
            AddCoinEntity(coin);
        }
    }

    void Tilemap::InitBreakables()
    {
        for (size_t i = 0; i < mBreakableEntities.size(); ++i)
        {
            mWorld->DestroyEntity(mBreakableEntities[i]);
        }
        mBreakableEntities.clear();

        for (size_t z = 0; z < mBreakables.size(); ++z)
        {
            auto breakable = mBreakables[z];
            AddBreakableEntity(breakable);
        }
    }

    void Tilemap::InitPlayer()
    {
        mWorld->DestroyEntity(mPlayerEntity);
        mPlayerEntity = mWorld->CreateEntity();

        auto animation = mAnimationFactory->GetAnimationsData("Player.json");
        auto playerTex = mTextureFactory->GetTexture(animation->TexturePath.string());
        ECS::AnimationComponent animationComponent{"Idle"};
        auto playerTile = GetTile(mPlayerStart.i, mPlayerStart.j);
        auto playerPos = playerTile.GetPosition(); // + playerTile.GetSize() / 2.0f;
        mWorld->Set<ECS::TransformComponent>(mPlayerEntity, ECS::TransformComponent{playerPos, 1.0f});
        mWorld->Set<PlayerComponent>(mPlayerEntity, PlayerComponent{5.0f});
        mWorld->Set<ECS::SpriteComponent>(mPlayerEntity, ECS::SpriteComponent{playerTex.get(), mZindexes.Player});
        mWorld->Set<ECS::ColliderComponent>(mPlayerEntity, ECS::ColliderComponent(
                                                               Physics::Rectangle{Vector2f{0.0f, 0.0f}, PixelsToMeters(Vector2f(7.0f, 15.0f))},
                                                               PlatformerLayers::Player,
                                                               PlatformerLayers::Enemy + PlatformerLayers::Coin,
                                                               false));
        mWorld->Set<ECS::CharacterController>(mPlayerEntity, ECS::CharacterController{5.0f, 100.0f, 20.0f, -20.0f, 20.0f, 0.1f, true, PlatformerLayers::Scenario});
        mWorld->Set<ECS::AnimationComponent>(mPlayerEntity, animationComponent);

        mWorld->Set<Mochi::Graphics::AnimationsData>(mPlayerEntity, *(animation.get()));
    }

    void Tilemap::InitMap()
    {
        InitScenario();
        InitCoins();
        InitBreakables();
        InitEnemies();
        InitPlayer();
    }

    void Tilemap::GenerateRenderCommands()
    {
        mRenderCommandCache.clear();
        Vector2f tileSize{mProperties.TileSize, mProperties.TileSize};
        for (size_t i = 0; i < mTiles.size(); ++i)
        {
            TileSpaceStatus tss = 0;
            auto tile = mTiles[i];

            if (tile.i > 0)
            {
                if (HasTileAt(tile.i - 1, tile.j))
                    tss += 2;
            }
            if (tile.i < mProperties.Width)
            {
                if (HasTileAt(tile.i + 1, tile.j))
                    tss += 4;
            }
            if (tile.j > 0)
            {
                if (HasTileAt(tile.i, tile.j - 1))
                    tss += 8;
            }
            if (tile.j < mProperties.Height)
            {
                if (HasTileAt(tile.i, tile.j + 1))
                    tss += 1;
            }

            Graphics::RenderCommand rc = mTilesets[mTilesetIds[tile.TilesetIndex]]->GetTileCommand(tss);
            auto tileRect = GetTile(tile.i, tile.j);
            tileRect.SetSize(tileSize);
            rc.destRect = tileRect;
            rc.zindex = mZindexes.Scenario;
            mRenderCommandCache.push_back(rc);
        }
    }

    std::vector<std::string> Tilemap::GetTilesetIds() const
    {
        return mTilesetIds;
    }

    std::vector<std::string> Tilemap::GetBreakableIds() const
    {
        std::vector<std::string> keys{};
        for (auto &element : mBreakableTypes)
        {
            keys.push_back(element.first);
        }
        return keys;
    }

    std::vector<std::string> Tilemap::GetEnemyIds() const
    {
        std::vector<std::string> keys{};
        for (auto &element : mEnemyTypes)
        {
            keys.push_back(element.first);
        }
        return keys;
    }

#if DEBUG
    void Tilemap::DebugGizmos(Debug::IGizmos *gizmos, Graphics::Camera *camera) const
    {
        const Color color = Color{255, 255, 0, 30};

        for (int i = 0; i < mProperties.Width; ++i)
        {
            Vector2f start{i * mRealSquareSize, 0.0f};
            Vector2f end{i * mRealSquareSize, mProperties.Height * mRealSquareSize};
            start -= camera->GetPosition();
            end -= camera->GetPosition();
            Physics::Line l{start, end};
            gizmos->DrawLine(&l, color);
        }
        for (int j = 0; j < mProperties.Height; ++j)
        {
            Vector2f start{0.0f, j * mRealSquareSize};
            Vector2f end{mProperties.Width * mRealSquareSize, j * mRealSquareSize};
            start -= camera->GetPosition();
            end -= camera->GetPosition();
            Physics::Line l{start, end};
            gizmos->DrawLine(&l, color);
        }

        Engine &e = Engine::Get();
        auto gui = e.GetGUI();
        Graphics::GUITextOptions options;
        options.TextSize = 16.0f;
        Vector2f size = mRealSquareSize;
        Vector2f cameraPos = MetersToPixels(camera->GetPosition());
        cameraPos.y = -cameraPos.y;
        float halfTileSize = mProperties.TileSize / 2.0f;
        for (int i = 0; i < mProperties.Width; ++i)
        {
            for (int j = 0; j < mProperties.Height; ++j)
            {
                Vector2f pos = Vector2f(i * mProperties.TileSize + halfTileSize, -j * mProperties.TileSize - halfTileSize) - cameraPos;
                options.DstRect = Rectf(pos, size);
                gui->Text(std::format("{}-{}", i, j).c_str(), options);
            }
        }
    }
#endif
}