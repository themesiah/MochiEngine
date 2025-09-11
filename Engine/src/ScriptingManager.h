#ifndef HDEF_SCRIPTINGMANAGER
#define HDEF_SCRIPTINGMANAGER

#include <sol/sol.hpp>
#include <memory>

namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Scripting
{
    class ScriptingManager
    {
    private:
        std::shared_ptr<FS::PackCatalog> mCatalog;

    public:
        ScriptingManager(std::shared_ptr<FS::PackCatalog> packCatalog);
        ~ScriptingManager();
        void Execute(const std::string &code);
        void ExecuteFile(const std::string &path);
        sol::state State;
    };
}

#endif