#define SOL_NO_MEMORY_ALIGNMENT 1

#include "ScriptingManager.h"

#include <sol/sol.hpp>

#include "Packer/PackCatalog.h"
#include "Utils/Logger.h"

namespace Mochi::Scripting
{

    ScriptingManager::ScriptingManager(FS::PackCatalog *packCatalog) : mCatalog(packCatalog), State()
    {
        State.open_libraries();
        State.set_function("LogOk", [](const std::string &str)
                           { LOG_OK(str); });
        State.set_function("LogInfo", [](const std::string &str)
                           { LOG_INFO(str); });
        State.set_function("LogWarning", [](const std::string &str)
                           { LOG_WARNING(str); });
        State.set_function("LogError", [](const std::string &str)
                           { LOG_ERROR(str); });

        ExecuteFile("Script/LuaInit.lua");
        State.script("return 5");
    }

    ScriptingManager::~ScriptingManager()
    {
    }

    void ScriptingManager::Execute(const std::string &code)
    {
        try
        {
            State.script(code);
        }
        catch (const sol::error &e)
        {
            LOG_ERROR(e.what());
        }
    }

    void ScriptingManager::ExecuteFile(const std::string &path)
    {
        std::vector<char> data = mCatalog->GetFile(path);
        std::string code(data.begin(), data.end());
        State.script(code);
    }
}