#ifndef HDEF_SPRITE
#define HDEF_SPRITE

#include "IRenderable.h"

#include <memory>
#include <vector>
#include <SDL3/SDL.h>

class Sprite : public IRenderable
{
private:
    std::shared_ptr<SDL_Texture> mTexture;
    SDL_FRect mDestRect;
    SDL_FRect mSrcRect;
    float mScale;

public:
    Sprite(SDL_Renderer *, std::vector<char>);
    virtual ~Sprite();
    virtual void Render(SDL_Renderer *renderer) const;
};

#endif