#include "DebugLayer.h"
#include "../Engine.h"

#include <format>
#include "SDL3/SDL.h"

#include "../Utils/Logger.h"

#include "../Constants.h"

#include "../Graphics/Renderer.h"
#include "../GUI/GUI.h"

namespace Mochi
{
    DebugLayer::DebugLayer(Graphics::Renderer *renderer, Graphics::GUI *gui)
        : Layer(), mRenderer(renderer), mGUI(gui)
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
        SDL_RendererLogicalPresentation *rlp{NULL};
        int logicalW, logicalH;
        SDL_GetRenderLogicalPresentation(mRenderer->GetRenderer(), &logicalW, &logicalH, rlp);

        SDL_SetRenderScale(mRenderer->GetRenderer(), 1, 1);
        mGUI->Text(CONST_DEVBUILD_TEXT, 8, {0, (float)logicalH - 8}, {255, 255, 255, SDL_ALPHA_OPAQUE});
        mGUI->Text(std::format("{} fps", (int)(1.0f / Engine::Get().GetLastRealDelta())).c_str(), 8, {0, 0}, {255, 255, 255, SDL_ALPHA_OPAQUE});
    }
}