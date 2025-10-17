#define SOL_NO_MEMORY_ALIGNMENT 1
#define SOL_ALL_SAFETIES_ON 1

#include "ScriptingManager.h"

#include <sol/sol.hpp>
#include <string_view>

#include "../Packer/PackCatalog.h"
#include "../Utils/Logger.h"

namespace Mochi::Scripting
{
    ScriptingManager::ScriptingManager(FS::PackCatalog *packCatalog) : mCatalog(packCatalog), mTasks(), State()
    {
        State.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::table);
        State.set_function("LogOk", [](const std::string &str)
                           { LOG_OK(str); });
        State.set_function("LogInfo", [](const std::string &str)
                           { LOG_INFO(str); });
        State.set_function("LogWarning", [](const std::string &str)
                           { LOG_WARNING(str); });
        State.set_function("LogError", [](const std::string &str)
                           { LOG_ERROR(str); });

        ExecuteFile("Script/LuaInit.lua");

        State.set_function("start_task",
                           [this](sol::function func)
                           {
                               sol::coroutine co = func;
                               if (co.valid())
                               {
                                   mTasks.push_back({co, 0.0f});
                               }
                           });
    }

    ScriptingManager::~ScriptingManager()
    {
    }

    void ScriptingManager::Execute(const std::string &code)
    {
        try
        {
            State.script(code, sol::script_throw_on_error);
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
        Execute(code);
    }

    void ScriptingManager::Update(const float &dt)
    {
        auto it = mTasks.begin();
        while (it != mTasks.end())
        {
            auto &task = *it;
            if (task.WaitTime > 0.0f)
            {
                task.WaitTime -= dt;
                ++it;
                continue;
            }

            sol::protected_function_result result = task.Coroutine();

            auto status = result.status();
            if (result.valid() && result.get_type() == sol::type::number)
            {
                task.WaitTime = result.get<float>();
                ++it;
            }
            else if (result.valid() && status == sol::call_status::yielded && result.get_type() == sol::type::none)
            {
                task.WaitTime = 0.0f;
                ++it;
            }
            else
            {
                it = mTasks.erase(it);
            }
        }
    }
}