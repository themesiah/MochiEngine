#ifndef HDEF_SPRITESHEET
#define HDEF_SPRITESHEET

#include <memory>
#include <string>
#include <vector>

#include "../Types/Types.hpp"
#include "RenderCommand.h"
#include "SpriteBase.h"

struct SDL_Texture;
namespace Mochi::Graphics
{
    struct AnimationsData;
    class AbstractTextureFactory;
    class IAnimationFactory;
    /**
     * @brief Spritesheet takes all the functionality of SpriteBase, generates animation data for the texture, and expects it to be used manually.
     *
     * This means that the Spritesheet generates a texture containing several sprites, and the user is responsible to change frames.
     *
     * This has several uses. If a sprite has animations that are not represented with traditional flows (forward, backward, pingpong) this allows to change
     * the frames manually. It also works to create sprite atlases and improve performance, using the same texture for a lot of different sprites.
     *
     * Ownership:
     *
     * - It has shared ownership of the animation data with the animation factory.
     *
     * Guarantees:
     *
     * - If animation file is valid, the sprite will at least have a frame and start on the selected frame, with a source rect according the data.
     *
     * Not responsible:
     *
     * - This class does not animate, but changing the frame index over time animates the sprite. Check AnimatedSprite.h for this purpose.
     */
    class Spritesheet : public SpriteBase
    {
    private:
        void ApplyFrameData();
        int mFrameIndex;

    protected:
        std::shared_ptr<AnimationsData> mAnimationsData;

    public:
        /// @brief Constructs the spritesheet with the default SpriteBase constructor, internally calling LoadTexture.
        /// @param animationFactory The animation factory.
        /// @param textureFactory The texture factory.
        /// @param animationPath The path or unique identifier to the animation file in the file system.
        /// @param startingFrame The first frame the sprite will show.
        Spritesheet(IAnimationFactory *animationFactory, AbstractTextureFactory *textureFactory, const std::string &animationPath, const int &startingFrame);
        virtual ~Spritesheet();
        /// @brief Sets the current frame for the animation
        /// @param frameIndex A 0 based index of which texture is going to be shown.
        void SetFrame(const int &frameIndex);
        /// @brief Gets the current frame index.
        /// @returns Current frame index
        int GetFrame() const;
    };
}

#endif