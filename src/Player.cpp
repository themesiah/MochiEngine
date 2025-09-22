#include "Player.h"

#include "Input/ActionManager.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/AnimationFactory.h"
#include "Graphics/TextureFactory.h"

#include "Utils/Logger.h"

inline const std::string PLAYER_ANIM_PATH = "Player/PlayerShip.json";
inline const std::string PLAYER_STARTING_ANIM = "Idle";
inline const std::string PLAYER_DOWN_ANIM = "Down";
inline const std::string PLAYER_UP_ANIM = "Up";

Player::Player(
    std::shared_ptr<Mochi::Graphics::AnimationFactory> animationFactory,
    std::shared_ptr<Mochi::Graphics::TextureFactory> textureFactory)
    : Mochi::Graphics::AnimatedSprite(animationFactory, textureFactory, PLAYER_ANIM_PATH, PLAYER_STARTING_ANIM),
      mSpeed(3.0f)
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

    if (vertical < 0.0f)
    {
        if (GetCurrentAnimation() != PLAYER_UP_ANIM)
        {
            PlayAnimation(PLAYER_UP_ANIM);
        }
    }
    else if (vertical > 0.0f)
    {
        if (GetCurrentAnimation() != PLAYER_DOWN_ANIM)
        {
            PlayAnimation(PLAYER_DOWN_ANIM);
        }
    }
    else
    {
        if (GetCurrentAnimation() != PLAYER_STARTING_ANIM)
        {
            PlayAnimation(PLAYER_STARTING_ANIM);
        }
    }
}
