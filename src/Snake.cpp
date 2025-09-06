#include "Snake.h"

#include <memory>
#include <string>

#include "Graphics/AnimatedSprite.h"
#include "Graphics/AnimationFactory.h"
#include "Graphics/TextureFactory.h"
#include "Graphics/Camera.h"
#include "Input/ActionManager.h"

#include "Entity/IUpdateable.h"

#include "Utils/Logger.h"

Snake::Snake(std::shared_ptr<Mochi::Graphics::AnimationFactory> mAnimationFactory, std::shared_ptr<Mochi::Graphics::TextureFactory> mTextureFactory, const std::string &animationPath, const std::string &mainAnimation)
    : AnimatedSprite(mAnimationFactory, mTextureFactory, animationPath, mainAnimation)
{
}

Snake::~Snake() {}
void Snake::Update(const float &dt, std::shared_ptr<Mochi::Input::ActionManager> mActionManager)
{
    if (mActionManager->Performed("Debug1"))
    {
        PlayAnimation("Walk");
    }

    if (mActionManager->Performed("Debug2"))
    {
        PlayAnimation("Run");
    }

    if (mActionManager->Performed("Debug3"))
    {
        PlayAnimation("Attack");
    }
}