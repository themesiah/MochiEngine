#ifndef HDEF_IFILELOADER
#define HDEF_IFILELOADER

#include <string>
#include <vector>

class IFileLoader
{
public:
    virtual ~IFileLoader() {};
    virtual bool IsValid() const = 0;
    virtual std::vector<char> GetFile(std::string path) = 0;
    virtual bool HasFile(std::string path) const = 0;
};

#endif