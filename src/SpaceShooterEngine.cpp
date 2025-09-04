#include "SpaceShooterEngine.h"

#include <iostream>

#include "Graphics/Camera.h"
#include "Graphics/Sprite.h"

#include "Input/ActionManager.h"

#include "Snake.h"

SpaceShooterEngine::SpaceShooterEngine() : Engine()
{
    mSpriteHandler = AddEntity(std::make_shared<Sprite>(mTextureFactory, "Sprites/Background2.png"));
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