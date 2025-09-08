#include "GUI.h"

#include "../Packer/PackCatalog.h"
#include "../Graphics/Renderer.h"

#include "../Input/ActionManager.h"
#include "../Input/InputManager.h"

#include "../Constants.h"
#include "../Utils/Assert.h"
#include "../Utils/Logger.h"

namespace Mochi::Graphics
{
    GUI::GUI(std::shared_ptr<FS::PackCatalog> catalog, std::shared_ptr<Renderer> renderer, std::shared_ptr<Input::ActionManager> actionManager)
        : mCurrentId(0), mFocusId(0), mRenderer(renderer), mActionManager(actionManager)
    {
        mTextureFactory = std::make_unique<TextureFactory>(catalog, renderer->GetRenderer());

        if (!TTF_Init())
        {
            SDL_Log("Couldn't init ttf: %s", SDL_GetError());
            throw SDL_APP_FAILURE;
        }

        auto fontBuffer = catalog->GetFile(CONST_MAIN_FONT_PATH);
        void *fontCopy = SDL_malloc(fontBuffer.size());
        SDL_memcpy(fontCopy, fontBuffer.data(), fontBuffer.size());
        SDL_IOStream *rw = SDL_IOFromMem(fontCopy, fontBuffer.size());
        mFont = std::shared_ptr<TTF_Font>(TTF_OpenFontIO(rw, true, CONST_DEVBUILD_TEXT_SIZE), TTF_CloseFont);
        if (!mFont)
        {
            SDL_Log("Couldn't load %s: %s", CONST_MAIN_FONT_PATH, SDL_GetError());
            throw SDL_APP_FAILURE;
        }
    }

    GUI::~GUI()
    {
    }

    void GUI::ResetFrame()
    {
        mCurrentId = 0;
    }

    GUIElementId GUI::GetNextId()
    {
        return mCurrentId++;
    }

    bool GUI::Button(const char *label, const float &textSize, SDL_FRect dstRect, const std::string &texturePath)
    {
        auto tex = mTextureFactory->GetTexture(texturePath);

        SDL_RenderTexture9Grid(mRenderer->GetRenderer().get(), tex.get(), NULL, 10, 10, 10, 10, 0, &dstRect);
        GUI::Text(label, textSize, {dstRect.x + dstRect.w / 2 - (textSize * strlen(label) / 4), dstRect.y + dstRect.h / 2 - textSize / 2}, {255, 255, 255, 255});

        if (mActionManager->GetInputManager()->MouseWasPressed(1))
        {
            auto mousePos = mActionManager->GetInputManager()->GetMousePosition();
            if (mousePos.x > dstRect.x && mousePos.x < dstRect.x + dstRect.w && mousePos.y > dstRect.y && mousePos.y < dstRect.y + dstRect.h)
            {
                return true;
            }
        }

        return false;
    }

    void GUI::Text(const char *label, const float &textSize, Vector2f position, const SDL_Color &color)
    {
        SDL_Surface *surface = TTF_RenderText_Solid(mFont.get(), label, 0, color);
        ASSERT(std::format("Surface is null: {}", SDL_GetError()), surface != NULL);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(mRenderer->GetRenderer().get(), surface);
        ASSERT("Texture is null", texture != NULL);
        SDL_SetTextureScaleMode(texture, SDL_ScaleMode::SDL_SCALEMODE_NEAREST); // SUPER IMPORTANT!

        SDL_RendererLogicalPresentation *rlp{NULL};
        int logicalW, logicalH;
        SDL_GetRenderLogicalPresentation(mRenderer->GetRenderer().get(), &logicalW, &logicalH, rlp);

        float w, h;
        SDL_GetTextureSize(texture, &w, &h);
        SDL_FRect dstRect{position.x, position.y, w / h * textSize, textSize};
        SDL_RenderTexture(mRenderer->GetRenderer().get(), texture, NULL, &dstRect);
        SDL_DestroySurface(surface);
        SDL_DestroyTexture(texture);
    }
}