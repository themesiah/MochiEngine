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
#include "Event/EngineEvents.h"
#include "Utils/Conversion.hpp"

#include "Bullets/AbstractBulletPool.h"
#include "Bullets/PlayerBulletPool.h"
#include "Player.h"
#include "ZIndexEnum.h"

#include "ShooterEvents.h"
#include "PointsSystem.h"

#include "Enemies/AbstractEnemy.h"
#include "Enemies/Enemy.h"
#include "Enemies/Enemy2.h"
#include "Bullets/EnemyBulletPoolFollow.h"
#include "Bullets/EnemyBulletPoolDirection.h"
#include "Enemies/Boss.h"

#include "Background.h"
#include "Config.hpp"

#if DEBUG
#include "Debug/IGizmos.h"
#endif

namespace Mochi::Shooter
{
    const std::string EXPLOSION_ANIMATION_PATH = "Explosion.json";
    const std::string EXPLOSION_ANIMATION_MAIN = "Explosion";

    GameLayer::GameLayer()
        : Layer(), mLevelCompleted(false), mEnemies(), mEnemiesMarkedForDestruction(), mVFXList(), mVFXMarkedForDestruction(), mEnemyBulletPools()
    {
        mCatalog->OpenPack("Data/Game");
        mConfig = std::make_unique<Config>(mAudioManager);

        mTextureFactory = mRenderer->CreateTextureFactory(mCatalog);
        mAnimationFactory = std::make_unique<Graphics::AsepriteAnimationFactory>(mCatalog);

        mPlayer = std::make_shared<Player>(mAnimationFactory.get(), mTextureFactory.get(), mCamera, mActionManager, mEventBus, mGUI);
        mPlayer->GetTransform()->Position = Vector2f({-2.0f, 0.0f});

        mBackground = std::make_shared<Background>(mTextureFactory.get());

        mPointsSystem = std::make_unique<PointsSystem>(mEventBus, mGUI);

        mEnemyBulletPools.push_back(std::make_unique<EnemyBulletPoolFollow>(
            std::make_shared<Graphics::SpriteBase>(mTextureFactory.get(), "EnemyBullets.png"), 1000, 5.0f, 15.0f, mPlayer));
        mEnemyBulletPools.push_back(std::make_unique<EnemyBulletPoolDirection>(
            std::make_shared<Graphics::SpriteBase>(mTextureFactory.get(), "EnemyBullets.png"), 1000, 5.0f, 15.0f, mPlayer));

        mEnemyDestroyedSubscription = mEventBus->Subscribe<EnemyDestroyedEvent>(
            [&](const EnemyDestroyedEvent &e)
            {
                std::unique_ptr<Graphics::OneshotAnimation> destructionVfx = std::make_unique<Graphics::OneshotAnimation>(
                    mAnimationFactory.get(),
                    mTextureFactory.get(),
                    EXPLOSION_ANIMATION_PATH,
                    EXPLOSION_ANIMATION_MAIN);

                destructionVfx->SetZIndex(ZINDEX_ENEMYVFX);
                destructionVfx->GetTransform()->Position = e.Enemy->GetTransform()->Position;
                destructionVfx->GetTransform()->Scale = e.Enemy->GetTransform()->Scale * 2 * e.ExplosionScale;

                auto ptr = destructionVfx.get();
                destructionVfx->SetFinishCallback([&, ptr]()
                                                  { mVFXMarkedForDestruction.push_back(ptr); });
                mVFXList.push_back(std::move(destructionVfx));

                // mAudioManager->PlayOneShot("Explosion");

                if (auto boss = dynamic_cast<Boss *>(e.Enemy))
                {
                    LOG_INFO("BOSS DESTROYED!");
                    mLevelCompleted = true;
                }
            });

        mPlayerDestroyedSubscription = mEventBus->Subscribe<PlayerDeadEvent>(
            [&](const PlayerDeadEvent &e)
            {
                std::unique_ptr<Graphics::OneshotAnimation> destructionVfx = std::make_unique<Graphics::OneshotAnimation>(
                    mAnimationFactory.get(),
                    mTextureFactory.get(),
                    EXPLOSION_ANIMATION_PATH,
                    EXPLOSION_ANIMATION_MAIN);

                destructionVfx->GetTransform()->Position = e.Player->GetTransform()->Position;
                destructionVfx->GetTransform()->Scale = e.Player->GetTransform()->Scale * 2.0f;

                auto ptr = destructionVfx.get();
                destructionVfx->SetFinishCallback([&, ptr]()
                                                  { mVFXMarkedForDestruction.push_back(ptr); });
                mVFXList.push_back(std::move(destructionVfx));
            });
    }

    void GameLayer::InitLayer()
    {
        BindLuaTypesAndFunctions();
        mScripting->State["player"] = mPlayer;
        mScripting->State["bg"] = mBackground;
        mScripting->ExecuteFileGlobal("ShooterCore.lua");
        mScripting->ExecuteFile("Level1Setup.lua");

        mConfig->Init();
    }

