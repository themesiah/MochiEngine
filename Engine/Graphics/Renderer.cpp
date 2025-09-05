#include "Renderer.h"

#include <SDL3/SDL.h>
#include <algorithm>

#include "Camera.h"

namespace Mochi::Graphics
{

    Renderer::Renderer()
    {
    }

    Renderer::~Renderer()
    {
    }

    bool Renderer::Init(const char *appName, const char *appVersion, const char *appId, const char *windowName)
    {
        SDL_Renderer *renderer;
        SDL_Window *window;
        SDL_SetAppMetadata(appName, appVersion, appId);

        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
            return false;
        }

        if (!SDL_CreateWindowAndRenderer(windowName, 640, 360, 0, &window, &renderer))
        {
            SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
            return false;
        }

        mRenderer = std::shared_ptr<SDL_Renderer>(renderer, SDL_DestroyRenderer);
        mWindow = std::shared_ptr<SDL_Window>(window, SDL_DestroyWindow);

        if (!SDL_SetRenderLogicalPresentation(mRenderer.get(), 320, 180, SDL_RendererLogicalPresentation::SDL_LOGICAL_PRESENTATION_INTEGER_SCALE))
        {
            SDL_Log("Couldn't set render logical presentation: %s", SDL_GetError());
            return false;
        }

        if (!SDL_SetRenderVSync(mRenderer.get(), 1))
        {
            SDL_Log("Couldn't set vsyinc to true: %s", SDL_GetError());
            return false;
        }

        return true;
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
            auto dstRect = camera->WorldToScreen(command.destRect);
            dstRect.x += logicalW / 2; // Move local size to camera!
            dstRect.y += logicalH / 2;
            SDL_RenderTexture(mRenderer.get(), command.texture.get(), &command.sourceRect, &dstRect);
        }
    }

    void Renderer::FinishRendering() const
    {
        SDL_RenderPresent(mRenderer.get());
    }

}
