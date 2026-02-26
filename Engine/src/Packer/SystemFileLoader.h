#ifndef HDEF_SYSTEMFILELOADER
#define HDEF_SYSTEMFILELOADER

#include "IFileLoader.h"

#include <string>
#include <vector>
#include <filesystem>

namespace Mochi::FS
{
    /// @brief This class creates a very simple virtual file system from a directory.
    /// This allows read access to all files in that directory referencing them from their path as a unique identifier.
    /// As this doesn't give the benefits of asset obfuscation, it is only used on debug mode, and can't be configured at the moment.
    class SystemFileLoader : public IFileLoader
    {
    private:
        std::filesystem::path mDirectoryPath;

    public:
        SystemFileLoader(const std::string &directoryPath);
        virtual ~SystemFileLoader();
        virtual bool IsValid() const;
        virtual std::vector<char> GetFile(const std::string &path);
        virtual bool HasFile(const std::string &path) const;
        virtual std::vector<std::string> GetAvailableFiles() const;
    };
}

#endif