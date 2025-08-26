// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <fmod_studio.h>
#include "Audio/FmodWrapper.h"

#include <iostream>
#include <filesystem>

TEST_CASE("Init FMOD")
{
    FMODWrapper fmod = FMODWrapper();
    FMOD_RESULT result = fmod.Init();
    CHECK(result == FMOD_OK);
    result = fmod.LoadBank("Master");
    CHECK(result == FMOD_OK);
    result = fmod.PlayBGM("TestMusic");
    CHECK(result == FMOD_OK);
    result = fmod.PauseBGM();
    CHECK(result == FMOD_OK);
    result = fmod.ResumeBGM();
    CHECK(result == FMOD_OK);
    result = fmod.StopBGM();
    CHECK(result == FMOD_OK);
}