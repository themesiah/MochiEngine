#include "SpaceShooterEngine.h"

#include <iostream>

#include "Graphics/Camera.h"
#include "Graphics/SpriteBase.h"

#include "Input/ActionManager.h"
#include "Audio/FMODWrapper.h"
#include "ScriptingManager.h"
#include "Packer/PackCatalog.h"

#include "Bullets/PlayerBulletPool.h"
#include "Player.h"
#include "ZIndexEnum.h"

#include "ShooterEvents.h"
#include "PointsSystem.h"

#include "Enemies/Enemy.h"

namespace Mochi::Shooter
{
    SpaceShooterEngine::SpaceShooterEngine(const char *appName, const char *appVersion, const char *appId, const char *windowName)
        : Engine(appName, appVersion, appId, windowName)
    {
        mCatalog->OpenPack("Data/Game");

        mScripting->ExecuteFile("Script/FMODCallbackDefinitionAlternative.lua");

        mPlayer = std::make_shared<Player>(mAnimationFactory, mTextureFactory, mCamera);
        mPlayer->SetZIndex(ZINDEX_PLAYER);

        mPointsSystem = std::make_unique<PointsSystem>(mEventBus.get(), mGUI.get());

        mEnemy = std::make_shared<Enemy>(mEventBus.get(), mTextureFactory);
        mEnemy->SetZIndex(ZINDEX_ENEMY);
        mEnemy->SetPosition({2.0f, 0.0f});
    }

    SpaceShooterEngine::~SpaceShooterEngine()
    {
    }

    bool SpaceShooterEngine::OnUpdate(const float &dt)
    {
        mPlayer->Update(dt, mActionManager.get());

        auto playerBulletPool = mPlayer->GetBulletPool();
        auto enemyShape = mEnemy->GetCollider();
        std::vector<int> collisions = playerBulletPool->CheckAgainst(enemyShape);
        for (size_t i = 0; i < collisions.size(); ++i)
        {
            playerBulletPool->ReleaseBullet(collisions[i]);
            mEnemy->ReceiveDamage(1);
        }

        AddRenderCommand(mPlayer->GetRenderData());
        AddRenderCommands(mPlayer->GetBulletPool()->GetRenderData());
        AddRenderCommand(mEnemy->GetRenderData());

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

    void SpaceShooterEngine::OnRender() const
    {
        mPointsSystem->Draw();
    }
}