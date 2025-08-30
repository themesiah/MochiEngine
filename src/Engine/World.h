#ifndef HDEF_WORLD
#define HDEF_WORLD

#include <memory>
#include <SDL3/SDL_events.h>

class Sprite;
class FMODWrapper;
class ActionManager;
class PackCatalog;
struct SDL_Window;
struct SDL_Renderer;
struct TTF_Font;
struct TTF_TextEngine;
struct TTF_Text;
class World
{
private:
    std::shared_ptr<FMODWrapper> mFmod;
    std::shared_ptr<ActionManager> mActionManager;
    std::shared_ptr<PackCatalog> mCatalog;
    Sprite *mSampleSprite;
    TTF_Font *mFont;
    TTF_TextEngine *mTextEngine;
    TTF_Text *mText;

public:
    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};
    World();
    void Update(const float &dt);
    void AppEvent(SDL_Event *event);
    void Render() const;
    virtual ~World();
};

#endif