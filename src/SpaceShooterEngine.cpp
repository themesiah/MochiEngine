#include "SpaceShooterEngine.h"

#include <iostream>

#include "Graphics/Camera.h"
#include "Graphics/Sprite.h"

#include "Input/ActionManager.h"

#include "Snake.h"

SpaceShooterEngine::SpaceShooterEngine(const char *appName, const char *appVersion, const char *appId, const char *windowName)
    : Engine(appName, appVersion, appId, windowName)
{
    mSpriteHandler = AddEntity(std::make_shared<Mochi::Graphics::Sprite>(mTextureFactory, "Sprites/Background2.png"));
    mAnimatedSpriteHandler = AddEntity(std::make_shared<Snake>(mAnimationFactory, mTextureFactory, "Sprites/Snake.json", "Idle"));
}

SpaceShooterEngine::~SpaceShooterEngine()
{
}

bool SpaceShooterEngine::OnUpdate(const float &dt)
{
    mCamera->Move(mActionManager->Value("Horizontal") * dt * 1,
                  mActionManager->Value("Vertical") * dt * 1);

    return true;
}