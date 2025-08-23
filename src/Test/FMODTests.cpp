// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <fmod.h>
#include <fmod_common.h>

TEST_CASE("Init FMOD")
{
    FMOD_RESULT result;
    FMOD_SYSTEM *system = NULL;
    result = FMOD_System_Create(&system, FMOD_VERSION);
    CHECK(result == FMOD_OK);
    result = FMOD_System_Init(system, 512, FMOD_INIT_NORMAL, 0);
    CHECK(result == FMOD_OK);
    result = FMOD_System_Release(system);
    CHECK(result == FMOD_OK);
}