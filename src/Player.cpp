#include "Player.h"

#include "Engine.h"
#include "Input/IActionManager.h"
#include "Graphics/AnimatedSprite.h"
#include "Graphics/IAnimationFactory.h"
#include "Graphics/AbstractTextureFactory.h"
#include "Graphics/Camera.h"
#include "Graphics/SpriteBase.h"
#include "Event/EngineEvents.h"

#include "GUI/AbstractGUI.h"

#include "Bullets/PlayerBulletPool.h"
#include "ZIndexEnum.h"
#include "ShooterEvents.h"

#include "Utils/MathUtils.h"

#include "Utils/Logger.h"
#include "Utils/Conversion.hpp"
#include "GUI/GUICommon.hpp"

namespace Mochi::Shooter
{
    inline const std::string PLAYER_ANIM_PATH = "Player/PlayerShip.json";
    inline const std::string SHIELD_ANIM_PATH = "Player/PlayerShield.json";
    inline const std::string BULLET_PATH = "Player/Bullet.png";
    inline const std::string PLAYER_IDLE_ANIM = "Idle";
    inline const std::string PLAYER_DOWN_ANIM = "Down";
    inline const std::string PLAYER_UP_ANIM = "Up";
    inline constexpr float MOVEMENT_SPEED = 15.0f;
    inline constexpr float TILT_SPEED = 10.0f;
    inline constexpr float SHOT_DELAY = 0.05f;
    inline constexpr int MAX_BULLETS = 100;
    inline constexpr float BULLETS_LIFETIME = 1.0f;
    inline constexpr float BULLETS_SPEED = 48.0f;
    inline constexpr int MAX_HEALTH = 3;
    inline constexpr float DAMAGE_DELAY = 2.0f;
    inline constexpr float DAMAGED_BLINK_RATE = 0.4f;
    inline constexpr int MAX_LIVES = 1;
    inline constexpr float REESPAWN_TIME = 1.0f;

    Player::Player(
        Graphics::IAnimationFactory *animationFactory,
        Graphics::AbstractTextureFactory *textureFactory,
        Graphics::Camera *camera,
        Input::IActionManager *actionManager,
        Event::EventBus *eventBus,
        Graphics::AbstractGUI *gui)
        : Graphics::Spritesheet(animationFactory, textureFactory, PLAYER_ANIM_PATH, 0),
          mSpeed(MOVEMENT_SPEED),
          mTilt(0.0f),
          mTiltSpeed(TILT_SPEED),
          mCamera(camera),
          mShotDelay(SHOT_DELAY),
          mShotTimer(0.0f),
          mActionManager(actionManager),
          mEventBus(eventBus),
          mGUI(gui),
          mCollider(PixelsToMeters(Rectf({0.0f, 0.0f}, {32.0f, 32.0f}))),
          mMaxHealth(MAX_HEALTH),
          mHealth(mMaxHealth),
          mStartingLives(MAX_LIVES),
          mLives(mStartingLives),
          mDamageDelay(DAMAGE_DELAY),
          mDamageTimer(mDamageDelay),
          mDamagedState(false),
          mIsAlive(true),
          mAwaitingContinue(false),
          mIsControllable(true),
          mReespawnTime(REESPAWN_TIME),
          mReespawnTimer(0.0f)
    {
        auto logicalPresentation = mCamera->GetLogicalPresentation();
        mBounds = Rectf(10.0f, 10.0f, logicalPresentation.x - 20.0f, logicalPresentation.y - 20.0f);

        auto playerBulletRenderable = std::make_shared<Graphics::SpriteBase>(textureFactory, BULLET_PATH);
        mBulletPool = std::make_shared<PlayerBulletPool>(playerBulletRenderable, MAX_BULLETS, BULLETS_LIFETIME, BULLETS_SPEED);

        SetScale(2.0f);
        playerBulletRenderable->SetScale(GetScale());

        SetZIndex(ZINDEX_PLAYER);

        mShield = std::make_unique<Graphics::Spritesheet>(animationFactory, textureFactory, SHIELD_ANIM_PATH, 0);
        mShield->SetScale(GetScale());
        mShield->SetZIndex(GetZIndex() + 1);
        mShield->SetAlpha(100);
    }

