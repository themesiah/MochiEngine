#include "GameLayer.h"

#include "Graphics/IRenderer.h"
#include "Graphics/IAnimationFactory.h"
#include "Graphics/AsepriteAnimationFactory.h"
#include "Graphics/AnimationData.h"

#include "Packer/PackCatalog.h"
#include "Audio/IAudioManager.h"
#include "Input/IActionManager.h"

#include "ECS/ECSWorld.h"
#include "ECS/Components/ECSSprite.h"
#include "ECS/Components/ECSTransform.h"
#include "ECS/Components/ECSAnimation.h"

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