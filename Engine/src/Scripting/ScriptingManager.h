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
    struct LuaTask
    {
        sol::coroutine Coroutine;
        float WaitTime;
    };

    class ScriptingManager
    {
    private:
        FS::PackCatalog *mCatalog;
        std::vector<LuaTask> mTasks;

    public:
        ScriptingManager(FS::PackCatalog *packCatalog);
        ~ScriptingManager();
        void Execute(const std::string &code);
        void ExecuteFile(const std::string &path);
        void Update(const float &dt);
        sol::state State;
    };
}

#endif