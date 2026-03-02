#ifndef HDEF_STRINGUTILS
#define HDEF_STRINGUTILS

#include <string>
#include <algorithm>

namespace Mochi::Utils
{
    /**
     * @defgroup Stringutils String utils
     *
     * Utils/StringUtils.h
     * @{
     */

    /// @brief Replaces all / with \\ and makes all text lowercase. This is used to standarize paths inside the catalog file system.
    /// @param path The original path.
    /// @return The normalized path.
    inline std::string NormalizePath(std::string path)
    {
        // Replace forward slashes with backslashes
        std::replace(path.begin(), path.end(), '/', '\\');

        // Convert to lowercase
        std::transform(path.begin(), path.end(), path.begin(),
                       [](unsigned char c)
                       { return std::tolower(c); });

        return path;
    }

    /**@} */
}

#endif