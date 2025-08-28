// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>

#include "Constants.h"

TEST_CASE("Init TTF and sample text")
{
    bool success;
    SDL_Window *window;
    SDL_Renderer *renderer;
    success = SDL_Init(SDL_INIT_VIDEO);
    REQUIRE(success);
    success = SDL_CreateWindowAndRenderer("TEST", 640, 480, 0, &window, &renderer);
    REQUIRE(success);
    success = TTF_Init();
    REQUIRE(success);

    TTF_Font *font = TTF_OpenFont(std::format("{}/{}", CONST_DATA_FOLDER, CONST_MAIN_FONT_PATH).c_str(), CONST_DEVBUILD_TEXT_SIZE);
    REQUIRE(font);
    TTF_TextEngine *textEngine = TTF_CreateRendererTextEngine(renderer);
    REQUIRE(textEngine);
    TTF_Text *text = TTF_CreateText(textEngine, font, "This is a test text", 0);
    REQUIRE(text);

    int w, h;
    TTF_GetTextSize(text, &w, &h);
    CHECK_GE(w, 0);
    CHECK_GE(h, 0);
    CHECK(TTF_GetTextEngine(text) == textEngine);
}