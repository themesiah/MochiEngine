#ifndef HDEF_IRENDERER
#define HDEF_IRENDERER

#include <vector>
#include <memory>

#include "RenderCommand.h"

namespace Mochi::FS
{
    class PackCatalog;
}
struct SDL_Renderer;
namespace Mochi::Graphics
{
    class Camera;
    class AbstractTextureFactory;
    class IRenderer
    {
    public:
        virtual ~IRenderer() {}
        virtual void StartFrameRendering() const = 0;
        virtual void Render(std::vector<RenderCommand> renderQueue, Camera *camera) const = 0;
        virtual void FinishRendering() const = 0;
        virtual std::unique_ptr<Camera> CreateCamera() const = 0;
        virtual std::unique_ptr<AbstractTextureFactory> CreateTextureFactory(FS::PackCatalog *) = 0;
    };
}

#endif