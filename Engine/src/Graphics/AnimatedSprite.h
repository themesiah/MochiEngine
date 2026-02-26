#ifndef HDEF_ANIMATEDSPRITE
#define HDEF_ANIMATEDSPRITE

#include "Spritesheet.h"
#include "AnimationData.h"
#include "../Types/Types.hpp"

#include <string>
#include <memory>
#include <vector>

namespace Mochi::Graphics
{
    class AbstractTextureFactory;
    class IAnimationFactory;
    /**
     * @brief AnimatedSprite does all the Spritesheet does and also provides an easy interface to animate and select animations for the sprite.
     *
     * AnimatedSprite has the same data as Spritesheet, but also uses the AnimationsData to internally change the frame shown in the spritesheet.
     *
     *
     * Ownership:
     *
     * - Same as Spritesheet, having a shared ownership of AnimationsData and ITexture.
     *
     *
     * Guarantees:
     *
     * - The sprite will change frames on the time defined by AnimationsData, given Update is called with a correct delta time.
     *
     * - Animations will change the next frame after calling PlayAnimation.
     *
     * - Animations will stop playing when calling StopAnimation.
     *
     *
     * Limitations:
     *
     * - For now, AnimatedSprite only works as part of a Spritesheet. So animations composed of several sprites are not currently supported.
     */
    class AnimatedSprite : public Spritesheet
    {
    protected:
        bool mPlaying;
        float mTimer;
        int mLoops;
        bool mForward;
        std::string mCurrentAnimation;

    public:
        /// @brief Constructs the spritesheet with the default SpriteBase constructor, internally calling LoadTexture.
        /// @param animationFactory The animation factory.
        /// @param textureFactory The texture factory.
        /// @param animationPath The path or unique identifier to the animation file in the file system.
        /// @param mainAnimation The first animation the sprite will show.
        AnimatedSprite(IAnimationFactory *animationFactory, AbstractTextureFactory *textureFactory, const std::string &animationPath, const std::string &mainAnimation);
        virtual ~AnimatedSprite();
        /// @brief This method updates an internal timer and changes frames when needed, according to the rules defined in the AnimationsData metadata. This needs
        /// to be called manually to work.
        /// @param dt The delta time.
        virtual void Update(const float &dt) override;
        /// @brief Given an existing animation name, it starts that animation. This means immediately changing the frame, resetting the timer and setting the current animation.
        /// @param animationName The unique name of the desired animation to play.
        virtual void PlayAnimation(const std::string &animationName);
        /// @brief Stops the current animation and the timer, freezing the sprite in the current frame.
        void StopAnimation();
        std::string GetCurrentAnimation() const;
    };
}

#endif