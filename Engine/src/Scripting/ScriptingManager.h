#ifndef HDEF_SCRIPTINGMANAGER
#define HDEF_SCRIPTINGMANAGER

#include <sol/sol.hpp>

namespace Mochi::FS
{
    class PackCatalog;
}
namespace Mochi::Scripting
{
    class ScriptingManager
    {
    private:
        FS::PackCatalog *mCatalog;

    public:
        ScriptingManager(FS::PackCatalog *packCatalog);
        ~ScriptingManager();
        void Execute(const std::string &code);
        void ExecuteFile(const std::string &path);
        sol::state State;
    };
}

#endif