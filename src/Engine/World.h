#ifndef HDEF_WORLD
#define HDEF_WORLD

class Sprite;
struct SDL_Window;
struct SDL_Renderer;
struct FMOD_SYSTEM;
class World
{
private:
    int _timesUpdated;
    Sprite *mSampleSprite;
    FMOD_SYSTEM *mFmodSystem;

public:
    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};
    World();
    void Update(const float &dt);
    void Render() const;
    virtual ~World();
};

#endif