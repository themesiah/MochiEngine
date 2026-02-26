#ifndef HDEF_IRENDERER
#define HDEF_IRENDERER

#include <vector>
#include <memory>

#include "RenderCommand.h"

namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Debug
{
    class IGizmos;
}
namespace Mochi::Input
{
    class IActionManager;
}
struct SDL_Renderer;
namespace Mochi::Graphics
{
    class Camera;
    class AbstractTextureFactory;
    class AbstractGUI;

    /// @brief This interface provides means to show graphics in the game and create the subsystems needed to create textures
    /// for the same renderer and GUI using the same renderer.
    /// The interface provides separate methods for rendering to meet the needs of different kind of renderers (initialize, load to gpu, present, for example).
    class IRenderer
    {
    public:
        virtual ~IRenderer() {}
        /// @brief Initializes a frame
        virtual void StartFrameRendering() const = 0;
        /// @brief Actually renders the contents of the render queue projected on the camera.
        /// @param renderQueue The vector of render commands to be drawn.
        /// @param camera The camera on which to project the rendering.
        virtual void Render(std::vector<RenderCommand> renderQueue, Camera *camera) const = 0;
        /// @brief Finishes a frame rendering
        virtual void FinishRendering() const = 0;
        /// @brief Creates (without ownership) a texture factory of the type compatible with the renderer.
        /// @param catalog The file catalog used for the factory to load files from.
        /// @return A unique pointer to the created texture factory.
        virtual std::unique_ptr<AbstractTextureFactory> CreateTextureFactory(FS::PackCatalog *catalog) const = 0;
        /// @brief Creates (without ownership) the GUI system compatible with the renderer.
        /// @param catalog The file catalog used for the GUI system to load files from.
        /// @param actionManager The action manager, used to manage UI interactions.
        /// @return A unique pointer to the created gui system.
        virtual std::unique_ptr<AbstractGUI> CreateGUI(FS::PackCatalog *catalog, Input::IActionManager *actionManager) = 0;
#if DEBUG
        /// @brief Creates (without ownership) a gizmos system compatible with the renderer.
        /// This system is only relevant on debug mode
        /// @return A unique pointer to the created gizmos system.
        virtual std::unique_ptr<Debug::IGizmos> CreateGizmos() = 0;
#endif
    };
}

#endif