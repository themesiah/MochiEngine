#ifndef HDEF_GIZMOS
#define HDEF_GIZMOS

using namespace Mochi;

#include "SDL3/SDL.h"

#include "../Physics/Shapes.h"
#include "../Utils/Conversion.hpp"

inline void DrawPoint(SDL_Renderer *renderer, const Physics::Point *point, const SDL_Color &color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    auto pos = MetersToPixels(FlipY(point->Position)) + Vector2f(CONST_RENDER_LOGICAL_X, CONST_RENDER_LOGICAL_Y) / 2;
    SDL_RenderPoint(renderer, pos.x, pos.y);
}

inline void DrawLine(SDL_Renderer *renderer, const Physics::Line *line, const SDL_Color &color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    auto pos1 = MetersToPixels(FlipY(line->Position)) + Vector2f(CONST_RENDER_LOGICAL_X, CONST_RENDER_LOGICAL_Y) / 2;
    auto pos2 = MetersToPixels(FlipY(line->End)) + Vector2f(CONST_RENDER_LOGICAL_X, CONST_RENDER_LOGICAL_Y) / 2;
    SDL_RenderLine(renderer, pos1.x, pos1.y, pos2.x, pos2.y);
}

inline void DrawCircle(SDL_Renderer *renderer, const Physics::Circle *circle, const SDL_Color &color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    const int resolution = 32;
    float radius = MetersToPixels(circle->Radius);
    auto center = MetersToPixels(FlipY(circle->Position));

    std::vector<SDL_FPoint> points(resolution + 1);

    for (int i = 0; i <= resolution; i++)
    {
        float theta = (float)i / resolution * 2.0f * M_PI;
        points[i].x = center.x + radius * cosf(theta) + CONST_RENDER_LOGICAL_X / 2;
        points[i].y = center.y - radius * sinf(theta) + CONST_RENDER_LOGICAL_Y / 2;
    }

    SDL_RenderLines(renderer, points.data(), points.size());
}

inline void DrawRectangle(SDL_Renderer *renderer, const Physics::Rectangle *rectangle, const SDL_Color &color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_FRect rect;
    rect.w = MetersToPixels(rectangle->Extents.x * 2.0f);
    rect.h = MetersToPixels(rectangle->Extents.y * 2.0f);
    rect.x = MetersToPixels(rectangle->Position.x) + CONST_RENDER_LOGICAL_X / 2 - rect.w / 2.0f;
    rect.y = MetersToPixels(-rectangle->Position.y) + CONST_RENDER_LOGICAL_Y / 2 - rect.h / 2.0f;
    SDL_RenderRect(renderer, &rect);
}

inline void DrawShape(SDL_Renderer *renderer, const Physics::Shape &shape, const SDL_Color &color)
{
    if (auto p = dynamic_cast<const Physics::Point *>(&shape))
    {
        DrawPoint(renderer, p, color);
    }
    else if (auto l = dynamic_cast<const Physics::Line *>(&shape))
    {
        DrawLine(renderer, l, color);
    }
    else if (auto c = dynamic_cast<const Physics::Circle *>(&shape))
    {
        DrawCircle(renderer, c, color);
    }
    else if (auto r = dynamic_cast<const Physics::Rectangle *>(&shape))
    {
        DrawRectangle(renderer, r, color);
    }
}
#endif