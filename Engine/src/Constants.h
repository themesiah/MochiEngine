#ifndef HDEF_CONSTANTS
#define HDEF_CONSTANTS

#include <string>

namespace Mochi
{
    inline constexpr const char *CONST_DEVBUILD_TEXT = "DEVELOPMENT BUILD";
    inline constexpr float CONST_DEVBUILD_TEXT_SIZE = 16.0f;

    inline constexpr const char *CONST_MAIN_FONT_PATH = "Fonts/SuperTechnology.ttf";

    /// @brief Path to the actions file used by the default Engine constructor
    inline const std::string CONST_ACTIONS_FILE = "Actions.json";

    /// @brief Width of the logical screen used for the application
    /// The final resolution will be just a reescaling of this resolution
    inline constexpr int CONST_RENDER_LOGICAL_X = 640;

    /// @brief Height of the logical screen used for the application
    /// The final resolution will be just a reescaling of this resolution
    inline constexpr int CONST_RENDER_LOGICAL_Y = 360;
}

#endif