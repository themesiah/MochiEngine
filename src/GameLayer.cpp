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

        mPointsSystem = std::make_unique<PointsSystem>(mEventBus, mGUI);

        mEnemy = std::make_shared<Enemy>(mEventBus, mTextureFactory.get());
        mEnemy->SetZIndex(ZINDEX_ENEMY);
        mEnemy->SetPosition({2.0f, 0.0f});
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

}