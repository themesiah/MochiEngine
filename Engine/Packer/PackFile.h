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