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
    /**
     * @brief This class allows rendering with SDL3 in "the simple way", that is showing textures with SDL_RenderTexture instead of using the GPU module.
     *
     * NOTE: Do not mistake SDLRenderer (this) and SDL_Renderer (renderer struct used by SDL internally).
     *
     * Besides that, SDLRenderer is responsible for creating AbstractTextureFactory of the type SDLTextureFactory that generates SDLTexture and AbstractGUI of the
     * type SDLGUI.
     *
     * Ownership:
     *
     * - The SDL_Renderer as a unique pointer
     *
     * - The application window as a unique pointer
     *
     * Guarantees:
     *
     * - A clean frame generation if StartFrameRendering, Render and FinishRendering are called in order.
     *
     * - That the factories generate textures that can be used by SDLRenderer.
     *
     * - That the GUI system uses the SDLRenderer for showing textures.
     *
     * - That a single SDL_Renderer and SDL_Window will be created and disposed when destroying the SDLRenderer.
     */
    class SDLRenderer : public IRenderer
    {
    private:
        std::unique_ptr<SDL_Renderer, void (*)(SDL_Renderer *)> mRenderer;
        std::unique_ptr<SDL_Window, void (*)(SDL_Window *)> mWindow;

    public:
        /// @brief Initializes the renderer and window for the application, setting the necessary metadata for the game to work.
        /// @param appName Name of the application.
        /// @param appVersion Version of the application.
        /// @param appId Unique string defined as a reverse id used to identify the application.
        /// @param windowName The name that will show on the top of the window.
        SDLRenderer(const char *appName, const char *appVersion, const char *appId, const char *windowName);
        virtual ~SDLRenderer();
        SDL_Renderer *GetRenderer() const { return mRenderer.get(); }
        SDL_Window *GetWindow() const { return mWindow.get(); }
        /// @brief Called before rendering anything. Sets a render color and clears the window.
        virtual void StartFrameRendering() const override;
        /// @brief Orders the textures by zindex and renders them on screen depending on the camera position.
        ///
        /// As this renderer knows that every texture coming in the renderQueue is an SDLTexture, it performs
        // a dynamic cast to SDLTexture before working with it.
        /// @param renderQueue List of render commands to render.
        /// @param camera The camera on which to project the render.
        virtual void Render(std::vector<RenderCommand> renderQueue, Camera *camera) const override;
        /// @brief Present the screen.
        virtual void FinishRendering() const override;
        virtual std::unique_ptr<AbstractTextureFactory> CreateTextureFactory(FS::PackCatalog *catalog) const;
        virtual std::unique_ptr<AbstractGUI> CreateGUI(FS::PackCatalog *catalog, Input::IActionManager *actionManager);
#if DEBUG
        virtual std::unique_ptr<Debug::IGizmos> CreateGizmos();
#endif
    };
}

#endif