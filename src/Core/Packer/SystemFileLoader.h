#ifndef HDEF_SYSTEMFILELOADER
#define HDEF_SYSTEMFILELOADER

#include "IFileLoader.h"

#include <string>
#include <vector>
#include <filesystem>

class SystemFileLoader : public IFileLoader
{
private:
    std::filesystem::path mDirectoryPath;

public:
    SystemFileLoader(const std::string &directoryPath);
    virtual ~SystemFileLoader();
    virtual bool IsValid() const;
    virtual std::vector<char> GetFile(std::string path);
    virtual bool HasFile(std::string path) const;
};

#endif