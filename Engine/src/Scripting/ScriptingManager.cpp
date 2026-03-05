#define SOL_NO_MEMORY_ALIGNMENT 1
#define SOL_ALL_SAFETIES_ON 1

#include "ScriptingManager.h"

#include <sol/sol.hpp>
#include <string_view>

#include "../Packer/PackCatalog.h"
#include "../Utils/Logger.h"
#include "../Utils/StringUtils.h"

namespace Mochi::Scripting
{
    ScriptingManager::ScriptingManager(FS::PackCatalog *packCatalog) : mCatalog(packCatalog), mEnvironment(sol::nil), State()
    {
        State.open_libraries(sol::lib::base, sol::lib::coroutine, sol::lib::table, sol::lib::debug, sol::lib::math, sol::lib::string);
        State.set_function("LogOk", [](const std::string &str)
                           { LOG_OK(str); });
        State.set_function("LogInfo", [](const std::string &str)
                           { LOG_INFO(str); });
        State.set_function("LogWarning", [](const std::string &str)
                           { LOG_WARNING(str); });
        State.set_function("LogError", [](const std::string &str)
                           { LOG_ERROR(str); });

        CleanEnvironment();
        ExecuteFileGlobal("Script/LuaInit.lua");
        ExecuteFileGlobal("Script/Coroutines.lua");
        ExecuteFileGlobal("Script/Math.lua");
    }

    ScriptingManager::~ScriptingManager()
    {
        if (mEnvironment != sol::nil)
            mEnvironment.abandon();
    }

    void ScriptingManager::Execute(const std::string_view &code)
    {
        try
        {
            if (mEnvironment != sol::nil)
            {
                State.script(code, mEnvironment, sol::script_throw_on_error);
            }
        }
        catch (const sol::error &e)
        {
            LOG_ERROR(e.what());
        }
    }

    void ScriptingManager::ExecuteFile(const std::string &path)
    {
        auto normalizePath = Utils::NormalizePath(path);
        std::vector<char> data = mCatalog->GetFile(normalizePath);
        std::string code(data.begin(), data.end());
        Execute(code);
    }

    void ScriptingManager::ExecuteGlobal(const std::string_view &code)
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

    void ScriptingManager::ExecuteFileGlobal(const std::string &path)
    {
        auto normalizePath = Utils::NormalizePath(path);
        std::vector<char> data = mCatalog->GetFile(normalizePath);
        std::string code(data.begin(), data.end());
        ExecuteGlobal(code);
    }

    void ScriptingManager::Update(const float &dt)
    {
        if (mEnvironment != sol::nil)
        {
            mEnvironment["UpdateCoroutines"](dt);
        }
    }

    void ScriptingManager::CleanEnvironment()
    {
        sol::table globals = State.globals();
        mEnvironment = sol::environment(State, sol::create, State.globals());
        mEnvironment[sol::metatable_key] = State.create_table_with("__index", globals);
    }
}