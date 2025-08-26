#ifndef HDEF_WORLD
#define HDEF_WORLD

#include <memory>
#include <SDL3/SDL_events.h>

class Sprite;
class FMODWrapper;
class InputManager;
struct SDL_Window;
struct SDL_Renderer;
class World
{
private:
    std::unique_ptr<FMODWrapper> mFmod;
    std::unique_ptr<InputManager> mInputManager;
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