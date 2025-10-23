#include "GameLayer.h"
#include "Layer.h"

#include "Engine.h"

#include <memory>
#include "SDL3/SDL.h"

#include "Graphics/Camera.h"
#include "Graphics/SpriteBase.h"
#include "Graphics/AbstractTextureFactory.h"
#include "Graphics/AsepriteAnimationFactory.h"
#include "Graphics/OneshotAnimation.h"
#include "Graphics/IRenderer.h"
#include "Graphics/SDL/SDLRenderer.h"
#include "GUI/AbstractGUI.h"
#include "Event/EventBus.h"
#include "Input/ActionManager.h"
#include "Packer/PackCatalog.h"
#include "Scripting/ScriptingManager.h"
#include "Audio/IAudioManager.h"
#include "Utils/Conversion.hpp"

#include "Bullets/PlayerBulletPool.h"
#include "Player.h"
#include "ZIndexEnum.h"

#include "ShooterEvents.h"
#include "PointsSystem.h"

#include "Enemies/Enemy.h"

#if DEBUG
#include "Debug/IGizmos.h"
#endif

namespace Mochi::Shooter
{
    const std::string EXPLOSION_ANIMATION_PATH = "Explosion.json";
    const std::string EXPLOSION_ANIMATION_MAIN = "Explosion";

    GameLayer::GameLayer()
        : Layer(), mEnemies(), mEnemiesMarkedForDestruction(), mVFXList(), mVFXMarkedForDestruction()
    {
        mCatalog->OpenPack("Data/Game");

        mTextureFactory = mRenderer->CreateTextureFactory(mCatalog);
        mAnimationFactory = std::make_unique<Graphics::AsepriteAnimationFactory>(mCatalog);

        BindLuaTypesAndFunctions();
        mScripting->ExecuteFileGlobal("ShooterCore.lua");
        mScripting->ExecuteFile("Level1Setup.lua");

        mPlayer = std::make_shared<Player>(mAnimationFactory.get(), mTextureFactory.get(), mCamera, mActionManager, mEventBus);
        mPlayer->SetPosition({-2.0f, 0.0f});

        mPointsSystem = std::make_unique<PointsSystem>(mEventBus, mGUI);

        mEnemyDestroyedSubscription = mEventBus->Subscribe<EnemyDestroyedEvent>(
            [&](const EnemyDestroyedEvent &e)
            {
                std::unique_ptr<Graphics::OneshotAnimation> destructionVfx = std::make_unique<Graphics::OneshotAnimation>(
                    mAnimationFactory.get(),
                    mTextureFactory.get(),
                    EXPLOSION_ANIMATION_PATH,
                    EXPLOSION_ANIMATION_MAIN);

                destructionVfx->SetPosition(e.Enemy->GetPosition());
                destructionVfx->SetScale(e.Enemy->GetScale() * 2);

                auto ptr = destructionVfx.get();
                destructionVfx->SetFinishCallback([&, ptr]()
                                                  { mVFXMarkedForDestruction.push_back(ptr); });
                mVFXList.push_back(std::move(destructionVfx));
            });
    }

    GameLayer::~GameLayer()
    {
        mEventBus->Unsubscribe<EnemyDestroyedEvent>(mEnemyDestroyedSubscription);
    }

