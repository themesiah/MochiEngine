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
    class Spritesheet : public SpriteBase
    {
    private:
        void ApplyFrameData();
        int mFrameIndex;

    protected:
        std::shared_ptr<AnimationsData> mAnimationsData;

    public:
        Spritesheet(IAnimationFactory *animationFactory, AbstractTextureFactory *textureFactory, const std::string &animationPath, const int &startingFrame);
        virtual ~Spritesheet();
        void SetFrame(const int &frameIndex);
        int GetFrame() const;
    };
}

#endif