#ifndef HDEF_DUMMYRENDERER
#define HDEF_DUMMYRENDERER

#include <memory>

#include "IRenderer.h"
#include "AbstractTextureFactory.h"
#include "ITexture.h"
#include "../GUI/AbstractGUI.h"
#include "../Debug/IGizmos.h"

namespace Mochi::Graphics
{
    class DummyTexture : public ITexture
    {
    public:
        DummyTexture() {}
        virtual ~DummyTexture() {}
        virtual Vector2f GetSize() const override { return {0.0f, 0.0f}; };
    };

    class DummyTextureFactory : public AbstractTextureFactory
    {
    public:
        DummyTextureFactory() {}
        virtual ~DummyTextureFactory() {}
        virtual std::shared_ptr<ITexture> LoadTexture(const std::string &texturePath) override { return std::make_shared<DummyTexture>(); };
    };

    class DummyGUI : public AbstractGUI
    {
    public:
        DummyGUI(FS::PackCatalog *catalog, IRenderer *renderer, Input::IActionManager *actionManager) : AbstractGUI(catalog, renderer, actionManager) {}
        virtual ~DummyGUI() {}
        virtual bool Button(const char *label, const float &textSize, Rectf dstRect, const std::string &texturePath) override { return false; }
        virtual void Text(const char *label, const float &textSize, Vector2f position, const Color &color) override {}
    };

    class DummyGizmos : public Debug::IGizmos
    {
    public:
        DummyGizmos() {}
        virtual ~DummyGizmos() {}
        virtual void DrawRectangle(const Physics::Rectangle *rectangle, const Color &color) const override {};
        virtual void DrawCircle(const Physics::Circle *circle, const Color &color) const override {};
        virtual void DrawLine(const Physics::Line *line, const Color &color) const override {};
        virtual void DrawPoint(const Physics::Point *point, const Color &color) const override {};
    };

    class DummyRenderer : public IRenderer
    {
    public:
        DummyRenderer() {}
        virtual ~DummyRenderer() {}
        virtual void StartFrameRendering() const override {}
        virtual void Render(std::vector<RenderCommand> renderQueue, Camera *camera) const override {}
        virtual void FinishRendering() const override {}
        virtual std::unique_ptr<AbstractTextureFactory> CreateTextureFactory(FS::PackCatalog *) const override { return std::make_unique<DummyTextureFactory>(); }
        virtual std::unique_ptr<AbstractGUI> CreateGUI(FS::PackCatalog *catalog, Input::IActionManager *actionManager) override { return std::make_unique<DummyGUI>(catalog, this, actionManager); }
#if DEBUG
        virtual std::unique_ptr<Debug::IGizmos> CreateGizmos() override { return std::make_unique<DummyGizmos>(); }
#endif
    };
}

#endif