    bool GameLayer::Update(const float &dt)
    {
        mPlayer->Update(dt);

        auto playerCollider = mPlayer->GetCollider();
        for (auto &enemy : mEnemies)
        {
            if (playerCollider.Collides(enemy->GetCollider()))
            {
                mPlayer->ReceiveDamage();
            }
        }

        auto playerBulletPool = mPlayer->GetBulletPool();

        for (auto &enemy : mEnemies)
        {
            enemy->Update(dt);
            auto enemyShape = enemy->GetCollider();
            std::vector<int> collisions = playerBulletPool->CheckAgainst(enemyShape);
            for (size_t i = 0; i < collisions.size(); ++i)
            {
                playerBulletPool->ReleaseBullet(collisions[i]);
                if (enemy->ReceiveDamage(1))
                {
                    mEnemiesMarkedForDestruction.push_back(enemy.get());
                }
            }
        }

        for (auto &vfx : mVFXList)
        {
            vfx->Update(dt);
        }

        for (auto &enemyToDestroy : mEnemiesMarkedForDestruction)
        {
            mEnemies.erase(
                std::remove_if(mEnemies.begin(), mEnemies.end(),
                               [&](const std::shared_ptr<Enemy> &enemy)
                               { return enemy.get() == enemyToDestroy; }),
                mEnemies.end());
        }

        for (auto &vfxToDestroy : mVFXMarkedForDestruction)
        {
            mVFXList.erase(
                std::remove_if(mVFXList.begin(), mVFXList.end(),
                               [&](const std::shared_ptr<Graphics::OneshotAnimation> &vfx)
                               { return vfx.get() == vfxToDestroy; }),
                mVFXList.end());
        }

        mEnemiesMarkedForDestruction.clear();
        mVFXMarkedForDestruction.clear();

        // mCamera->Move(mActionManager->Value("Horizontal") * dt * 1,
        //               mActionManager->Value("Vertical") * dt * 1);

        // if (mActionManager->Performed("Debug1"))
        // {
        //     mFmod->PlayOneShot("WilhelmScream");
        // }
        // if (mActionManager->Performed("Debug2"))
        // {
        //     mFmod->SetBusVolume("SFX", 0.1f);
        // }
        // if (mActionManager->Performed("Debug3"))
        // {
        //     mFmod->PlayBGM("TestMusic");
        // }
        // if (mActionManager->Performed("Debug4"))
        // {
        //     mFmod->SkipToTimelinePosition(30000);
        // }
        // if (mActionManager->Performed("Debug5"))
        // {
        //     mScripting->ExecuteFile("Script/TestScript.lua");
        // }

        return true;
    }

    void GameLayer::Render() const
    {
        auto &engine = Engine::Get();
        engine.AddRenderCommand(mPlayer->GetRenderData());
        engine.AddRenderCommands(mPlayer->GetBulletPool()->GetRenderData());
        for (auto &enemy : mEnemies)
        {
            engine.AddRenderCommand(enemy->GetRenderData());
        }
        for (auto &vfx : mVFXList)
        {
            engine.AddRenderCommand(vfx->GetRenderData());
        }
    }

    void GameLayer::GUI() const
    {
        mPointsSystem->Draw();
    }

#if DEBUG
    void GameLayer::Debug() const
    {
        Engine &e = Engine::Get();
        auto camera = e.GetCamera();

        auto collider = mPlayer->GetCollider();
        mGizmos->DrawRectangle(&collider, {255, 255, 0, 255});

        for (auto &enemy : mEnemies)
        {
            auto enemyCollider = enemy->GetCollider();
            mGizmos->DrawRectangle(&enemyCollider, {255, 0, 0, 255});
        }

        auto playerBullets = mPlayer->GetBulletPool();
        auto bulletPositions = playerBullets->GetPositions();
        Physics::Rectangle rect({0.0f, 0.0f}, PixelsToMeters(Vector2f{24.0f, 6.0f}));

        for (size_t i = 0; i < bulletPositions.size(); ++i)
        {
            rect.Position = bulletPositions[i];
            mGizmos->DrawRectangle(&rect, {255, 255, 0, 255});
        }
    }
#endif

    void GameLayer::BindLuaTypesAndFunctions()
    {
        mScripting->State.new_usertype<Enemy>(
            "Enemy",
            sol::base_classes, sol::bases<Graphics::SpriteBase>());

        mScripting->State.set_function(
            "CreateEnemy",
            [this]()
            {
                std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(mEventBus, mTextureFactory.get(), mAnimationFactory.get());
                mEnemies.push_back(enemy);
                return enemy;
            });

        mScripting->State.set_function(
            "DeleteEnemy",
            [this](std::shared_ptr<Enemy> deletableElement)
            {
                mEnemiesMarkedForDestruction.push_back(deletableElement.get());
            });
    }
}