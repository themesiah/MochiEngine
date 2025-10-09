#ifndef HDEF_SDLGUI
#define HDEF_SDLGUI

#include "AbstractGUI.h"

#include <memory>
#include <vector>

#include "../Types/Types.hpp"

struct TTF_Font;
namespace Mochi::Graphics
{
    class SDLRenderer;
    class IRenderer;
    class SDLGUI : public AbstractGUI
    {
    private:
        friend class SDLRenderer;
        std::unique_ptr<TTF_Font, void (*)(TTF_Font *)> mFont;
        std::vector<char> mFontRaw;
        SDLGUI(FS::PackCatalog *catalog, IRenderer *renderer, Input::IActionManager *actionManager);

    public:
        virtual ~SDLGUI();
        virtual bool Button(const char *label, const float &textSize, Rectf dstRect, const std::string &texturePath) override;
        virtual void Text(const char *label, const float &textSize, Vector2f position, const Color &color) override;
    };
}

#endif