#include "GameLayer.h"

#include "Graphics/IRenderer.h"
#include "Graphics/IAnimationFactory.h"
#include "Graphics/AbstractTextureFactory.h"
#include "Graphics/AsepriteAnimationFactory.h"
#include "Graphics/AnimationData.h"

#include "Packer/PackCatalog.h"
#include "Audio/IAudioManager.h"
#include "Input/IActionManager.h"
#include "Utils/Conversion.hpp"

#include "ECS/ECSWorld.h"
#include "ECS/Components/ECSSprite.h"
#include "ECS/Components/ECSTransform.h"
#include "ECS/Components/ECSAnimation.h"
#include "ECS/Components/ECSCollider.h"
#include "ECS/Components/CharacterController.hpp"

#include "Systems/PlayerMovementSystem.h"
#include "Systems/LeftRightEnemySystem.h"
#include "Systems/CoinAcquirementSystem.h"
#include "Systems/BreakableSystem.h"
#include "Components/PlayerComponent.h"
#include "Components/LeftRightComponent.h"
#include "Components/EnemyComponent.h"
#include "Components/GlobalDataComponent.h"
#include "Components/CoinComponent.h"
#include "Components/BreakableComponent.h"
#include "Tilemap.h"
#include "PlatformerLayers.h"

namespace Mochi::Platformer
{
    GameLayer::GameLayer() : Layer(), mBlocksEntities(), mEnemyEntities()
    {
        mCatalog->OpenPack("Data/Game");
        mAudioManager->LoadAudio("Audio.json");
        mTextureFactory = mRenderer->CreateTextureFactory(mCatalog);
        mAnimationFactory = std::make_unique<Graphics::AsepriteAnimationFactory>(mCatalog);
        mTilemap = std::make_unique<Tilemap>(mECSWorld, 30, 30, 32);
    }

    GameLayer::~GameLayer()
    {
    }

    void GameLayer::InitLayer()
    {
        mECSWorld->RegisterSystem<PlayerMovementSystem>();
        mECSWorld->RegisterSystem<LeftRightEnemySystem>();
        mECSWorld->RegisterSystem<CoinAcquirementSystem>();
        mECSWorld->RegisterSystem<BreakableSystem>();
        mECSWorld->SetGlobal<GlobalDataComponent>();

        InitPlayer();
        InitScenario();
        InitEnemies();
        InitSpecialTiles();
    }

    bool GameLayer::Update(const float &dt)
    {
        if (mActionManager->Performed("Debug1"))
        {
            mAudioManager->PlayOneShot("WilhelmScream");
        }

        if (mActionManager->Performed("Debug2"))
        {
            auto entity = mECSWorld->CreateEntity();
            mECSWorld->Set<ECS::TransformComponent>(entity, ECS::TransformComponent{Vector2f(0.0f, 0.0f), 1.0f});
            mECSWorld->Set<ECS::SpriteComponent>(entity, ECS::SpriteComponent{mTextureFactory->GetTexture("TestSprite.png").get(), 0});
        }

        if (mActionManager->Performed("Debug3"))
        {
            auto animation = mAnimationFactory->GetAnimationsData("TestSprite2.json");
            auto sprite = mTextureFactory->GetTexture(animation->TexturePath.string());

            auto entity = mECSWorld->CreateEntity();
            auto spriteComponent = ECS::SpriteComponent{sprite.get(), 0};
            ECS::AnimationComponent animationComponent{"Down"};
            mECSWorld->Set<ECS::TransformComponent>(entity, ECS::TransformComponent{Vector2f(0.0f, 0.0f), 1.0f});
            mECSWorld->Set<ECS::SpriteComponent>(entity, spriteComponent);
            mECSWorld->Set<Mochi::Graphics::AnimationsData>(entity, *(animation.get()));
            mECSWorld->Set<ECS::AnimationComponent>(entity, animationComponent);
        }

        return true;
    }

    void GameLayer::Render() const
    {
    }

    void GameLayer::GUI()
    {
    }

#if DEBUG
    void GameLayer::Debug() const
    {
        mTilemap->DebugGizmos(mGizmos);
    }
#endif

    void GameLayer::InitPlayer()
    {
        auto animation = mAnimationFactory->GetAnimationsData("Player.json");
        auto playerTex = mTextureFactory->GetTexture(animation->TexturePath.string());
        mPlayerEntity = mECSWorld->CreateEntity();
        ECS::AnimationComponent animationComponent{"Idle"};
        mECSWorld->Set<ECS::TransformComponent>(mPlayerEntity, ECS::TransformComponent{mTilemap->GetTile(10, 15).GetPosition(), 1.0f});
        mECSWorld->Set<PlayerComponent>(mPlayerEntity, PlayerComponent{5.0f});
        mECSWorld->Set<ECS::SpriteComponent>(mPlayerEntity, ECS::SpriteComponent{playerTex.get(), 3});
        mECSWorld->Set<ECS::ColliderComponent>(mPlayerEntity, ECS::ColliderComponent(
                                                                  Physics::Rectangle{Vector2f{0.0f, 0.0f}, PixelsToMeters(Vector2f(7.0f, 15.0f))},
                                                                  PlatformerLayers::Player,
                                                                  PlatformerLayers::Enemy + PlatformerLayers::Coin,
                                                                  false));
        mECSWorld->Set<ECS::CharacterController>(mPlayerEntity, ECS::CharacterController{5.0f, 100.0f, 20.0f, -20.0f, 20.0f, 0.1f, true, PlatformerLayers::Scenario});
        mECSWorld->Set<ECS::AnimationComponent>(mPlayerEntity, animationComponent);

        mECSWorld->Set<Mochi::Graphics::AnimationsData>(mPlayerEntity, *(animation.get()));
    }

