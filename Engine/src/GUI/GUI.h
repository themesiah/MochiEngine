#ifndef HDEF_GUI
#define HDEF_GUI

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <memory>
#include <vector>

#include "../Graphics/TextureFactory.h"
#include "../Types/Types.hpp"

namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Input
{
    class ActionManager;
}
namespace Mochi::Graphics
{
    using GUIElementId = unsigned int;

    class Renderer;
    class GUI
    {
    private:
        GUIElementId mCurrentId;
        GUIElementId mFocusId;
        std::unique_ptr<TextureFactory> mTextureFactory;
        std::shared_ptr<Renderer> mRenderer;
        std::shared_ptr<Input::ActionManager> mActionManager;

        std::shared_ptr<TTF_Font> mFont;
        GUIElementId GetNextId();

    public:
        GUI(std::shared_ptr<FS::PackCatalog> catalog, std::shared_ptr<Renderer> renderer, std::shared_ptr<Input::ActionManager> actionManager);
        ~GUI();
        void ResetFrame();
        bool Button(const char *label, const float &textSize, SDL_FRect dstRect, const std::string &texturePath);
        void Text(const char *label, const float &textSize, Vector2f position, const SDL_Color &color);
    };
}

#endif