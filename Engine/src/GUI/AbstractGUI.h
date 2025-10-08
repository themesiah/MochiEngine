#ifndef HDEF_ABSTRACTGUI
#define HDEF_ABSTRACTGUI

#include <string>
#include <memory>

#include "../Types/Types.hpp"
// #include "../Graphics/AbstractTextureFactory.h"

struct SDL_Color;
namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Input
{
    class IActionManager;
}
namespace Mochi::Graphics
{
    using GUIElementId = unsigned int;

    class IRenderer;
    class AbstractTextureFactory;
    class AbstractGUI
    {
    protected:
        GUIElementId mCurrentId;
        GUIElementId mFocusId;
        std::unique_ptr<AbstractTextureFactory> mTextureFactory;
        IRenderer *mRenderer;
        FS::PackCatalog *mCatalog;
        Input::IActionManager *mActionManager;
        GUIElementId GetNextId();

    public:
        AbstractGUI(FS::PackCatalog *catalog, IRenderer *renderer, Input::IActionManager *actionManager);
        virtual ~AbstractGUI();
        void ResetFrame();

        // UI Elements
        virtual bool Button(const char *label, const float &textSize, Rectf dstRect, const std::string &texturePath) = 0;
        virtual void Text(const char *label, const float &textSize, Vector2f position, const SDL_Color &color) = 0;
    };
}

#endif