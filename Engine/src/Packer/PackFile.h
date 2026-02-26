#ifndef HDEF_PACKFILE
#define HDEF_PACKFILE

#include "IFileLoader.h"

#include <string>
#include <unordered_map>
#include <fstream>
#include <stdint.h>
#include <vector>

namespace Mochi::FS
{
    inline constexpr const char *PACKFILE_EXTENSION = "pak";

    using PathLenSize_t = uint16_t;
    using PackHeaderLength_t = uint64_t;
    using FileOffset_t = uint64_t;
    using FileSize_t = uint64_t;
    using FileCount_t = uint32_t;

    struct FileEntry
    {
        std::string path;
        FileOffset_t offset;
        FileSize_t size;
    };
    using Header_t = std::unordered_map<std::string, FileEntry>;

    struct PackHeader
    {
        PackHeaderLength_t HeaderLength;
        size_t BlobSize;
        Header_t HeaderMap;
    };

    /// @brief PackFile is a class with two purposes.
    /// First, it implements IFileLoader, allowing to be used as a simple file system.
    /// Second, it has the Pack and Unpack methods, used to create .pak files, used by this file system.
    /// Creating .pak files has several benefits. The first one is obfuscation, making it more difficult to end users to
    /// get access to your game assets. The second one is being loaded as a stream, as it allows to only load what is needed at the moment.
    /// It also is easily extended to, for example, zip all or only specific files, reducing final build size.
    /// Ownership:
    /// - It takes ownership of the point in memory where the pak file is. However, it is created as a stream and not loaded fully on initialization.
    /// Guarantees:
    /// - If the file exist at the specified path and is a valid pak file, the metadata will be loaded and the pack open.
    /// Thread safe:
    /// - GetFile is not thread safe, as it uses a single handler to seek and read data.
    /// Check IFileLoader.h for more info on the interface methods.
    class PackFile : public IFileLoader
    {
    private:
        PackHeader mHeader;
        std::ifstream mHandler;
        template <typename T>
        static T ReadBinary(std::ifstream &in);
        std::vector<char> UnpackFile(const std::string &filePath);
        void LoadPackIndex();

    public:
        PackFile(const std::string &packfilePath);
        virtual ~PackFile();
        virtual bool IsValid() const;
        static size_t Pack(const std::string &folderPath, const std::string &outPath);
        size_t Unpack(const std::string &outFolder);
        virtual std::vector<char> GetFile(const std::string &outFolder);
        virtual bool HasFile(const std::string &outFolder) const;
        virtual std::vector<std::string> GetAvailableFiles() const;
    };
}

#endif