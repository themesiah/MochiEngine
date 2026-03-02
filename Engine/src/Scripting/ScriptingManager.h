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
    /**
     * @brief ScriptingManager is a class containing the necessary methods to load and execute lua scripts binded to the C++ code.
     *
     * It allows separating script executions by a cleanable environment and global executions, updating script coroutines and executing from plain code or from a file.
     *
     * It also allows direct use of sol::state, providing advanced and manual use of the lua api. You can for example bind existing sprites from C++ in lua to manage their
     * transforms, or capture lua data in tables directly from C++. This is dangerous and the user should be careful to manage lifetimes correctly when accesing the state directly.
     *
     * @remark As future work, this class could work with a map of environments, or passing the environment as a parameter on execute. For now, having an environment that
     * can be cleaned would allow to load new scenes with a clean slate of level specific lua tables.
     *
     * Ownership:
     *
     * - This class owns the sol2 state (which is the lua state).
     *
     * - This class also owns the environment used to clean lua metadata.
     *
     *
     * Guarantees:
     *
     * - Lua will be initialized with the needed lua libraries and MochiEngine specific libraries.
     *
     * - Lua state and environment state will start clean (besides the pre-loaded MochiEngine methods).
     *
     * - All coroutines will be called each frame.
     *
     *
     * Non responsible:
     *
     * - No scripts besides the initialization ones will be loaded automatically.
     *
     * - Executed scripts with any kind of errors will throw an error. There is no guarantee that the state of the game will be correct after that.
     *
     * - References taken or injected directly using the public sol::state should be user managed. They can bring unexpected results if lifetimes are not well coordinated.
     *
     *
     * Thread safety:
     *
     * - This class is not thread safe. Code should be executed from the main thread. (TODO: Executions should be stored and done in the update. That way all besides update
     * could be thread safe)
     */
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