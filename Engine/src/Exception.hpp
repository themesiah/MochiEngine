#ifndef HDEF_EXCEPTION
#define HDEF_EXCEPTION

#include <stdexcept>
#include <string>
#include <format>
#include <source_location>

namespace Mochi
{

    /**
     * @defgroup Exception Engine Exceptions
     *
     * List of exception classes thrown in the engine.
     *
     * Errors thrown on the setup phase terminate the application, as they are considered errors that one can not recover from
     * (like a core asset not being loaded, or a submodule not being setup correctly).
     *
     * Errors thrown while on the update phase are to be checked manually and continue the execution or report a "false" as a return
     * in the update in order to finish execution. Any EngineError without a explicit type (any inheriting from it) will terminate the application
     * anyway. If your code can throw any error, catch that specific error or the game will crash.
     *
     * @{
     */

    /**
     * @brief Generic error that can be thrown inside the MochiEngine.
     *
     * This is meant to be extended by specific errors to have better control on try/catch clauses.
     *
     * Any EngineError thrown in the update method will be considered an unmanaged exception.
     */
    class EngineError : public std::runtime_error
    {
    public:
        explicit EngineError(const std::string &msg, std::source_location loc = std::source_location::current())
            : std::runtime_error(std::format("{} [{}:{}]", msg, loc.file_name(), loc.line())) {};
    };

    /**
     * @brief Specific implementation of EngineError for resources that are not found in the specified path.
     */
    class ResourceNotFoundError : public EngineError
    {
    public:
        explicit ResourceNotFoundError(const std::string &resource, const std::string &hint = "") : EngineError(std::format("Resource not found: {} (({}))", resource, hint)) {}
    };

    /**
     * @brief Specific implementation of EngineError for subsystems that have not been initialized correctly.
     */
    class SystemInitializationError : public EngineError
    {
    public:
        explicit SystemInitializationError(const std::string &system, const std::string &msg) : EngineError(std::format("Error initializing system {}: {}", system, msg)) {}
    };

    /**
     * @brief Specific implementation of EngineError thrown when actions loaded by an Input::IActionManager are malformed.
     */
    class MalformedInputAction : public EngineError
    {
    public:
        explicit MalformedInputAction(std::source_location loc = std::source_location::current()) : EngineError("JSON input action was malformed", loc) {}
    };

    /**
     * @brief Specific implementation of EngineError thrown when there is an error on the audio subsystem.
     */
    class AudioOperationError : public EngineError
    {
    public:
        explicit AudioOperationError(const std::string &operation, const std::string &msg) : EngineError(std::format("Audio operation failed: {} (({}))", operation, msg)) {}
    };

    /**@} */
}
#endif