#ifndef HDEF_IFILELOADER
#define HDEF_IFILELOADER

#include <string>
#include <vector>

namespace Mochi::FS
{
    /// @brief Interface to get files that are part of the game data.
    /// Note that the interface does not have any save or write function. This is not meant to create persistent file systems, but to create virtual file systems
    /// from which to read.
    class IFileLoader
    {
    public:
        virtual ~IFileLoader() {};
        /// @brief Tells us if the file loader can be used at the moment.
        /// @return True if can be used, false if not
        virtual bool IsValid() const = 0;

        /// @brief Give us a binary representation of a file in a filesystem
        /// @throws EngineError if is not valid or doesn't have file
        /// @param path A path, or unique identifier to the file
        /// @return A vector of chars, or binary representation of a file.
        virtual std::vector<char> GetFile(const std::string &path) = 0;

        /// @brief Tells us if the filesystem has a file
        /// @param path A path, or unique identifier to the file
        /// @return True if it has the file, false if not
        virtual bool HasFile(const std::string &path) const = 0;

        /// @brief Tells us which files are in the filesystem
        /// @return A vector with the unique identifiers of all files
        virtual std::vector<std::string> GetAvailableFiles() const = 0;
    };
}

#endif