#ifndef HDEF_RENDERER
#define HDEF_RENDERER

#include "../IRenderer.h"

#include <memory>
#include <vector>

#include "../RenderCommand.h"

namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Debug
{
    class IGizmos;
}
struct SDL_Renderer;
struct SDL_Window;

namespace Mochi::Graphics
{
    class Camera;
    class AbstractTextureFactory;
    class SDLRenderer : public IRenderer
    {
    private:
        std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> mRenderer;
        std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> mWindow;

    public:
        SDLRenderer(const char *appName, const char *appVersion, const char *appId, const char *windowName);
        virtual ~SDLRenderer();
        SDL_Renderer *GetRenderer() const { return mRenderer.get(); }
        SDL_Window *GetWindow() const { return mWindow.get(); }
        virtual void StartFrameRendering() const override;
        virtual void Render(std::vector<RenderCommand> renderQueue, Camera *camera) const override;
        virtual void FinishRendering() const override;
        virtual std::unique_ptr<AbstractTextureFactory> CreateTextureFactory(FS::PackCatalog *catalog) const;
        virtual std::unique_ptr<AbstractGUI> CreateGUI(FS::PackCatalog *catalog, Input::IActionManager *actionManager);
#if DEBUG
        virtual std::unique_ptr<Debug::IGizmos> CreateGizmos();
#endif
    };
}

#endif