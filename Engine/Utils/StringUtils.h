#ifndef HDEF_STRINGUTILS
#define HDEF_STRINGUTILS

#include <string>
#include <algorithm>

namespace Mochi
{
    namespace Utils
    {
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
    }
}
#endif