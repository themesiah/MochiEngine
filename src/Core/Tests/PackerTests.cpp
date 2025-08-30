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

#include "PackCatalog.h"
#include "PackFile.h"
#include "SystemFileLoader.h"
#include "CoreConstants.h"
#include "Input/ActionManager.h"
#include "Input/InputManager.h"

TEST_CASE("SDL texture Packfile")
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

TEST_CASE("SDL texture Filesystem")
{
    SystemFileLoader dir(CONST_DATA_FOLDER);
    REQUIRE(dir.IsValid());
    bool success;
    SDL_Window *window;
    SDL_Renderer *renderer;
    success = SDL_Init(SDL_INIT_VIDEO);
    REQUIRE(success);
    success = SDL_CreateWindowAndRenderer("TEST", 640, 480, 0, &window, &renderer);
    REQUIRE(success);

    REQUIRE(dir.HasFile("Coproducers.jpg"));
    auto buffer = dir.GetFile("Coproducers.jpg");
    SDL_Texture *texture = IMG_LoadTexture_IO(renderer, SDL_IOFromConstMem(buffer.data(), buffer.size()), true);
    REQUIRE(texture != NULL);
    CHECK(true);
}

TEST_CASE("Load audiobank Packfile")
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

TEST_CASE("Load audiobank Filesystem")
{
    SystemFileLoader dir(CONST_DATA_FOLDER);
    REQUIRE(dir.IsValid());

    FMOD_RESULT result;
    FMOD_STUDIO_SYSTEM *system;
    result = FMOD_Studio_System_Create(&system, FMOD_VERSION);
    REQUIRE(result == FMOD_OK);
    result = FMOD_Studio_System_Initialize(system, 512, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
    REQUIRE(result == FMOD_OK);

    REQUIRE(dir.HasFile("Audiobanks/Master.bank"));
    REQUIRE(dir.HasFile("Audiobanks/Master.strings.bank"));
    auto masterBuffer = dir.GetFile("Audiobanks/Master.bank");
    FMOD_STUDIO_BANK *masterBank;
    result = FMOD_Studio_System_LoadBankMemory(system, masterBuffer.data(), masterBuffer.size(), FMOD_STUDIO_LOAD_MEMORY_MODE::FMOD_STUDIO_LOAD_MEMORY, 0, &masterBank);
    CHECK(result == FMOD_OK);
    auto stringsBuffer = dir.GetFile("Audiobanks/Master.strings.bank");
    FMOD_STUDIO_BANK *stringsBank;
    result = FMOD_Studio_System_LoadBankMemory(system, stringsBuffer.data(), stringsBuffer.size(), FMOD_STUDIO_LOAD_MEMORY_MODE::FMOD_STUDIO_LOAD_MEMORY, 0, &stringsBank);
    CHECK(result == FMOD_OK);
}

TEST_CASE("Load fonts Packfile")
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

TEST_CASE("Load fonts Filesystem")
{
    SystemFileLoader dir(CONST_DATA_FOLDER);
    REQUIRE(dir.IsValid());

    bool success;
    SDL_Window *window;
    SDL_Renderer *renderer;
    success = SDL_Init(SDL_INIT_VIDEO);
    REQUIRE(success);
    success = SDL_CreateWindowAndRenderer("TEST", 640, 480, 0, &window, &renderer);
    REQUIRE(success);
    success = TTF_Init();
    REQUIRE(success);

    REQUIRE(dir.HasFile("Fonts/SuperTechnology.ttf"));
    auto buffer = dir.GetFile("Fonts/SuperTechnology.ttf");
    TTF_Font *font = TTF_OpenFontIO(SDL_IOFromConstMem(buffer.data(), buffer.size()), true, 16.0f);
    REQUIRE(font);
}

TEST_CASE("Load actions Packfile")
{
    PackFile file(std::format("{}/Data.pak", CONST_DATA_FOLDER));
    REQUIRE(file.IsValid());

    std::shared_ptr<InputManager> inputManager = std::make_shared<InputManager>();
    ActionManager actionManager = ActionManager(inputManager);
    REQUIRE(file.HasFile("Actions.json"));
    auto buffer = file.GetFile("Actions.json");
    bool success = actionManager.LoadActions(buffer);
    REQUIRE(success);
    CHECK(actionManager.HasAction("Debug1"));
}

TEST_CASE("Load actions Filesystem")
{
    SystemFileLoader dir(CONST_DATA_FOLDER);
    REQUIRE(dir.IsValid());

    std::shared_ptr<InputManager> inputManager = std::make_shared<InputManager>();
    ActionManager actionManager = ActionManager(inputManager);
    REQUIRE(dir.HasFile("Actions.json"));
    auto buffer = dir.GetFile("Actions.json");
    bool success = actionManager.LoadActions(buffer);
    REQUIRE(success);
    CHECK(actionManager.HasAction("Debug1"));
}

TEST_CASE("Catalog test filesystem")
{
    PackCatalog pc(PackCatalog::FileLoaderType::FileSystem);

    CHECK_FALSE(pc.IsPackOpen("data"));
    CHECK_FALSE(pc.HasFile("Actions.json"));
    pc.OpenPack("Data");
    REQUIRE(pc.IsPackOpen("data"));
    CHECK(pc.HasFile("Actions.json"));
}

TEST_CASE("Catalog test packfile")
{
    PackCatalog pc(PackCatalog::FileLoaderType::Packfile);

    CHECK_FALSE(pc.IsPackOpen("TestData/data"));
    CHECK_FALSE(pc.HasFile("Actions.json"));
    pc.OpenPack("TestData/Data");
    REQUIRE(pc.IsPackOpen("TestData/data"));
    CHECK(pc.HasFile("Actions.json"));
}