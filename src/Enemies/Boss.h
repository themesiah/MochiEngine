#ifndef HDEF_BOSS
#define HDEF_BOSS

#include "AbstractEnemy.h"

#include "Graphics/AbstractTextureFactory.h"
#include "Graphics/IAnimationFactory.h"

namespace Mochi::Shooter
{
    enum BossState
    {
        BossState_Vertical,
        BossState_Horizontal,
        BossState_Background
    };

    class Boss : public AbstractEnemy
    {
    private:
        BossState mState;

    public:
        Boss(Event::EventBus *eventBus, Graphics::AbstractTextureFactory *textureFactory, Graphics::IAnimationFactory *animationFactory);
        virtual ~Boss();
        virtual void Update(const float &dt) override;
        void SetState(const BossState &state);
    };
}

#endif