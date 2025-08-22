#include "World.h"

#include <SDL3/SDL.h>
#include <iostream>

World::World()
{
    _timesUpdated = 0;
}

void World::Update(const float &dt)
{
    const bool *keyboardState = SDL_GetKeyboardState(NULL);

    if (keyboardState[SDL_SCANCODE_A] == true)
    {
        std::cout << "A is pressed!";
    }

    _timesUpdated++;
    std::cout << "Times updated is: " << _timesUpdated << std::endl;
}

void World::Render() const
{

    SDL_RenderPresent(renderer); /* put it all on the screen! */
}