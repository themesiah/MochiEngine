#include "Player.h"

#include "Input/ActionManager.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/AnimationFactory.h"
#include "Graphics/TextureFactory.h"
#include "Graphics/Camera.h"

#include "Utils/MathUtils.h"

#include "Utils/Logger.h"

inline const std::string PLAYER_ANIM_PATH = "Player/PlayerShip.json";
inline const std::string PLAYER_IDLE_ANIM = "Idle";
inline const std::string PLAYER_DOWN_ANIM = "Down";
inline const std::string PLAYER_UP_ANIM = "Up";

Player::Player(
    std::shared_ptr<Mochi::Graphics::AnimationFactory> animationFactory,
    std::shared_ptr<Mochi::Graphics::TextureFactory> textureFactory,
    std::shared_ptr<Mochi::Graphics::Camera> camera)
    : Mochi::Graphics::Spritesheet(animationFactory, textureFactory, PLAYER_ANIM_PATH, 0),
      mSpeed(3.0f),
      mTilt(0.0f),
      mTiltSpeed(10.0f),
      mCamera(camera)
{
    auto logicalPresentation = mCamera->GetLogicalPresentation();
    mBounds = Mochi::Rectf(10.0f, 10.0f, logicalPresentation.x - 20.0f, logicalPresentation.y - 20.0f);
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
    auto lastPosition = GetPosition();
    auto newPosition = GetPosition() + movement;

    // Camera world to screen with vector2f
    auto screenNewPosition = mCamera->WorldToScreen(newPosition);

    screenNewPosition.x = Mochi::Math::Clamp(screenNewPosition.x, mBounds.x, mBounds.x + mBounds.w);
    screenNewPosition.y = Mochi::Math::Clamp(screenNewPosition.y, mBounds.y, mBounds.y + mBounds.h);
    newPosition = mCamera->ScreenToWorld(screenNewPosition);

    SetPosition(newPosition);

    auto delta = newPosition - lastPosition;
    float tiltDirection = 0.0f;
    if (delta.y > 0.0f)
        tiltDirection = 1.0f;
    else if (delta.y < 0.0f)
        tiltDirection = -1.0f;
    mTilt = Mochi::Math::MoveTowards(mTilt, tiltDirection, dt, mTiltSpeed);

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
}
