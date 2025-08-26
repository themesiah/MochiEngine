#ifndef HDEF_WORLD
#define HDEF_WORLD

#include <memory>
#include <SDL3/SDL_events.h>

class Sprite;
class FMODWrapper;
class ActionManager;
struct SDL_Window;
struct SDL_Renderer;
class World
{
private:
    std::shared_ptr<FMODWrapper> mFmod;
    std::shared_ptr<ActionManager> mActionManager;
    Sprite *mSampleSprite;

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