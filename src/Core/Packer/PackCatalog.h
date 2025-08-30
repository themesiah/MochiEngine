#ifndef HDEF_PACKCATALOG
#define HDEF_PACKCATALOG

#include <vector>
#include <memory>
#include <string>

class IFileLoader;
class PackCatalog
{

public:
    enum FileLoaderType
    {
        FileSystem = 0,
        Packfile = 1
    };
    PackCatalog(FileLoaderType);
    ~PackCatalog();
    bool OpenPack(const std::string &);
    bool ClosePack(const std::string &);
    bool IsPackOpen(const std::string &);
    std::vector<char> GetFile(const std::string &) const;
    bool HasFile(const std::string &) const;

private:
    struct Pack
    {
        std::string packId;
        std::shared_ptr<IFileLoader> fileLoader;
    };
    std::vector<Pack> mPacks;
    FileLoaderType mLoaderType;
};

#endif