// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>
#include <string>

#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <fmod_studio.h>
#include <fmod.h>
#include <fmod_errors.h>

#include "PackFile.h"
#include "CoreConstants.h"

TEST_CASE("Unpack SDL texture")
{
    PackFile file(std::format("{}/Data.pak", CONST_DATA_FOLDER));
    REQUIRE(file.IsValid());
    bool success;
    SDL_Window *window;
    SDL_Renderer *renderer;
    success = SDL_Init(SDL_INIT_VIDEO);
    REQUIRE(success);
    success = SDL_CreateWindowAndRenderer("TEST", 640, 480, 0, &window, &renderer);
    REQUIRE(success);

    REQUIRE(file.HasFile("Coproducers.jpg"));
    auto buffer = file.GetFile("Coproducers.jpg");
    SDL_Texture *texture = IMG_LoadTexture_IO(renderer, SDL_IOFromConstMem(buffer.data(), buffer.size()), true);
    REQUIRE(texture != NULL);
    CHECK(true);
}

TEST_CASE("Unpack audiobank")
{
    PackFile file(std::format("{}/Data.pak", CONST_DATA_FOLDER));
    REQUIRE(file.IsValid());

    FMOD_RESULT result;
    FMOD_STUDIO_SYSTEM *system;
    result = FMOD_Studio_System_Create(&system, FMOD_VERSION);
    REQUIRE(result == FMOD_OK);
    result = FMOD_Studio_System_Initialize(system, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
    REQUIRE(result == FMOD_OK);

    REQUIRE(file.HasFile("Audiobanks/Master.bank"));
    REQUIRE(file.HasFile("Audiobanks/Master.strings.bank"));
    auto masterBuffer = file.GetFile("Audiobanks/Master.bank");
    FMOD_STUDIO_BANK *masterBank;
    result = FMOD_Studio_System_LoadBankMemory(system, masterBuffer.data(), masterBuffer.size(), FMOD_STUDIO_LOAD_MEMORY_MODE::FMOD_STUDIO_LOAD_MEMORY, 0, &masterBank);
    CHECK(result == FMOD_OK);
    auto stringsBuffer = file.GetFile("Audiobanks/Master.strings.bank");
    FMOD_STUDIO_BANK *stringsBank;
    result = FMOD_Studio_System_LoadBankMemory(system, stringsBuffer.data(), stringsBuffer.size(), FMOD_STUDIO_LOAD_MEMORY_MODE::FMOD_STUDIO_LOAD_MEMORY, 0, &stringsBank);
    CHECK(result == FMOD_OK);
}

TEST_CASE("Load fonts")
{
    PackFile file(std::format("{}/Data.pak", CONST_DATA_FOLDER));
    REQUIRE(file.IsValid());

    bool success;
    SDL_Window *window;
    SDL_Renderer *renderer;
    success = SDL_Init(SDL_INIT_VIDEO);
    REQUIRE(success);
    success = SDL_CreateWindowAndRenderer("TEST", 640, 480, 0, &window, &renderer);
    REQUIRE(success);
    success = TTF_Init();
    REQUIRE(success);

    REQUIRE(file.HasFile("Fonts/SuperTechnology.ttf"));
    auto buffer = file.GetFile("Fonts/SuperTechnology.ttf");
    TTF_Font *font = TTF_OpenFontIO(SDL_IOFromConstMem(buffer.data(), buffer.size()), true, 16.0f);
    REQUIRE(font);
}