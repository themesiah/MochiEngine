#ifndef HDEF_PLAYER
#define HDEF_PLAYER

#include <memory>
#include <vector>

#include "Graphics/RenderCommand.h"
#include "Graphics/Spritesheet.h"
#include "Types/Types.hpp"
#include "Physics/Shapes.h"
#include "Event/EventBus.h"

namespace Mochi::Input
{
    class IActionManager;
}
namespace Mochi::Graphics
{
    class Camera;
    class AbstractTextureFactory;
    class IAnimationFactory;
    class AbstractGUI;
}
namespace Mochi::Shooter
{
    enum PlayerState
    {
        None,        // Starting state, so we can change states at the start
        Cutscene,    // Non controllable, just moving on the cutscene
        Playing,     // Playing as normal
        Damaged,     // Playing but non damageable (and blinking with alpa)
        Reespawning, // Non damageable, non controllable, will appear from the left
        Dead         // Waiting for UI confirmation to continue (and reespawn)
    };

    class PlayerBulletPool;
    class Player : public Mochi::Graphics::Spritesheet
    {
    private:
        float mSpeed;
        float mTilt;
        float mTiltSpeed;
        Mochi::Graphics::Camera *mCamera;
        Mochi::Rectf mBounds;
        std::shared_ptr<PlayerBulletPool> mBulletPool;
        float mShotDelay;
        float mShotTimer;
        Input::IActionManager *mActionManager;
        Event::EventBus *mEventBus;
        Graphics::AbstractGUI *mGUI;
        Physics::Rectangle mCollider;
        std::unique_ptr<Graphics::Spritesheet> mShield;

        int mMaxHealth;
        int mHealth;
        int mStartingLives;
        int mLives;
        float mDamageDelay;
        float mDamageTimer;
        float mReespawnTime;
        float mReespawnTimer;
        PlayerState mState;
        void Die();
        void OnStateEnter(const PlayerState &state);
        void OnStateExit(const PlayerState &state);

        void DamageBlink(const float &dt);
        void Movement(const float &dt, const Vector2f &movement);

    public:
        Player(Mochi::Graphics::IAnimationFactory *animationFactory,
               Mochi::Graphics::AbstractTextureFactory *textureFactory,
               Mochi::Graphics::Camera *camera,
               Input::IActionManager *actionManager,
               Event::EventBus *eventBus,
               Graphics::AbstractGUI *gui);
        virtual ~Player();
        virtual void Update(const float &dt) override;
        std::shared_ptr<PlayerBulletPool> GetBulletPool() const;
        virtual void SetScale(const float &scale) override;
        Physics::Rectangle GetCollider() const;
        void ReceiveDamage();
        virtual std::vector<Graphics::RenderCommand> GetRenderData() const override;
        void GUI();
        void ChangeState(const PlayerState &state);
        int GetCurrentHealth() const;
        void SetHealth(const int &newHealth);
        int GetCurrentLives() const;
        void SetLives(const int &lives);
        PlayerState GetCurrentState() const;
        void Reespawn();
    };
}

#endif