#include "Player.h"

#include "Input/ActionManager.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/AnimationFactory.h"
#include "Graphics/TextureFactory.h"
#include "Graphics/Camera.h"
#include "Graphics/Sprite.h"

#include "Bullets/PlayerBulletPool.h"

#include "Utils/MathUtils.h"

#include "Utils/Logger.h"

namespace Mochi::Shooter
{
    inline const std::string PLAYER_ANIM_PATH = "Player/PlayerShip.json";
    inline const std::string BULLET_PATH = "Player/Bullet.png";
    inline const std::string PLAYER_IDLE_ANIM = "Idle";
    inline const std::string PLAYER_DOWN_ANIM = "Down";
    inline const std::string PLAYER_UP_ANIM = "Up";
    inline constexpr float MOVEMENT_SPEED = 3.0f;
    inline constexpr float TILT_SPEED = 10.0f;
    inline constexpr float SHOT_DELAY = 0.05f;
    inline constexpr int MAX_BULLETS = 100;
    inline constexpr float BULLETS_LIFETIME = 1.0f;
    inline constexpr float BULLETS_SPEED = 15.0f;

    Player::Player(
        std::shared_ptr<Graphics::AnimationFactory> animationFactory,
        std::shared_ptr<Graphics::TextureFactory> textureFactory,
        std::shared_ptr<Graphics::Camera> camera)
        : Graphics::Spritesheet(animationFactory, textureFactory, PLAYER_ANIM_PATH, 0),
          mSpeed(MOVEMENT_SPEED),
          mTilt(0.0f),
          mTiltSpeed(TILT_SPEED),
          mCamera(camera),
          mShotDelay(SHOT_DELAY),
          mShotTimer(0.0f)
    {
        auto logicalPresentation = mCamera->GetLogicalPresentation();
        mBounds = Rectf(10.0f, 10.0f, logicalPresentation.x - 20.0f, logicalPresentation.y - 20.0f);

        auto playerBulletRenderable = std::make_shared<Graphics::Sprite>(textureFactory, BULLET_PATH);
        mBulletPool = std::make_shared<PlayerBulletPool>(playerBulletRenderable, MAX_BULLETS, BULLETS_LIFETIME, BULLETS_SPEED);
    }

    Player::~Player()
    {
    }

    void Player::Update(const float &dt, std::shared_ptr<Input::ActionManager> actionManager)
    {

        //////////////////////
        ////// MOVEMENT //////
        //////////////////////
        float horizontal = actionManager->Value("Horizontal");
        float vertical = actionManager->Value("Vertical");
        Vector2f movement = {horizontal, vertical};
        movement *= (dt * mSpeed);
        auto lastPosition = GetPosition();
        auto newPosition = GetPosition() + movement;

        // Camera world to screen with vector2f
        auto screenNewPosition = mCamera->WorldToScreen(newPosition);

        screenNewPosition.x = Math::Clamp(screenNewPosition.x, mBounds.x, mBounds.x + mBounds.w);
        screenNewPosition.y = Math::Clamp(screenNewPosition.y, mBounds.y, mBounds.y + mBounds.h);
        newPosition = mCamera->ScreenToWorld(screenNewPosition);

        SetPosition(newPosition);

        auto delta = newPosition - lastPosition;
        float tiltDirection = 0.0f;
        if (delta.y > 0.0f)
            tiltDirection = 1.0f;
        else if (delta.y < 0.0f)
            tiltDirection = -1.0f;
        mTilt = Math::MoveTowards(mTilt, tiltDirection, dt, mTiltSpeed);

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

        //////////////////////
        //////// SHOT ////////
        //////////////////////
        mShotTimer += dt;
        if (actionManager->Performed("Shot") && mShotTimer >= mShotDelay)
        {
            auto pos = GetPosition() + Vector2f(0.5f, 0.0f);
            mBulletPool->AddBullet(pos + Vector2f(0.0f, 0.1f));
            mBulletPool->AddBullet(pos + Vector2f(0.0f, -0.1f));
            mShotTimer = 0.0f;
        }
        mBulletPool->Update(dt);
    }

    std::shared_ptr<PlayerBulletPool> Player::GetBulletPool() const
    {
        return mBulletPool;
    }
}