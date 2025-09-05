#ifndef HDEF_IFILELOADER
#define HDEF_IFILELOADER

#include <string>
#include <vector>

namespace Mochi::FS
{
    class IFileLoader
    {
    public:
        virtual ~IFileLoader() {};
        virtual bool IsValid() const = 0;
        virtual std::vector<char> GetFile(const std::string &) = 0;
        virtual bool HasFile(const std::string &) const = 0;
        virtual std::vector<std::string> GetAvailableFiles() const = 0;
    };
}

#endif