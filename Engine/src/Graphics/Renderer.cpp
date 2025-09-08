#include "Renderer.h"

#include <SDL3/SDL.h>
#include <algorithm>
#include <memory>

#include "Camera.h"
#include "../Types/Types.hpp"
#include "../Exception.hpp"

namespace Mochi::Graphics
{

    Renderer::Renderer(const char *appName, const char *appVersion, const char *appId, const char *windowName)
    {
        SDL_Renderer *renderer;
        SDL_Window *window;
        SDL_SetAppMetadata(appName, appVersion, appId);

        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            throw SystemInitializationError("Graphics", SDL_GetError());
        }

        if (!SDL_CreateWindowAndRenderer(windowName, 640, 360, 0, &window, &renderer))
        {
            throw SystemInitializationError("Graphics", SDL_GetError());
        }

        mRenderer = std::shared_ptr<SDL_Renderer>(renderer, SDL_DestroyRenderer);
        mWindow = std::shared_ptr<SDL_Window>(window, SDL_DestroyWindow);

        if (!SDL_SetRenderLogicalPresentation(mRenderer.get(), 320, 180, SDL_RendererLogicalPresentation::SDL_LOGICAL_PRESENTATION_INTEGER_SCALE))
        {
            throw SystemInitializationError("Graphics", SDL_GetError());
        }

        if (!SDL_SetRenderVSync(mRenderer.get(), 1))
        {
            throw SystemInitializationError("Graphics", SDL_GetError());
        }
    }

    Renderer::~Renderer()
    {
    }

    void Renderer::StartFrameRendering() const
    {
        SDL_SetRenderDrawColor(mRenderer.get(), 154, 192, 193, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(mRenderer.get());
    }

    void Renderer::Render(std::vector<RenderCommand> renderQueue, std::shared_ptr<Camera> camera) const
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
            SDL_RenderTexture(mRenderer.get(), command.texture.get(), &src, &dstRect);
        }
    }

    void Renderer::FinishRendering() const
    {
        SDL_RenderPresent(mRenderer.get());
    }

    std::shared_ptr<Camera> Renderer::CreateCamera() const
    {
        int w, h;
        SDL_RendererLogicalPresentation *rlp = NULL;
        SDL_GetRenderLogicalPresentation(mRenderer.get(), &w, &h, rlp);
        return std::make_shared<Camera>((Vector2f){0, 0}, 1, (SDL_Point){w, h});
    }

}
