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
    class IRenderer
    {
    public:
        virtual ~IRenderer() {}
        virtual void StartFrameRendering() const = 0;
        virtual void Render(std::vector<RenderCommand> renderQueue, Camera *camera) const = 0;
        virtual void FinishRendering() const = 0;
        virtual std::unique_ptr<Camera> CreateCamera() const = 0;
        virtual std::unique_ptr<AbstractTextureFactory> CreateTextureFactory(FS::PackCatalog *) const = 0;
        virtual std::unique_ptr<AbstractGUI> CreateGUI(FS::PackCatalog *catalog, Input::IActionManager *actionManager) = 0;
        virtual std::unique_ptr<Debug::IGizmos> CreateGizmos() = 0;
    };
}

#endif