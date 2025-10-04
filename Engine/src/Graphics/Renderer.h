#ifndef HDEF_RENDERER
#define HDEF_RENDERER

#include <memory>
#include <vector>

#include "RenderCommand.h"

struct SDL_Renderer;
struct SDL_Window;

namespace Mochi::Graphics
{
    class Camera;
    class Renderer
    {
    private:
        std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> mRenderer;
        std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> mWindow;

    public:
        Renderer(const char *appName, const char *appVersion, const char *appId, const char *windowName);
        ~Renderer();
        SDL_Renderer *GetRenderer() const { return mRenderer.get(); }
        SDL_Window *GetWindow() const { return mWindow.get(); }
        void StartFrameRendering() const;
        void Render(std::vector<RenderCommand> renderQueue, Camera *camera) const;
        void FinishRendering() const;
        std::shared_ptr<Camera> CreateCamera() const;
    };
}

#endif