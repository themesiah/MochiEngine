#include "SpaceShooterEngine.h"

#include <iostream>

#include "Graphics/Camera.h"
#include "Graphics/Sprite.h"

#include "Input/ActionManager.h"
#include "Audio/FMODWrapper.h"
#include "ScriptingManager.h"
#include "Packer/PackCatalog.h"

#include "Snake.h"
#include "Player.h"

SpaceShooterEngine::SpaceShooterEngine(const char *appName, const char *appVersion, const char *appId, const char *windowName)
    : Engine(appName, appVersion, appId, windowName)
{
    mCatalog->OpenPack("Data/Game");

    mScripting->ExecuteFile("Script/FMODCallbackDefinitionAlternative.lua");

    mPlayer = std::make_shared<Player>(mAnimationFactory, mTextureFactory, mCamera);
    mPlayerHandler = AddEntity(mPlayer);
}

SpaceShooterEngine::~SpaceShooterEngine()
{
}

bool SpaceShooterEngine::OnUpdate(const float &dt)
{
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