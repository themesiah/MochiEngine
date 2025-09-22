#include "Player.h"

#include "Input/ActionManager.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/AnimationFactory.h"
#include "Graphics/TextureFactory.h"

#include "Utils/MathUtils.h"

#include "Utils/Logger.h"

inline const std::string PLAYER_ANIM_PATH = "Player/PlayerShip.json";
inline const std::string PLAYER_IDLE_ANIM = "Idle";
inline const std::string PLAYER_DOWN_ANIM = "Down";
inline const std::string PLAYER_UP_ANIM = "Up";

Player::Player(
    std::shared_ptr<Mochi::Graphics::AnimationFactory> animationFactory,
    std::shared_ptr<Mochi::Graphics::TextureFactory> textureFactory)
    : Mochi::Graphics::Spritesheet(animationFactory, textureFactory, PLAYER_ANIM_PATH, 0),
      mSpeed(3.0f),
      mTilt(0.0f),
      mTiltSpeed(10.0f)
{
}

Player::~Player()
{
}

void Player::Update(const float &dt, std::shared_ptr<Mochi::Input::ActionManager> actionManager)
{
    float horizontal = actionManager->Value("Horizontal");
    float vertical = actionManager->Value("Vertical");
    Mochi::Vector2f movement = {horizontal, vertical};
    movement *= (dt * mSpeed);
    SetPosition(GetPosition() + movement);

    mTilt = Mochi::Math::MoveTowards(mTilt, vertical, dt, mTiltSpeed);

    if (mTilt == 0)
    {
        SetFrame(0);
    }
    else if (mTilt == 1.0f)
    {
        SetFrame(4);
    }
    else if (mTilt == -1.0f)
    {
        SetFrame(2);
    }
    else if (mTilt > 0.0f)
    {
        SetFrame(3);
    }
    else if (mTilt < 0.0f)
    {
        SetFrame(1);
    }

    // if (vertical < 0.0f)
    // {
    //     if (GetCurrentAnimation() != PLAYER_UP_ANIM)
    //     {
    //         PlayAnimation(PLAYER_UP_ANIM);
    //     }
    // }
    // else if (vertical > 0.0f)
    // {
    //     if (GetCurrentAnimation() != PLAYER_DOWN_ANIM)
    //     {
    //         PlayAnimation(PLAYER_DOWN_ANIM);
    //     }
    // }
    // else
    // {
    //     if (GetCurrentAnimation() != PLAYER_IDLE_ANIM)
    //     {
    //         PlayAnimation(PLAYER_IDLE_ANIM);
    //     }
    // }
}
