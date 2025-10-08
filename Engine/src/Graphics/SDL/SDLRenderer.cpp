#include "SDLRenderer.h"

#include <SDL3/SDL.h>
#include <algorithm>
#include <memory>

#include "../IRenderer.h"
#include "../Camera.h"
#include "../Types/Types.hpp"
#include "../Exception.hpp"
#include "../Constants.h"
#include "SDLTexture.h"
#include "SDLTextureFactory.h"
#include "../../Debug/SDLGizmos.h"
#include "../../GUI/SDLGUI.h"
#include "../../Input/IActionManager.h"

namespace Mochi::Graphics
{

    SDLRenderer::SDLRenderer(const char *appName, const char *appVersion, const char *appId, const char *windowName)
        : mRenderer({nullptr, SDL_DestroyRenderer}), mWindow({nullptr, SDL_DestroyWindow})
    {
        SDL_Renderer *renderer;
        SDL_Window *window;
        SDL_SetAppMetadata(appName, appVersion, appId);

        SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS, "1");

        if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
        {
            throw SystemInitializationError("Graphics", SDL_GetError());
        }

        if (!SDL_CreateWindowAndRenderer(windowName, 640, 360, 0, &window, &renderer))
        {
            throw SystemInitializationError("Graphics", SDL_GetError());
        }

        mRenderer.reset(renderer);
        mWindow.reset(window);

        if (!SDL_SetRenderLogicalPresentation(mRenderer.get(), CONST_RENDER_LOGICAL_X, CONST_RENDER_LOGICAL_Y, SDL_RendererLogicalPresentation::SDL_LOGICAL_PRESENTATION_INTEGER_SCALE))
        {
            throw SystemInitializationError("Graphics", SDL_GetError());
        }

        // if (!SDL_SetRenderVSync(mRenderer.get(), 1))
        // {
        //     throw SystemInitializationError("Graphics", SDL_GetError());
        // }
    }

    SDLRenderer::~SDLRenderer()
    {
    }

    void SDLRenderer::StartFrameRendering() const
    {
        SDL_SetRenderDrawColor(mRenderer.get(), 154, 192, 193, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(mRenderer.get());
    }

    void SDLRenderer::Render(std::vector<RenderCommand> renderQueue, Camera *camera) const
    {
        std::sort(renderQueue.begin(), renderQueue.end(),
                  [](RenderCommand &a, RenderCommand &b)
                  { return a.zindex < b.zindex; });

        SDL_RendererLogicalPresentation *rlp{NULL};
        int logicalW, logicalH;
        SDL_GetRenderLogicalPresentation(mRenderer.get(), &logicalW, &logicalH, rlp);

        float cameraZoom = camera->GetZoom();
        SDL_SetRenderScale(mRenderer.get(), cameraZoom, cameraZoom);
        for (auto &command : renderQueue)
        {
            SDL_FRect dstRect = camera->WorldToScreen(command.destRect);
            SDL_FRect src = command.sourceRect;
            SDLTexture *tex = dynamic_cast<SDLTexture *>(command.texture);
            SDL_RenderTexture(mRenderer.get(), tex->GetTexture(), &src, &dstRect);
        }
    }

    void SDLRenderer::FinishRendering() const
    {
        SDL_RenderPresent(mRenderer.get());
    }

    std::unique_ptr<AbstractTextureFactory> SDLRenderer::CreateTextureFactory(FS::PackCatalog *catalog) const
    {
        return std::unique_ptr<SDLTextureFactory>(new SDLTextureFactory(catalog, mRenderer.get()));
    }

    std::unique_ptr<AbstractGUI> SDLRenderer::CreateGUI(FS::PackCatalog *catalog, Input::IActionManager *actionManager)
    {
        return std::unique_ptr<SDLGUI>(new SDLGUI(catalog, this, actionManager));
    }

    std::unique_ptr<Debug::IGizmos> SDLRenderer::CreateGizmos()
    {
        return std::unique_ptr<Debug::SDLGizmos>(new Debug::SDLGizmos(this));
    }
}
