#ifndef HDEF_SPRITE
#define HDEF_SPRITE

#include "IRenderable.h"

#include <memory>
#include <vector>
#include <string>
#include <SDL3/SDL.h>

class TextureFactory;
class Sprite : public IRenderable
{
private:
    std::shared_ptr<SDL_Texture> mTexture;
    SDL_FRect mDestRect;
    SDL_FRect mSrcRect;
    SDL_Renderer *mRenderer;
    float mScale;

public:
    Sprite(std::shared_ptr<TextureFactory>, SDL_Renderer *, const std::string &filename);
    virtual ~Sprite();
    virtual void Render(SDL_Renderer *renderer) const;
};

#endif