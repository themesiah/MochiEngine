#ifndef HDEF_ANIMATEDSPRITE
#define HDEF_ANIMATEDSPRITE

#include "IAnimatable.h"
#include "IRenderable.h"
#include "AnimationData.h"

#include <string>
#include <memory>
#include <SDL3/SDL.h>

class PackCatalog;
class AnimatedSprite : public IRenderable, public IAnimatable
{
private:
    AnimationsData mAnimationsData;
    float mTimer;
    int mCurrentFrame;
    std::string mCurrentAnimation;

    std::shared_ptr<SDL_Texture> mTexture;
    SDL_FRect mDestRect;
    SDL_FRect mSrcRect;
    SDL_FPoint mSize;
    float mScale;

public:
    AnimatedSprite(std::shared_ptr<PackCatalog>, SDL_Renderer *, const std::string &animationPath, const std::string &mainAnimation);
    virtual ~AnimatedSprite();
    virtual void Render(SDL_Renderer *renderer) const;
    virtual void UpdateAnimation(const float &dt);
    void PlayAnimation(const std::string &animationName);
};

#endif