    Player::~Player()
    {
    }

    void Player::SetScale(const float &scale)
    {
        Graphics::Spritesheet::SetScale(scale);
        mBulletPool->GetRenderable()->SetScale(scale);
    }

    void Player::Update(const float &dt)
    {
        Graphics::Spritesheet::Update(dt);

        //////////////////////////
        //// DAMAGE FEEDBACK /////
        //////////////////////////
        if (mDamagedState || !mIsControllable)
        {
            mDamageTimer += dt;
            float remainder = Math::Repeat(mDamageTimer, DAMAGED_BLINK_RATE);
            if (remainder < DAMAGED_BLINK_RATE / 2.0f)
            {
                SetAlpha(10);
            }
            else
            {
                SetAlpha(255);
            }
            if (mDamageTimer >= mDamageDelay)
            {
                mDamagedState = false;
                SetAlpha(255);
            }
        }

        float horizontal = 0.0f;
        float vertical = 0.0f;
        bool shot = false;
        if (mIsControllable)
        {
            horizontal = mActionManager->Value("Horizontal");
            vertical = mActionManager->Value("Vertical");
            shot = mActionManager->Performed("Shot");
        }
        else
        {
            mReespawnTimer += dt;
            SetPosition(Vector2f::MoveTowards(GetPosition(), {-10.0f, 0.0f}, mSpeed * dt));
            if (mReespawnTimer >= mReespawnTime)
            {
                mIsControllable = true;
                mDamagedState = true;
                mDamageTimer = 0.0f;
            }
        }
        //////////////////////
        ////// MOVEMENT //////
        //////////////////////
        Vector2f movement = {horizontal, vertical};
        movement *= (dt * mSpeed);
        auto lastPosition = GetPosition();
        auto newPosition = GetPosition() + movement;

        // Camera world to screen with vector2f
        auto screenNewPosition = mCamera->WorldToScreen(newPosition);

        if (mIsControllable)
        {
            screenNewPosition.x = Math::Clamp(screenNewPosition.x, mBounds.x, mBounds.x + mBounds.w);
            screenNewPosition.y = Math::Clamp(screenNewPosition.y, mBounds.y, mBounds.y + mBounds.h);
            newPosition = mCamera->ScreenToWorld(screenNewPosition);

            SetPosition(newPosition);
        }

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
        if (shot && mShotTimer >= mShotDelay)
        {
            auto pos = GetPosition() + Vector2f(1.0f, 0.0f);
            mBulletPool->AddBullet(pos + Vector2f(0.0f, 0.2f));
            mBulletPool->AddBullet(pos + Vector2f(0.0f, -0.2f));
            mShotTimer = 0.0f;
        }
        mBulletPool->Update(dt);

        mShield->SetPosition(GetPosition());
    }

    std::shared_ptr<PlayerBulletPool> Player::GetBulletPool() const
    {
        return mBulletPool;
    }

    Physics::Rectangle Player::GetCollider() const
    {
        auto collider = mCollider;
        collider.Position = GetPosition();
        return collider;
    }

    void Player::ReceiveDamage()
    {
        if (!mDamagedState && mIsControllable)
        {
            mHealth--;
            mDamageTimer = 0.0f;
            mEventBus->Publish<PlayerDamageReceivedEvent>({});

            LOG_INFO("Player received damage");

            if (mHealth == 1)
            {
                mShield->SetVisible(false);
            }
            if (mHealth <= 0)
            {
                Die();
                if (mLives > 0)
                {
                    mShield->SetVisible(true);
                }
            }
            if (!mAwaitingContinue)
            {
                mDamagedState = true;
                mShield->SetFrame(mMaxHealth - mHealth);
            }
        }
    }

    void Player::Die()
    {
        mIsAlive = false;
        mEventBus->Publish<PlayerDeadEvent>({this});
        mLives--;
        LOG_INFO("Player died");
        if (mLives == 0)
        {
            mAwaitingContinue = true;
            Engine::Get().Pause();
        }
        else
        {
            mHealth = mMaxHealth;
            mReespawnTimer = 0.0f;
            mIsControllable = false;
            SetPosition({-19.0f, 0.0f});
        }
    }

