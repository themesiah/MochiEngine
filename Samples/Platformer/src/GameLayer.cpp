#include "GameLayer.h"

#include "Graphics/IRenderer.h"
#include "Graphics/IAnimationFactory.h"
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
#include "Components/PlayerComponent.h"

namespace Mochi::Platformer
{
    GameLayer::GameLayer() : Layer()
    {
        mCatalog->OpenPack("Data/Game");
        mAudioManager->LoadAudio("Audio.json");
        mTextureFactory = mRenderer->CreateTextureFactory(mCatalog);
        mAnimationFactory = std::make_unique<Graphics::AsepriteAnimationFactory>(mCatalog);
    }

    GameLayer::~GameLayer()
    {
    }

    void GameLayer::InitLayer()
    {
        mECSWorld->RegisterSystem<PlayerMovementSystem>();

        mPlayerEntity = mECSWorld->CreateEntity();
        auto playerTex = mTextureFactory->GetTexture("Player.png");
        mECSWorld->Set<ECS::TransformComponent>(mPlayerEntity, ECS::TransformComponent{Vector2f{0.0f, 0.0f}, 1.0f});
        mECSWorld->Set<PlayerComponent>(mPlayerEntity, PlayerComponent{5.0f});
        mECSWorld->Set<ECS::SpriteComponent>(mPlayerEntity, ECS::SpriteComponent{playerTex.get(), 1});
        mECSWorld->Set<ECS::ColliderComponent>(mPlayerEntity, ECS::ColliderComponent(
                                                                  Physics::Rectangle{Vector2f{0.0f, 0.0f}, PixelsToMeters(Vector2f(7.0f, 15.0f))},
                                                                  1,
                                                                  2,
                                                                  false));
        mECSWorld->Set<ECS::CharacterController>(mPlayerEntity, ECS::CharacterController{5.0f, 100.0f, 20.0f, -20.0f, 20.0f, 0.1f, true});

        auto blockTex = mTextureFactory->GetTexture("Block.png");
        auto blockSize = PixelsToMeters(blockTex->GetSize());
        std::vector<Vector2f> blockPositions = {
            {-2.0f, -1.0f},
            {0.0f, -2.0f},
            {blockSize.x * -1, -2.0f},
            {blockSize.x * 1, -2.0f},
            {blockSize.x * -2, -2.0f},
            {blockSize.x * 2, -2.0f},
            {blockSize.x * -3, -2.0f},
            {blockSize.x * 3, -2.0f},
            {blockSize.x * -4, -2.0f},
            {blockSize.x * 4, -2.0f},
        };
        for (size_t i = 0; i < blockPositions.size(); ++i)
        {
            auto blockEntity = mECSWorld->CreateEntity();
            mBlocksEntities.push_back(blockEntity);
            mECSWorld->Set<ECS::TransformComponent>(blockEntity, ECS::TransformComponent{blockPositions[i], 1.0f});
            mECSWorld->Set<ECS::SpriteComponent>(blockEntity, ECS::SpriteComponent{blockTex.get(), 0});
            mECSWorld->Set<ECS::ColliderComponent>(blockEntity, ECS::ColliderComponent(
                                                                    Physics::Rectangle{Vector2f{0.0f, 0.0f}, PixelsToMeters(blockTex->GetSize() / 2.0f)},
                                                                    2,
                                                                    0,
                                                                    false));
        }
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
            ECS::AnimationComponent animationComponent{};
            animationComponent.CurrentAnimation = "Down";
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
}