    GameLayer::~GameLayer()
    {
        mEventBus->Unsubscribe<EnemyDestroyedEvent>(mEnemyDestroyedSubscription);
        mEventBus->Unsubscribe<EnemyDestroyedEvent>(mPlayerDestroyedSubscription);

        mCatalog->ClosePack("Data/Game");
    }

    bool GameLayer::Update(const float &dt)
    {
        mConfig->Update(dt, mActionManager);

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

        for (auto &bulletPool : mEnemyBulletPools)
        {
            if (mLevelCompleted)
            {
                bulletPool->ReleaseAllBullets();
            }
            bulletPool->Update(dt);
            std::vector<int> collisions = bulletPool->CheckAgainst(playerCollider);
            for (size_t i = 0; i < collisions.size(); ++i)
            {
                playerBulletPool->ReleaseBullet(collisions[i]);
                mPlayer->ReceiveDamage();
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
                               [&](const std::shared_ptr<AbstractEnemy> &enemy)
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

        return true;
    }

    void GameLayer::Render() const
    {
        auto &engine = Engine::Get();
        engine.AddRenderCommands(mPlayer->GetRenderData());
        engine.AddRenderCommands(mPlayer->GetBulletPool()->GetRenderData());
        for (auto &enemy : mEnemies)
        {
            engine.AddRenderCommands(enemy->GetRenderData());
        }
        for (auto &pool : mEnemyBulletPools)
        {
            engine.AddRenderCommands(pool->GetRenderData());
        }
        for (auto &vfx : mVFXList)
        {
            engine.AddRenderCommands(vfx->GetRenderData());
        }

        engine.AddRenderCommands(mBackground->GetRenderData());
    }

    void GameLayer::GUI()
    {
        if (mLevelCompleted)
        {
            const Graphics::GUIOptions backgroundOptions{
                .TexturePath = "Sprites/Black.png",
                .DstRect = Graphics::FULL_SCREEN_DST,
                .ScreenAnchor = Graphics::GUI_MIDDLE_CENTER,
                .SpritePivot = Graphics::GUI_MIDDLE_CENTER,
                .Color = Color(0, 0, 0, 70)};

            mGUI->Sprite(backgroundOptions);

            const Graphics::GUITextOptions titleTextOptions{
                .DstRect = {Rectf({0.0f, -50.0f}, {})},
                .ScreenAnchor = Graphics::GUI_MIDDLE_CENTER,
                .TextPivot = Graphics::GUI_MIDDLE_CENTER,
                .TextSize = 82.0f};

            const Graphics::GUITextOptions pointsTextOptions{
                .DstRect = {Rectf({0.0f, 50.0f}, {})},
                .ScreenAnchor = Graphics::GUI_MIDDLE_CENTER,
                .TextPivot = Graphics::GUI_MIDDLE_CENTER,
                .TextSize = 64.0f};

            const Graphics::GUIOptions button1OptionsBase{
                .TexturePath = "Interface.png",
                .SrcRect = {Rectf({0.0f, 0.0f}, {32.0f, 32.0f})},
                .DstRect = {Rectf({0.0f, 100.0f}, {200.0f, 50.0f})},
                .Slice = {Rectf(9.0f, 9.0f, 9.0f, 9.0f)},
                .ScreenAnchor = Graphics::GUI_MIDDLE_CENTER,
                .SpritePivot = Graphics::GUI_MIDDLE_CENTER};

            Graphics::GUIButtonOptions button1Options{
                .BaseOptions = button1OptionsBase,
                .FocusedOptions = button1OptionsBase,
                .PressedOptions = button1OptionsBase};
            button1Options.FocusedOptions.SrcRect.value().SetPosition({32.0f, 0.0f});
            button1Options.PressedOptions.SrcRect.value().SetPosition({0.0f, 32.0f});

            const Graphics::GUITextOptions buttonTextOptions{
                .ScreenAnchor = Graphics::GUI_MIDDLE_CENTER,
                .TextPivot = Graphics::GUI_MIDDLE_CENTER,
                .TextSize = 32.0f};

            mGUI->Text("YOU WON!", titleTextOptions);
            mGUI->Text(std::format("FINAL SCORE: {}", mPointsSystem->GetPoints()).c_str(), pointsTextOptions);

            if (mGUI->Button(button1Options, "EXIT", buttonTextOptions).Released)
            {
                mEventBus->Publish<ApplicationQuitEvent>({});
            }
        }
        else
        {
            mPointsSystem->Draw();
            mPlayer->GUI();
        }
    }

#if DEBUG
    void GameLayer::Debug() const
    {

        if (mActionManager->Performed("Debug3"))
        {
            mEnemyBulletPools[0]->AddBullet({0.0f, 0.0f});
        }

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

        Physics::Circle circle({0.0f, 0.0f}, PixelsToMeters(6.0f));
        for (auto &pool : mEnemyBulletPools)
        {
            auto enemyBulletPositions = pool->GetPositions();
            for (size_t i = 0; i < enemyBulletPositions.size(); ++i)
            {
                circle.Position = enemyBulletPositions[i];
                mGizmos->DrawCircle(&circle, {255, 0, 0, 255});
            }
        }
    }
#endif

    void GameLayer::BindLuaTypesAndFunctions()
    {
        mScripting->State.new_usertype<AbstractEnemy>(
            "AbstractEnemy",
            "GetTransform", &AbstractEnemy::GetTransform,
            "SetTransform", &AbstractEnemy::SetTransform,
            "IsDead", &AbstractEnemy::IsDead,
            "SetHealth", &AbstractEnemy::SetHealth,
            "SetExplosionScale", &AbstractEnemy::SetExplosionScale,
            "SetInvincible", &AbstractEnemy::SetInvincible);

        mScripting->State.new_usertype<Enemy>(
            "Enemy",
            sol::base_classes, sol::bases<AbstractEnemy>());
        mScripting->State.new_usertype<Enemy2>(
            "Enemy2",
            sol::base_classes, sol::bases<AbstractEnemy>());

        mScripting->State.new_enum<PlayerState>("PlayerState", {{"None", PlayerState::None},
                                                                {"Cutscene", PlayerState::Cutscene},
                                                                {"Damaged", PlayerState::Damaged},
                                                                {"Dead", PlayerState::Dead},
                                                                {"Playing", PlayerState::Playing},
                                                                {"Reespawning", PlayerState::Reespawning}});

        mScripting->State.new_usertype<Player>(
            "Player",
            sol::base_classes, sol::bases<Graphics::Spritesheet, Graphics::SpriteBase>(),
            "ChangeState", &Player::ChangeState);

        mScripting->State.new_usertype<Background>(
            "Background",
            sol::base_classes, sol::bases<Graphics::SpriteBase>(),
            "GetXPosition", &Background::GetXPosition,
            "SetXPosition", &Background::SetXPosition);

        mScripting->State.new_enum<BossState>("BossState", {{"Vertical", BossState::BossState_Vertical},
                                                            {"Horizontal", BossState::BossState_Horizontal},
                                                            {"Background", BossState::BossState_Background}});

        mScripting->State.new_usertype<Boss>(
            "Boss",
            sol::base_classes, sol::bases<AbstractEnemy>(),
            "SetState", &Boss::SetState);

        mScripting->State.set_function("CreateEnemy", &GameLayer::CreateEnemy, this);
        mScripting->State.set_function("CreateBoss", &GameLayer::CreateBoss, this);

        mScripting->State.set_function("DeleteEnemy", &GameLayer::DeleteEnemy, this);

        mScripting->State.set_function("ShotBullet", &GameLayer::ShotBullet, this);
        mScripting->State.set_function("SetBulletDirection", &GameLayer::SetBulletDirection, this);
    }

    std::shared_ptr<AbstractEnemy> GameLayer::CreateEnemy(const int &enemyType)
    {
        std::shared_ptr<AbstractEnemy> enemy;
        switch (enemyType)
        {
        case 0:
            enemy = std::make_shared<Enemy>(mEventBus, mTextureFactory.get(), mAnimationFactory.get());
            break;
        case 1:
            enemy = std::make_shared<Enemy2>(mEventBus, mTextureFactory.get(), mAnimationFactory.get());
            break;
        case 2:
            enemy = std::make_shared<Boss>(mEventBus, mTextureFactory.get(), mAnimationFactory.get());
            break;
        default:
            throw EngineError(std::format("Enemy type {} does not exist. Can't create.", enemyType));
        }
        mEnemies.push_back(enemy);
        return enemy;
    }

    std::shared_ptr<Boss> GameLayer::CreateBoss()
    {
        auto boss = std::make_shared<Boss>(mEventBus, mTextureFactory.get(), mAnimationFactory.get());
        mEnemies.push_back(boss);
        return boss;
    }

    void GameLayer::DeleteEnemy(std::shared_ptr<AbstractEnemy> enemyToDelete)
    {
        mEnemiesMarkedForDestruction.push_back(enemyToDelete.get());
    }

    int GameLayer::ShotBullet(const int &bulletPoolIndex, const Vector2f &bulletPosition)
    {
        if (mEnemyBulletPools.size() < bulletPoolIndex)
        {
            throw EngineError(std::format("There are not {} enemy bullet pools. Max is {}", bulletPoolIndex, mEnemyBulletPools.size()));
        }
        else
        {
            return mEnemyBulletPools[bulletPoolIndex]->AddBullet(bulletPosition);
        }
    }

    void GameLayer::SetBulletDirection(const int &bulletPoolIndex, const int &bulletIndex, const Vector2f &bulletDirection)
    {
        if (auto bulletPoolDirection = dynamic_cast<EnemyBulletPoolDirection *>(mEnemyBulletPools[bulletPoolIndex].get()))
        {
            bulletPoolDirection->SetBulletDirection(bulletIndex, bulletDirection);
        }
        else
        {
            throw EngineError(std::format("Bullet pool with index {} is not directionable", bulletPoolIndex));
        }
    }
}