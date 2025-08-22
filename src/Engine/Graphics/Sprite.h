#ifndef HDEF_SPRITE
#define HDEF_SPRITE

#include <memory>
#include <SDL3/SDL.h>

class Sprite
{
private:
    std::shared_ptr<SDL_Texture> mTexture;
    SDL_FRect mDestRect;
    SDL_FRect mSrcRect;
    float mScale;

public:
    Sprite(SDL_Renderer *renderer, const char *file);
    void Render(SDL_Renderer *renderer) const;
    virtual ~Sprite();
};

#endif