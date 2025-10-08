// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "SDL3/SDL.h"

#include "Packer/PackCatalog.h"
#include "Graphics/AnimationFactory.h"
#include "Graphics/AbstractTextureFactory.h"
#include "../src/Player.h"

using namespace Mochi;

TEST_CASE("Player::1- Player init")
{
    /*FS::PackCatalog catalog(FS::PackCatalog::FileLoaderType::FileSystem);
    Graphics::AnimationFactory af(&catalog);
    Graphics::TextureFactory tf(&)
    Shooter::Player p()*/
    CHECK(true);
}
