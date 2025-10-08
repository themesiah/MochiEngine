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
#include "IGizmos.h"

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
        float w = static_cast<float>(CONST_RENDER_LOGICAL_X);
        float h = static_cast<float>(CONST_RENDER_LOGICAL_Y);

        Physics::Line l({0.0f, h / 2.0f}, {w, h / 2.0f});
        mGizmos->DrawLine(&l, {255, 0, 255, 128});
        Physics::Line l2({w / 22.0f, 0.0f}, {w / 22.0f, h});
        mGizmos->DrawLine(&l2, {255, 0, 255, 128});
    }
}