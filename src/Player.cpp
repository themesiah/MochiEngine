#include "Player.h"

#include "Input/ActionManager.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/AnimationFactory.h"
#include "Graphics/TextureFactory.h"

#include "Utils/Logger.h"

inline const std::string PLAYER_ANIM_PATH = "Player/PlayerShip.json";
inline const std::string PLAYER_STARTING_ANIM = "Idle";

Player::Player(std::shared_ptr<Mochi::Graphics::AnimationFactory> animationFactory, std::shared_ptr<Mochi::Graphics::TextureFactory> textureFactory) : mPosition(Mochi::Vector2f::Zero),
                                                                                                                                                       mSpeed(3.0f)
{
    mAnim = std::make_unique<Mochi::Graphics::AnimatedSprite>(animationFactory, textureFactory, PLAYER_ANIM_PATH, PLAYER_STARTING_ANIM);
}

Player::~Player()
{
}

void Player::UpdateAnimation(const float &dt)
{
    mAnim->UpdateAnimation(dt);
}

std::vector<Mochi::Graphics::RenderCommand> Player::GetRenderData() const
{
    auto commands = mAnim->GetRenderData();
    for (auto &command : commands)
    {
        command.destRect.SetPosition(mPosition);
    }
    return commands;
}

void Player::Update(const float &dt, std::shared_ptr<Mochi::Input::ActionManager> actionManager)
{
    float horizontal = actionManager->Value("Horizontal");
    float vertical = actionManager->Value("Vertical");
    Mochi::Vector2f movement = {horizontal, vertical};
    movement *= (dt * mSpeed);
    mPosition += movement;
}
