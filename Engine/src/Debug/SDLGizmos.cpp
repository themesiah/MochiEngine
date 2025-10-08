#include "SDLGizmos.h"

#include "../Graphics/SDL/SDLRenderer.h"
#include "SDL3/SDL.h"

#include "../Physics/Shapes.h"
#include "../Utils/Conversion.hpp"
#include "../Utils/MathUtils.h"

namespace Mochi::Debug
{
    SDLGizmos::SDLGizmos(Graphics::SDLRenderer *renderer) : mRenderer(renderer)
    {
    }

    void SDLGizmos::DrawRectangle(const Physics::Rectangle *rectangle, const SDL_Color &color) const
    {
        SDL_BlendMode lastBlendMode;
        SDL_GetRenderDrawBlendMode(mRenderer->GetRenderer(), &lastBlendMode);
        SDL_SetRenderDrawBlendMode(mRenderer->GetRenderer(), SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(mRenderer->GetRenderer(), color.r, color.g, color.b, color.a);
        SDL_FRect rect;
        rect.w = MetersToPixels(rectangle->Extents.x * 2.0f);
        rect.h = MetersToPixels(rectangle->Extents.y * 2.0f);
        rect.x = MetersToPixels(rectangle->Position.x) + CONST_RENDER_LOGICAL_X / 2 - rect.w / 2.0f;
        rect.y = MetersToPixels(-rectangle->Position.y) + CONST_RENDER_LOGICAL_Y / 2 - rect.h / 2.0f;
        SDL_RenderRect(mRenderer->GetRenderer(), &rect);
        SDL_SetRenderDrawBlendMode(mRenderer->GetRenderer(), lastBlendMode);
    }

    void SDLGizmos::DrawCircle(const Physics::Circle *circle, const SDL_Color &color) const
    {
        SDL_BlendMode lastBlendMode;
        SDL_GetRenderDrawBlendMode(mRenderer->GetRenderer(), &lastBlendMode);
        SDL_SetRenderDrawBlendMode(mRenderer->GetRenderer(), SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(mRenderer->GetRenderer(), color.r, color.g, color.b, color.a);
        const int resolution = 32;
        float radius = MetersToPixels(circle->Radius);
        auto center = MetersToPixels(FlipY(circle->Position));

        std::vector<SDL_FPoint> points(resolution + 1);

        for (int i = 0; i <= resolution; i++)
        {
            float theta = (float)i / resolution * 2.0f * F_PI;
            points[i].x = center.x + radius * cosf(theta) + CONST_RENDER_LOGICAL_X / 2;
            points[i].y = center.y - radius * sinf(theta) + CONST_RENDER_LOGICAL_Y / 2;
        }

        SDL_RenderLines(mRenderer->GetRenderer(), points.data(), (int)points.size());
        SDL_SetRenderDrawBlendMode(mRenderer->GetRenderer(), lastBlendMode);
    }

    void SDLGizmos::DrawLine(const Physics::Line *line, const SDL_Color &color) const
    {
        SDL_BlendMode lastBlendMode;
        SDL_GetRenderDrawBlendMode(mRenderer->GetRenderer(), &lastBlendMode);
        SDL_SetRenderDrawBlendMode(mRenderer->GetRenderer(), SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(mRenderer->GetRenderer(), color.r, color.g, color.b, color.a);
        auto pos1 = MetersToPixels(FlipY(line->Position)) + Vector2f(CONST_RENDER_LOGICAL_X, CONST_RENDER_LOGICAL_Y) / 2;
        auto pos2 = MetersToPixels(FlipY(line->End)) + Vector2f(CONST_RENDER_LOGICAL_X, CONST_RENDER_LOGICAL_Y) / 2;
        SDL_RenderLine(mRenderer->GetRenderer(), pos1.x, pos1.y, pos2.x, pos2.y);
        SDL_SetRenderDrawBlendMode(mRenderer->GetRenderer(), lastBlendMode);
    }

    void SDLGizmos::DrawPoint(const Physics::Point *point, const SDL_Color &color) const
    {
        SDL_BlendMode lastBlendMode;
        SDL_GetRenderDrawBlendMode(mRenderer->GetRenderer(), &lastBlendMode);
        SDL_SetRenderDrawBlendMode(mRenderer->GetRenderer(), SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(mRenderer->GetRenderer(), color.r, color.g, color.b, color.a);
        auto pos = MetersToPixels(FlipY(point->Position)) + Vector2f(CONST_RENDER_LOGICAL_X, CONST_RENDER_LOGICAL_Y) / 2;
        SDL_RenderPoint(mRenderer->GetRenderer(), pos.x, pos.y);
        SDL_SetRenderDrawBlendMode(mRenderer->GetRenderer(), lastBlendMode);
    }

}