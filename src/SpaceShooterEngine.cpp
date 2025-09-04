#include "SpaceShooterEngine.h"

#include <iostream>

#include "Graphics/Camera.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/Sprite.h"

#include "Input/ActionManager.h"

SpaceShooterEngine::SpaceShooterEngine() : Engine()
{
    mSampleSprite = std::make_shared<Sprite>(mTextureFactory, "Sprites/Background2.png");

    mAnimatedSprite = std::make_shared<AnimatedSprite>(mAnimationFactory, mTextureFactory, "Sprites/Snake.json", "Idle");

    mSpriteHandler = AddEntity(mSampleSprite);
    mAnimatedSpriteHandler = AddEntity(mAnimatedSprite);
}

SpaceShooterEngine::~SpaceShooterEngine()
{
}

bool SpaceShooterEngine::OnUpdate(const float &dt)
{
    mCamera->Move(mActionManager->Value("Horizontal") * dt * 1,
                  mActionManager->Value("Vertical") * dt * 1);

    if (mActionManager->Performed("Debug1"))
    {
        std::cout << "B is pressed!" << std::endl;
        // mFmod->PlayBGM("TestMusic");
        // mAnimatedSprite->PlayAnimation("Walk");
        RemoveEntity(mAnimatedSpriteHandler);
    }

    if (mActionManager->Performed("Debug2"))
    {
        std::cout << "C is pressed!" << std::endl;
        // mFmod->PauseBGM();
        mAnimatedSprite->PlayAnimation("Run");
    }

    if (mActionManager->Performed("Debug3"))
    {
        std::cout << "D is pressed!" << std::endl;
        // mFmod->ResumeBGM();
        mAnimatedSprite->PlayAnimation("Attack");
    }

    return true;
}