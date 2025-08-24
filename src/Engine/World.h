#ifndef HDEF_WORLD
#define HDEF_WORLD

#include <SDL3/SDL_events.h>

class Sprite;
struct SDL_Window;
struct SDL_Renderer;
struct FMOD_STUDIO_SYSTEM;
struct FMOD_STUDIO_BANK;
class World
{
private:
    int _timesUpdated;
    Sprite *mSampleSprite;
    FMOD_STUDIO_SYSTEM *mFmodSystem;
    FMOD_STUDIO_BANK *mMainBank;
    FMOD_STUDIO_BANK *mStringsBank;
    bool bWasDown;

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