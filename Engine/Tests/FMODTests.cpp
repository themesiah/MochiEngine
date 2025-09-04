// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <fmod_studio.h>
#include "Audio/FmodWrapper.h"
#include "Packer/PackCatalog.h"

#include <iostream>
#include <filesystem>
#include <memory>

TEST_CASE("Init FMOD")
{
    std::shared_ptr<PackCatalog> pc = std::make_shared<PackCatalog>(PackCatalog::FileLoaderType::FileSystem);
    pc->OpenPack("TestData");
    FMODWrapper fmod = FMODWrapper(pc);
    FMOD_RESULT result = fmod.Init();
    CHECK(result == FMOD_OK);
    result = fmod.LoadBank("Audiobanks/Master");
    REQUIRE(result == FMOD_OK);
    result = fmod.PlayBGM("TestMusic");
    REQUIRE(result == FMOD_OK);
    result = fmod.PauseBGM();
    CHECK(result == FMOD_OK);
    result = fmod.ResumeBGM();
    CHECK(result == FMOD_OK);
    result = fmod.StopBGM();
    CHECK(result == FMOD_OK);
}