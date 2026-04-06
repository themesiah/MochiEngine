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

#include "Systems/PlayerMovementSystem.h"
#include "Systems/LeftRightEnemySystem.h"
#include "Systems/CoinAcquirementSystem.h"
#include "Systems/BreakableSystem.h"
#include "Components/GlobalDataComponent.h"
#include "Tilemap/Tilemap.h"
#include "PlatformerLayers.h"

namespace Mochi::Platformer
{
    GameLayer::GameLayer() : Layer(), mBlocksEntities(), mEnemyEntities()
    {
        mCatalog->OpenPack("Data/Game");
        mAudioManager->LoadAudio("Audio.json");
        mTextureFactory = mRenderer->CreateTextureFactory(mCatalog);
        mAnimationFactory = std::make_unique<Graphics::AsepriteAnimationFactory>(mCatalog);
        mTilemap = std::make_unique<Tilemap>(mECSWorld, mCatalog, mTextureFactory.get(), mAnimationFactory.get());
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

        mTilemap->LoadTilemap("Tilemap1.json");
    }

    bool GameLayer::Update(const float &dt)
    {
        return true;
    }

    void GameLayer::Render() const
    {
        mTilemap->Render();
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
}