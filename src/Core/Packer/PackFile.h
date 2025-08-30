#ifndef HDEF_PACKFILE
#define HDEF_PACKFILE

#include "IFileLoader.h"

#include <string>
#include <unordered_map>
#include <fstream>
#include <stdint.h>
#include <vector>

inline constexpr const char *PACKFILE_EXTENSION = "pak";

typedef uint16_t PathLenSize_t;
typedef uint64_t PackHeaderLength_t;
typedef uint64_t FileOffset_t;
typedef uint64_t FileSize_t;
typedef uint32_t FileCount_t;

struct FileEntry
{
    std::string path;
    FileOffset_t offset;
    FileSize_t size;
};
typedef std::unordered_map<std::string, FileEntry> Header_t;

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
    virtual std::vector<char> GetFile(std::string path);
    virtual bool HasFile(std::string path) const;
};

#endif