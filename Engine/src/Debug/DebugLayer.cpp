#include "DebugLayer.h"
#include "../Engine.h"

#include <format>
#include "SDL3/SDL.h"

#include "../Utils/Logger.h"

#include "../Constants.h"

#include "../Graphics/IRenderer.h"
#include "../Graphics/SDL/SDLRenderer.h"
#include "../GUI/AbstractGUI.h"

#include "../Physics/Shapes.h"
#include "Gizmos.hpp"

namespace Mochi
{
    DebugLayer::DebugLayer()
        : Layer()
    {
    }

    DebugLayer::~DebugLayer()
    {
    }

    bool DebugLayer::Update(const float &dt)
    {
        return true;
    }

    void DebugLayer::Render() const
    {
    }

    void DebugLayer::GUI() const
    {
        // Dev build message
        Graphics::SDLRenderer *sdlrenderer = dynamic_cast<Graphics::SDLRenderer *>(mRenderer);
        if (!sdlrenderer)
            return;

        SDL_RendererLogicalPresentation *rlp{NULL};
        int logicalW, logicalH;
        SDL_GetRenderLogicalPresentation(sdlrenderer->GetRenderer(), &logicalW, &logicalH, rlp);

        SDL_SetRenderScale(sdlrenderer->GetRenderer(), 1, 1);
        mGUI->Text(CONST_DEVBUILD_TEXT, 8, {0, (float)logicalH - 8}, {255, 255, 255, SDL_ALPHA_OPAQUE});
        mGUI->Text(std::format("{} fps", (int)(1.0f / Engine::Get().GetLastRealDelta())).c_str(), 16, {0, 0}, {255, 255, 255, SDL_ALPHA_OPAQUE});
    }

    void DebugLayer::Debug() const
    {
        Graphics::SDLRenderer *sdlrenderer = dynamic_cast<Graphics::SDLRenderer *>(mRenderer);
        if (!sdlrenderer)
            return;
        auto renderer = sdlrenderer->GetRenderer();

        /*int w, h;
        SDL_RendererLogicalPresentation *pres = nullptr;
        SDL_GetRenderLogicalPresentation(renderer, &w, &h, pres);
        SDL_SetRenderDrawColor(renderer, 255, 0, 255, SDL_ALPHA_OPAQUE);
        SDL_RenderLine(renderer, 0.0f, h / 2, w, h / 2);
        SDL_RenderLine(renderer, w / 2, 0.0f, w / 2, h);

        Physics::Circle c({-5, -5}, 2);
        DrawCircle(renderer, &c, {255, 0, 255, 255});

        Physics::Point p({-5, -5});
        DrawPoint(renderer, &p, {255, 0, 255, 255});

        Physics::Line l({-1, 1}, {1, -1});
        DrawLine(renderer, &l, {255, 0, 255, 255});*/
    }
}