    std::vector<Graphics::RenderCommand> Player::GetRenderData() const
    {
        auto data = Graphics::Spritesheet::GetRenderData();
        auto shieldData = mShield->GetRenderData();
        data.insert(data.end(), shieldData.begin(), shieldData.end());
        return data;
    }

    void Player::GUI()
    {
        // Shields
        for (int i = 0; i < mMaxHealth - 1; ++i)
        {
            Graphics::GUIOptions options{
                .SrcRect = {Rectf({0.0f, 0.0f}, {16.0f, 16.0f})},
                .DstRect = {Rectf({32.0f * i, 0.0f}, {32.0f, 32.0f})},
                .ScreenAnchor = Graphics::GUI_TOP_LEFT,
                .SpritePivot = Graphics::GUI_TOP_LEFT};

            if (mHealth > i + 1)
            {
                options.SrcRect.value().SetPosition({16.0f, 0.0f});
            }
            else
            {
                options.SrcRect.value().SetPosition({0.0f, 0.0f});
            }
            mGUI->Sprite("UIElements.png", options);
        }

        // Lives
        for (int i = 0; i < mLives - 1; ++i)
        {
            Graphics::GUIOptions options{
                .SrcRect = {Rectf({0.0f, 16.0f}, {16.0f, 16.0f})},
                .DstRect = {Rectf({32.0f * i, 32.0f}, {32.0f, 32.0f})},
                .ScreenAnchor = Graphics::GUI_TOP_LEFT,
                .SpritePivot = Graphics::GUI_TOP_LEFT};
            mGUI->Sprite("UIElements.png", options);
        }

        if (mAwaitingContinue)
        {
            const Graphics::GUITextOptions titleTextOptions{
                .DstRect = {Rectf({0.0f, -50.0f}, {})},
                .ScreenAnchor = Graphics::GUI_MIDDLE_CENTER,
                .TextPivot = Graphics::GUI_MIDDLE_CENTER,
                .TextSize = 82.0f};

            const Graphics::GUIOptions button1Options{
                .SrcRect = {Rectf({0.0f, 0.0f}, {32.0f, 32.0f})},
                .DstRect = {Rectf({0.0f, 0.0f}, {200.0f, 50.0f})},
                .Slice = {Rectf(9.0f, 9.0f, 9.0f, 9.0f)},
                .ScreenAnchor = Graphics::GUI_MIDDLE_CENTER,
                .SpritePivot = Graphics::GUI_MIDDLE_CENTER};

            const Graphics::GUIOptions button2Options{
                .SrcRect = {Rectf({0.0f, 0.0f}, {32.0f, 32.0f})},
                .DstRect = {Rectf({0.0f, 80.0f}, {200.0f, 50.0f})},
                .Slice = {Rectf(9.0f, 9.0f, 9.0f, 9.0f)},
                .ScreenAnchor = Graphics::GUI_MIDDLE_CENTER,
                .SpritePivot = Graphics::GUI_MIDDLE_CENTER};

            const Graphics::GUITextOptions buttonTextOptions{
                .ScreenAnchor = Graphics::GUI_MIDDLE_CENTER,
                .TextPivot = Graphics::GUI_MIDDLE_CENTER,
                .TextSize = 32.0f};
            //  Show continue title
            mGUI->Text("CONTINUE?", titleTextOptions);
            // Show button YES. if yes, Reespawn
            if (mGUI->Button("Interface.png", button1Options, "CONTINUE", buttonTextOptions).Pressed)
            {
                Reespawn();
            }
            // Show button NO. If no, publish close game event
            if (mGUI->Button("Interface.png", button2Options, "EXIT", buttonTextOptions).Pressed)
            {
                mEventBus->Publish<ApplicationQuitEvent>({});
            }
        }
    }

    void Player::Reespawn()
    {
        mEventBus->Publish<PlayerContinueEvent>({});
        mLives = mStartingLives;
        mIsAlive = true;
        mAwaitingContinue = false;
        mHealth = mMaxHealth;
        mReespawnTimer = 0.0f;
        mIsControllable = false;
        mShield->SetFrame(0);
        SetPosition({-19.0f, 0.0f});
        Engine::Get().Resume();
    }
}