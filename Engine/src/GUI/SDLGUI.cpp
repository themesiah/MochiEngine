#include "SDLGUI.h"
#include "AbstractGUI.h"

#include "SDL3/SDL.h"
#include "SDL3_ttf/SDL_ttf.h"

#include "../Constants.h"
#include "../Packer/PackCatalog.h"
#include "../Graphics/AbstractTextureFactory.h"
#include "../Graphics/IRenderer.h"
#include "../Graphics/SDL/SDLRenderer.h"
#include "../Graphics/SDL/SDLTexture.h"
#include "../Graphics/SDL/SDLTextureFactory.h"
#include "../Input/IActionManager.h"
#include "../Utils/Assert.h"
#include "../Types/Types.hpp"

namespace Mochi::Graphics
{
    SDLGUI::SDLGUI(FS::PackCatalog *catalog, IRenderer *renderer, Input::IActionManager *actionManager)
        : AbstractGUI(catalog, renderer, actionManager), mFont({nullptr, TTF_CloseFont})
    {
        if (!TTF_Init())
        {
            throw SystemInitializationError("GUI", SDL_GetError());
        }

        auto fontBuffer = mCatalog->GetFile(CONST_MAIN_FONT_PATH);
        void *fontCopy = SDL_malloc(fontBuffer.size());
        SDL_memcpy(fontCopy, fontBuffer.data(), fontBuffer.size());
        SDL_IOStream *rw = SDL_IOFromMem(fontCopy, fontBuffer.size());
        auto font = TTF_OpenFontIO(rw, true, CONST_DEVBUILD_TEXT_SIZE);
        if (!font)
        {
            throw SystemInitializationError("GUI", SDL_GetError());
        }
        mFont.reset(font);
    }

    SDLGUI::~SDLGUI()
    {
    }

    bool SDLGUI::Button(const char *label, const float &textSize, Rectf dstRect, const std::string &texturePath)
    {
        auto tex = mTextureFactory->GetTexture(texturePath);
        SDLTexture *sdltex = dynamic_cast<SDLTexture *>(tex.get());
        if (!sdltex)
        {
            return false;
        }

        auto *ren = dynamic_cast<SDLRenderer *>(mRenderer);

        SDL_FRect dst = dstRect;
        SDL_RenderTexture9Grid(ren->GetRenderer(), sdltex->GetTexture(), NULL, 10, 10, 10, 10, 0, &dst);
        Text(label, textSize, {dstRect.x + dstRect.w / 2 - (textSize * strlen(label) / 4), dstRect.y + dstRect.h / 2 - textSize / 2}, {255, 255, 255, 255});

        if (mActionManager->Performed("UISelect"))
        {
            auto mousePos = mActionManager->CompoundValue("MousePosX", "MousePosY");
            if (mousePos.x > dstRect.x && mousePos.x < dstRect.x + dstRect.w && mousePos.y > dstRect.y && mousePos.y < dstRect.y + dstRect.h)
            {
                return true;
            }
        }

        return false;
    }

    void SDLGUI::Text(const char *label, const float &textSize, Vector2f position, const SDL_Color &color)
    {
        auto *ren = dynamic_cast<SDLRenderer *>(mRenderer);
        auto renderer = ren->GetRenderer();
        SDL_Surface *surface = TTF_RenderText_Solid(mFont.get(), label, 0, color);
        ASSERT(std::format("Surface is null: {}", SDL_GetError()), surface != NULL);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        ASSERT("Texture is null", texture != NULL);
        SDL_SetTextureScaleMode(texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST); // SUPER IMPORTANT!

        SDL_RendererLogicalPresentation *rlp{NULL};
        int logicalW, logicalH;
        SDL_GetRenderLogicalPresentation(renderer, &logicalW, &logicalH, rlp);

        float w, h;
        SDL_GetTextureSize(texture, &w, &h);
        SDL_FRect dstRect{position.x, position.y, w / h * textSize, textSize};
        SDL_RenderTexture(renderer, texture, NULL, &dstRect);
        SDL_DestroySurface(surface);
        SDL_DestroyTexture(texture);
    }

}