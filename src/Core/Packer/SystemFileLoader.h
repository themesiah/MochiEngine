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
    virtual std::vector<char> GetFile(const std::string &path);
    virtual bool HasFile(const std::string &path) const;
    virtual std::vector<std::string> GetAvailableFiles() const;
};

#endif