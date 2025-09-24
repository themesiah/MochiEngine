#ifndef HDEF_SPRITESHEET
#define HDEF_SPRITESHEET

#include <memory>
#include <string>
#include <vector>

#include "../Types/Types.hpp"
#include "RenderCommand.h"

struct SDL_Texture;
namespace Mochi::Graphics
{
    struct AnimationsData;
    class TextureFactory;
    class AnimationFactory;
    class Spritesheet
    {
    private:
        std::shared_ptr<SDL_Texture> mTexture;
        Rectf mDestRect;
        Rectf mSrcRect;
        float mScale;
        void ApplyFrameData();
        int mFrameIndex;

    protected:
        std::shared_ptr<AnimationsData> mAnimationsData;

    public:
        Spritesheet(std::shared_ptr<AnimationFactory> animationFactory, std::shared_ptr<TextureFactory> textureFactory, const std::string &animationPath, const int &startingFrame);
        ~Spritesheet();
        std::vector<RenderCommand> GetRenderData() const;
        void SetFrame(const int &frameIndex);
        int GetFrame() const;
        void SetScale(const float &scale);
        float GetScale() const;
        void SetPosition(const Vector2f &position);
        Vector2f GetPosition() const;
    };
}

#endif