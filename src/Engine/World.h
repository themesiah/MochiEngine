#ifndef HREF_WORLD
#define HDEF_WORLD

#include <SDL3/SDL.h>

class World
{
private:
    int _timesUpdated;

public:
    SDL_Window *window{nullptr};
    SDL_Renderer *renderer{nullptr};
    World();
    void Update(const float &dt);
    void Render() const;
};

#endif