// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <iostream>
#include <filesystem>
#include <memory>
#include <fmod_studio.h>

#include "Audio/FmodWrapper.h"
#include "Packer/PackCatalog.h"
#include "DoctestUtils.h"
#include "ScriptingManager.h"

TEST_CASE("Audio::1- Init FMOD")
{
    std::shared_ptr<Mochi::FS::PackCatalog> pc = std::make_shared<Mochi::FS::PackCatalog>(Mochi::FS::PackCatalog::FileLoaderType::FileSystem);
    pc->OpenPack("TestData");
    std::shared_ptr<Mochi::Scripting::ScriptingManager> sm = std::make_shared<Mochi::Scripting::ScriptingManager>(pc.get());
    FMOD_RESULT result;
    Mochi::Audio::FMODWrapper fmod(pc.get(), sm.get());
    TEST_NOTHROWS(fmod.LoadBank("Audiobanks/Master"));
    TEST_NOTHROWS(fmod.PlayBGM("TestMusic"));
    TEST_NOTHROWS(fmod.PauseBGM());
    TEST_NOTHROWS(fmod.ResumeBGM());
    TEST_NOTHROWS(fmod.StopBGM());

    TEST_THROWS(fmod.LoadBank("Notexistant/Bank"));
}