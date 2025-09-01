#ifndef HDEF_WORLD
#define HDEF_WORLD

#include <memory>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL.h>

class Sprite;
class AnimatedSprite;
class FMODWrapper;
class ActionManager;
class PackCatalog;
class TextureFactory;
class AnimationFactory;
class Renderer;
struct TTF_Font;
struct TTF_TextEngine;
struct TTF_Text;
class World
{
private:
    std::shared_ptr<Renderer> mRenderer;
    std::shared_ptr<FMODWrapper> mFmod;
    std::shared_ptr<ActionManager> mActionManager;
    std::shared_ptr<PackCatalog> mCatalog;
    std::shared_ptr<TextureFactory> mTextureFactory;
    std::shared_ptr<AnimationFactory> mAnimationFactory;
    Sprite *mSampleSprite;
    AnimatedSprite *mAnimatedSprite;
    TTF_Font *mFont;
    TTF_TextEngine *mTextEngine;
    TTF_Text *mText;

public:
    World();
    SDL_AppResult Update(const float &dt);
    SDL_AppResult AppEvent(SDL_Event *event);
    void Render() const;
    virtual ~World();
};

#endif