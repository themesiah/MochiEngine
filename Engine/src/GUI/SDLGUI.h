#ifndef HDEF_SDLGUI
#define HDEF_SDLGUI

#include "AbstractGUI.h"

#include <memory>
#include <vector>

#include "../Types/Types.hpp"
#include "GUICommon.hpp"

struct TTF_Font;
namespace Mochi::Graphics
{
    class SDLRenderer;
    class IRenderer;
    /**
     * @brief Implementation of AbstractGUI that uses SDLRenderer and SDLTTF to show GUI elements.
     *
     * NOTE: This class is provisional. It uses a fixed font and has minimal functionality for a GUI system.
     * This is not to be called a final product, but it does the trick.
     */
    class SDLGUI : public AbstractGUI
    {
    private:
        friend class SDLRenderer;
        std::unique_ptr<TTF_Font, void (*)(TTF_Font *)> mFont;
        std::vector<char> mFontRaw;
        SDLGUI(FS::PackCatalog *catalog, IRenderer *renderer, Input::IActionManager *actionManager);
        SDLRenderer *mSDLRenderer;

    public:
        virtual ~SDLGUI();
        virtual GUIResult Sprite(const GUIOptions &options) override;
        virtual GUIResultButton Button(const GUIButtonOptions &options, const char *label, const GUITextOptions &textOptions) override;
        virtual GUIResult Text(const char *label, const GUITextOptions &options) override;
    };
}

#endif