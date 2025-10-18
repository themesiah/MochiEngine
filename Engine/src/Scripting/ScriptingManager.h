#ifndef HDEF_SCRIPTINGMANAGER
#define HDEF_SCRIPTINGMANAGER

#include "sol/sol.hpp"
#include <vector>

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
        sol::environment mEnvironment;

    public:
        ScriptingManager(FS::PackCatalog *packCatalog);
        ~ScriptingManager();
        void Execute(const std::string &code);
        void ExecuteFile(const std::string &path);
        void ExecuteGlobal(const std::string &code);
        void ExecuteFileGlobal(const std::string &path);
        void Update(const float &dt);
        void CleanEnvironment();
        sol::state State;
    };
}

#endif