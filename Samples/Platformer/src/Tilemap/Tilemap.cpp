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

#include "Tileset.h"
#include "Tile.h"

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
#endif

namespace Mochi::Platformer
{
    Tilemap::Tilemap(ECS::ECSWorld *world, FS::PackCatalog *catalog, Graphics::AbstractTextureFactory *textureFactory, Graphics::IAnimationFactory *animationFactory)
        : mWorld(world), mTilesets({}), mTiles({}), mRenderCommandCache({}), mWidth(0), mHeight(0), mSquareSize(0.0f), mSquareSizePixels(0.0f),
          mCatalog(catalog), mTextureFactory(textureFactory), mAnimationFactory(animationFactory)
    {
    }

    Tilemap::~Tilemap()
    {
    }

    Rectf Tilemap::GetTile(const int &i, const int &j) const
    {
        if (i >= mWidth || j >= mHeight)
            throw EngineError("Tilemap is not that big");

        float x = ((float)i - mWidth / 2) * mSquareSize - mSquareSize / 2;
        float y = ((float)j - mHeight / 2) * mSquareSize - mSquareSize / 2;
        return Rectf{x, y, mSquareSizePixels, mSquareSizePixels};
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

    void Tilemap::LoadTilemap(const std::string &tilemapData)
    {
        std::vector<char> raw = mCatalog->GetFile(tilemapData);
        nlohmann::json data = nlohmann::json::parse(raw);

        // Main properties
        auto properties = data.at("properties");
        mWidth = properties.at("width");
        mHeight = properties.at("height");
        mSquareSizePixels = properties.at("tileSize");
        mSquareSize = PixelsToMeters(mSquareSizePixels);

        // Load tilesets
        auto tileSets = data.at("tilesets");
        LoadTilesets(tileSets);

        // Load main scenario
        auto scenario = data.at("scenario");
        auto zindexes = data.at("zindex");
        LoadScenario(scenario, zindexes);

        // Load enemies
        auto enemyTypes = data.at("enemyTypes");
        auto enemies = data.at("enemies");
        LoadEnemies(enemyTypes, enemies);

        // Load coins
        auto coins = data.at("coins");
        LoadCoins(coins);

        // Load breakables
        auto breakableTypes = data.at("breakableTypes");
        auto breakables = data.at("breakables");
        LoadBreakables(breakableTypes, breakables);

        // Initialize the player
        auto playerStart = data.at("playerStart");
        InitPlayer(playerStart);
    }

    void Tilemap::LoadTilesets(const nlohmann::json tilesets)
    {
        for (size_t i = 0; i < tilesets.size(); ++i)
        {
            Tileset ts = Tileset(tilesets[i], mAnimationFactory, mTextureFactory);
            mTilesets.push_back(ts);
        }
    }

    void Tilemap::LoadScenario(const nlohmann::json scenario, const nlohmann::json zindexes)
    {

        for (size_t i = 0; i < scenario.size(); ++i)
        {
            nlohmann::json tile = scenario[i];
            int set = tile.at("set");
            int x = tile.at("i");
            int y = tile.at("j");
            mTiles.push_back({x, y, set});
        }

        for (size_t i = 0; i < mTiles.size(); ++i)
        {
            TileSpaceStatus tss = 0;
            auto tile = mTiles[i];

            if (tile.i > 0)
            {
                if (HasTileAt(tile.i - 1, tile.j))
                    tss += 2;
            }
            if (tile.i < mWidth)
            {
                if (HasTileAt(tile.i + 1, tile.j))
                    tss += 4;
            }
            if (tile.j > 0)
            {
                if (HasTileAt(tile.i, tile.j - 1))
                    tss += 8;
            }
            if (tile.j < mHeight)
            {
                if (HasTileAt(tile.i, tile.j + 1))
                    tss += 1;
            }

            Graphics::RenderCommand rc = mTilesets[tile.TilesetIndex].GetTileCommand(tss);
            rc.destRect = GetTile(tile.i, tile.j);
            rc.zindex = zindexes.at("scenario");
            mRenderCommandCache.push_back(rc);

            auto blockEntity = mWorld->CreateEntity();
            mWorld->Set<ECS::TransformComponent>(blockEntity, ECS::TransformComponent{rc.destRect.GetPosition(), 1.0f});
            // mECSWorld->Set<ECS::SpriteComponent>(blockEntity, ECS::SpriteComponent{blockTex.get(), 0});
            mWorld->Set<ECS::ColliderComponent>(blockEntity, ECS::ColliderComponent(
                                                                 Physics::Rectangle{Vector2f{0.0f, 0.0f}, PixelsToMeters(rc.destRect.GetSize() / 2.0f)},
                                                                 PlatformerLayers::Scenario,
                                                                 0,
                                                                 false));
        }
    }

    void Tilemap::LoadEnemies(const nlohmann::json enemyTypes, const nlohmann::json enemies)
    {
        for (size_t z = 0; z < enemies.size(); ++z)
        {
            auto enemyEntity = mWorld->CreateEntity();
            auto enemy = enemies[z];
            int i = enemy["i"];
            int j = enemy["j"];
            int type = enemy["type"];
            auto enemyType = enemyTypes[type];

            std::string texPath;
            if (enemyType["sourceType"] == "Sprite")
            {
                texPath = enemyType["source"];
            } // TODO: source animation
            Graphics::ITexture *enemyTex = mTextureFactory->GetTexture(texPath).get();
            auto colliderSize = enemyType["colliderSize"];

            mWorld->Set<ECS::TransformComponent>(enemyEntity, ECS::TransformComponent{GetTile(i, j).GetPosition(), 1.0f});
            mWorld->Set<ECS::SpriteComponent>(enemyEntity, ECS::SpriteComponent{enemyTex, 3});
            mWorld->Set<ECS::ColliderComponent>(enemyEntity, ECS::ColliderComponent(
                                                                 Physics::Rectangle{Vector2f{0.0f, 0.0f}, PixelsToMeters(Vector2f{colliderSize[0], colliderSize[1]} / 2.0f)},
                                                                 PlatformerLayers::Enemy,
                                                                 PlatformerLayers::Player,
                                                                 false));
            mWorld->Set<ECS::CharacterController>(enemyEntity, ECS::CharacterController(2.0f, 100.0f, 20.0f, -20.0f, 20.0f, 0.1f, false, PlatformerLayers::Scenario));
            mWorld->Set<LeftRightComponent>(enemyEntity, LeftRightComponent{-1.0f, 2, 0.8f});
            mWorld->Set<EnemyComponent>(enemyEntity, EnemyComponent{});
        }
    }

    void Tilemap::LoadCoins(const nlohmann::json coins)
    {
        ECS::AnimationComponent animationComponent{"Idle"};
        auto animation = mAnimationFactory->GetAnimationsData("Coin.json");
        auto coinTex = mTextureFactory->GetTexture(animation->TexturePath.string());
        for (size_t z = 0; z < coins.size(); ++z)
        {
            auto coin = coins[z];
            auto coinEntity = mWorld->CreateEntity();
            int i = coin["i"];
            int j = coin["j"];
            mWorld->Set<ECS::TransformComponent>(coinEntity, ECS::TransformComponent{GetTile(i, j).GetPosition(), 1.0f});
            mWorld->Set<ECS::SpriteComponent>(coinEntity, ECS::SpriteComponent{coinTex.get(), 2});
            mWorld->Set<ECS::ColliderComponent>(coinEntity, ECS::ColliderComponent(
                                                                Physics::Circle{Vector2f{0.0f, 0.0f}, PixelsToMeters(12.0f)},
                                                                PlatformerLayers::Coin,
                                                                0,
                                                                false));
            mWorld->Set<CoinComponent>(coinEntity);
            mWorld->Set<ECS::AnimationComponent>(coinEntity, animationComponent);

            mWorld->Set<Mochi::Graphics::AnimationsData>(coinEntity, *(animation.get()));
        }
    }

    void Tilemap::LoadBreakables(const nlohmann::json breakableTypes, const nlohmann::json breakables)
    {
        for (size_t z = 0; z < breakables.size(); ++z)
        {
            auto breakable = breakables[z];
            int i = breakable["i"];
            int j = breakable["j"];
            int type = breakable["type"];
            auto breakableType = breakableTypes[type];

            std::string texPath;
            if (breakableType["sourceType"] == "Sprite")
            {
                texPath = breakableType["source"];
            }
            auto breakableTex = mTextureFactory->GetTexture(texPath);
            auto breakableEntity = mWorld->CreateEntity();
            mWorld->Set<ECS::TransformComponent>(breakableEntity, ECS::TransformComponent{GetTile(i, j).GetPosition(), 1.0f});
            mWorld->Set<ECS::SpriteComponent>(breakableEntity, ECS::SpriteComponent{breakableTex.get(), 2});
            mWorld->Set<ECS::ColliderComponent>(breakableEntity, ECS::ColliderComponent(
                                                                     Physics::Rectangle{Vector2f{0.0f, 0.0f}, PixelsToMeters(breakableTex->GetSize() / 2.0f)},
                                                                     PlatformerLayers::Scenario,
                                                                     0,
                                                                     false));
            mWorld->Set<BreakableComponent>(breakableEntity);
        }
    }

    void Tilemap::InitPlayer(const nlohmann::json playerStart)
    {
        auto animation = mAnimationFactory->GetAnimationsData("Player.json");
        auto playerTex = mTextureFactory->GetTexture(animation->TexturePath.string());
        auto playerEntity = mWorld->CreateEntity();
        int i = playerStart["i"];
        int j = playerStart["j"];
        ECS::AnimationComponent animationComponent{"Idle"};
        mWorld->Set<ECS::TransformComponent>(playerEntity, ECS::TransformComponent{GetTile(i, j).GetPosition(), 1.0f});
        mWorld->Set<PlayerComponent>(playerEntity, PlayerComponent{5.0f});
        mWorld->Set<ECS::SpriteComponent>(playerEntity, ECS::SpriteComponent{playerTex.get(), 4});
        mWorld->Set<ECS::ColliderComponent>(playerEntity, ECS::ColliderComponent(
                                                              Physics::Rectangle{Vector2f{0.0f, 0.0f}, PixelsToMeters(Vector2f(7.0f, 15.0f))},
                                                              PlatformerLayers::Player,
                                                              PlatformerLayers::Enemy + PlatformerLayers::Coin,
                                                              false));
        mWorld->Set<ECS::CharacterController>(playerEntity, ECS::CharacterController{5.0f, 100.0f, 20.0f, -20.0f, 20.0f, 0.1f, true, PlatformerLayers::Scenario});
        mWorld->Set<ECS::AnimationComponent>(playerEntity, animationComponent);

        mWorld->Set<Mochi::Graphics::AnimationsData>(playerEntity, *(animation.get()));
    }

    void Tilemap::Render() const
    {
        Engine::Get().AddRenderCommands(mRenderCommandCache);
    }

#if DEBUG
    void Tilemap::DebugGizmos(Debug::IGizmos *gizmos) const
    {
        const Color color = Color{255, 255, 0, 30};

        for (int i = -mWidth / 2; i < mWidth / 2; ++i)
        {
            Vector2f start{(float)i * mSquareSize, -mHeight / 2.0f * mSquareSize};
            Vector2f end{(float)i * mSquareSize, mHeight / 2.0f * mSquareSize};
            Physics::Line l{start, end};
            gizmos->DrawLine(&l, color);
        }
        for (int j = -mHeight / 2; j < mHeight / 2; ++j)
        {
            Vector2f start{-mWidth / 2.0f * mSquareSize, (float)j * mSquareSize};
            Vector2f end{mWidth / 2.0f * mSquareSize, (float)j * mSquareSize};
            Physics::Line l{start, end};
            gizmos->DrawLine(&l, color);
        }
    }
#endif
}