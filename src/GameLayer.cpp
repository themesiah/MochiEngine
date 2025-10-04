#include "GameLayer.h"
#include "Layer.h"

#include "Engine.h"

#include <memory>
#include "SDL3/SDL.h"

#include "Graphics/Camera.h"
#include "Graphics/SpriteBase.h"
#include "Graphics/TextureFactory.h"
#include "Graphics/AnimationFactory.h"
#include "Graphics/Renderer.h"
#include "GUI/GUI.h"
#include "Event/EventBus.h"

#include "Input/ActionManager.h"
#include "Packer/PackCatalog.h"
#include "ScriptingManager.h"
#include "Audio/FMODWrapper.h"

#include "Bullets/PlayerBulletPool.h"
#include "Player.h"
#include "ZIndexEnum.h"

#include "ShooterEvents.h"
#include "PointsSystem.h"

#include "Enemies/Enemy.h"

#if DEBUG
#include "Debug/Gizmos.hpp"
#endif

namespace Mochi::Shooter
{
    GameLayer::GameLayer(FS::PackCatalog *catalog, Scripting::ScriptingManager *scripting, Graphics::Renderer *renderer, Graphics::Camera *camera, Event::EventBus *eventBus, Graphics::GUI *gui, Input::ActionManager *actionManager)
        : Layer(), mCatalog(catalog), mScripting(scripting), mCamera(camera), mEventBus(eventBus), mGUI(gui), mActionManager(actionManager)
    {
        mCatalog->OpenPack("Data/Game");

        mScripting->ExecuteFile("Script/FMODCallbackDefinitionAlternative.lua");

        mTextureFactory = std::make_shared<Graphics::TextureFactory>(mCatalog, renderer->GetRenderer());
        mAnimationFactory = std::make_shared<Graphics::AnimationFactory>(mCatalog);

        mPlayer = std::make_shared<Player>(mAnimationFactory.get(), mTextureFactory.get(), mCamera);
        mPlayer->SetZIndex(ZINDEX_PLAYER);
        mPlayer->SetPosition({-2.0f, 0.0f});
        mPlayer->SetScale(2.0f);

        mPointsSystem = std::make_unique<PointsSystem>(mEventBus, mGUI);

        mEnemy = std::make_shared<Enemy>(mEventBus, mTextureFactory.get());
        mEnemy->SetZIndex(ZINDEX_ENEMY);
        mEnemy->SetPosition({6.0f, 1.0f});
        mEnemy->SetScale(2.0f);
    }

    GameLayer::~GameLayer()
    {
    }

    bool GameLayer::Update(const float &dt)
    {
        mPlayer->Update(dt, mActionManager);

        auto playerBulletPool = mPlayer->GetBulletPool();
        auto enemyShape = mEnemy->GetCollider();
        std::vector<int> collisions = playerBulletPool->CheckAgainst(enemyShape);
        for (size_t i = 0; i < collisions.size(); ++i)
        {
            playerBulletPool->ReleaseBullet(collisions[i]);
            mEnemy->ReceiveDamage(1);
        }

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
        engine.AddRenderCommand(mEnemy->GetRenderData());
    }

    void GameLayer::GUI() const
    {
        mPointsSystem->Draw();
    }

#if DEBUG
    void GameLayer::Debug() const
    {
        Engine &e = Engine::Get();
        auto renderer = e.GetRenderer()->GetRenderer();
        auto camera = e.GetCamera();

        auto enemyCollider = mEnemy->GetCollider();
        DrawRectangle(renderer, &enemyCollider, {255, 255, 0, SDL_ALPHA_OPAQUE});

                auto playerBullets = mPlayer->GetBulletPool();
        auto bulletPositions = playerBullets->GetPositions();
        Physics::Rectangle rect({0.0f, 0.0f}, PixelsToMeters(Vector2f{24.0f, 6.0f}));

        for (size_t i = 0; i < bulletPositions.size(); ++i)
        {
            rect.Position = bulletPositions[i];
            DrawRectangle(renderer, &rect, {255, 0, 0, SDL_ALPHA_OPAQUE});
        }
    }
#endif

}