    void GameLayer::InitScenario()
    {
        auto blockTex = mTextureFactory->GetTexture("Block.png");
        std::vector<Vector2f> blockPositions = {
            mTilemap->GetTile(9, 14).GetPosition(),
            mTilemap->GetTile(9, 15).GetPosition(),
            mTilemap->GetTile(9, 16).GetPosition(),
            mTilemap->GetTile(9, 17).GetPosition(),
            mTilemap->GetTile(9, 18).GetPosition(),
            mTilemap->GetTile(10, 14).GetPosition(),
            mTilemap->GetTile(11, 14).GetPosition(),
            mTilemap->GetTile(12, 14).GetPosition(),
            mTilemap->GetTile(12, 15).GetPosition(),
            mTilemap->GetTile(13, 14).GetPosition(),
            mTilemap->GetTile(14, 14).GetPosition(),
            mTilemap->GetTile(15, 14).GetPosition(),
            mTilemap->GetTile(15, 16).GetPosition(),
            mTilemap->GetTile(16, 14).GetPosition(),
            mTilemap->GetTile(17, 14).GetPosition(),
        };
        for (size_t i = 0; i < blockPositions.size(); ++i)
        {
            auto blockEntity = mECSWorld->CreateEntity();
            mBlocksEntities.push_back(blockEntity);
            mECSWorld->Set<ECS::TransformComponent>(blockEntity, ECS::TransformComponent{blockPositions[i], 1.0f});
            mECSWorld->Set<ECS::SpriteComponent>(blockEntity, ECS::SpriteComponent{blockTex.get(), 0});
            mECSWorld->Set<ECS::ColliderComponent>(blockEntity, ECS::ColliderComponent(
                                                                    Physics::Rectangle{Vector2f{0.0f, 0.0f}, PixelsToMeters(blockTex->GetSize() / 2.0f)},
                                                                    PlatformerLayers::Scenario,
                                                                    0,
                                                                    false));
        }
    }

    void GameLayer::InitEnemies()
    {
        auto enemyTex = mTextureFactory->GetTexture("Enemy.png");
        std::vector<Vector2f> blockPositions = {
            mTilemap->GetTile(17, 15).GetPosition(),
        };
        for (size_t i = 0; i < blockPositions.size(); ++i)
        {
            auto enemyEntity = mECSWorld->CreateEntity();
            mBlocksEntities.push_back(enemyEntity);
            mECSWorld->Set<ECS::TransformComponent>(enemyEntity, ECS::TransformComponent{blockPositions[i], 1.0f});
            mECSWorld->Set<ECS::SpriteComponent>(enemyEntity, ECS::SpriteComponent{enemyTex.get(), 2});
            mECSWorld->Set<ECS::ColliderComponent>(enemyEntity, ECS::ColliderComponent(
                                                                    Physics::Rectangle{Vector2f{0.0f, 0.0f}, PixelsToMeters(Vector2f{24.0f, 24.0f} / 2.0f)},
                                                                    PlatformerLayers::Enemy,
                                                                    PlatformerLayers::Player,
                                                                    false));
            mECSWorld->Set<ECS::CharacterController>(enemyEntity, ECS::CharacterController(2.0f, 100.0f, 20.0f, -20.0f, 20.0f, 0.1f, false, PlatformerLayers::Scenario));
            mECSWorld->Set<LeftRightComponent>(enemyEntity, LeftRightComponent{-1.0f, 2, 0.8f});
            mECSWorld->Set<EnemyComponent>(enemyEntity, EnemyComponent{});
        }
    }

    void GameLayer::InitSpecialTiles()
    {
        auto coinTex = mTextureFactory->GetTexture("Coin.png");
        std::vector<Vector2f> coinPositions = {
            mTilemap->GetTile(15, 17).GetPosition(),
        };
        for (size_t i = 0; i < coinPositions.size(); ++i)
        {
            auto coinEntity = mECSWorld->CreateEntity();
            mBlocksEntities.push_back(coinEntity);
            mECSWorld->Set<ECS::TransformComponent>(coinEntity, ECS::TransformComponent{coinPositions[i], 1.0f});
            mECSWorld->Set<ECS::SpriteComponent>(coinEntity, ECS::SpriteComponent{coinTex.get(), 1});
            mECSWorld->Set<ECS::ColliderComponent>(coinEntity, ECS::ColliderComponent(
                                                                   Physics::Circle{Vector2f{0.0f, 0.0f}, PixelsToMeters(12.0f)},
                                                                   PlatformerLayers::Coin,
                                                                   0,
                                                                   false));
            mECSWorld->Set<CoinComponent>(coinEntity);
        }

        auto breakableTex = mTextureFactory->GetTexture("BreakableBlock.png");
        std::vector<Vector2f> breakablePositions = {
            mTilemap->GetTile(15, 19).GetPosition(),
        };
        for (size_t i = 0; i < breakablePositions.size(); ++i)
        {
            auto breakableEntity = mECSWorld->CreateEntity();
            mBlocksEntities.push_back(breakableEntity);
            mECSWorld->Set<ECS::TransformComponent>(breakableEntity, ECS::TransformComponent{breakablePositions[i], 1.0f});
            mECSWorld->Set<ECS::SpriteComponent>(breakableEntity, ECS::SpriteComponent{breakableTex.get(), 1});
            mECSWorld->Set<ECS::ColliderComponent>(breakableEntity, ECS::ColliderComponent(
                                                                        Physics::Rectangle{Vector2f{0.0f, 0.0f}, PixelsToMeters(breakableTex->GetSize() / 2.0f)},
                                                                        PlatformerLayers::Scenario,
                                                                        0,
                                                                        false));
            mECSWorld->Set<BreakableComponent>(breakableEntity);
        }
    }
}