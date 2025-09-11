#ifndef HDEF_EXCEPTION
#define HDEF_EXCEPTION

#include <stdexcept>
#include <string>
#include <format>
#include <source_location>

namespace Mochi
{
    class EngineError : public std::runtime_error
    {
    public:
        explicit EngineError(const std::string &msg, std::source_location loc = std::source_location::current())
            : std::runtime_error(std::format("{} [{}:{}]", msg, loc.file_name(), loc.line())) {};
    };

    class ResourceNotFoundError : public EngineError
    {
    public:
        explicit ResourceNotFoundError(const std::string &resource, const std::string &hint = "") : EngineError(std::format("Resource not found: {} (({}))", resource, hint)) {}
    };

    class SystemInitializationError : public EngineError
    {
    public:
        explicit SystemInitializationError(const std::string &system, const std::string &msg) : EngineError(std::format("Error initializing system {}: {}", system, msg)) {}
    };

    class MalformedInputAction : public EngineError
    {
    public:
        explicit MalformedInputAction(std::source_location loc = std::source_location::current()) : EngineError("JSON input action was malformed", loc) {}
    };

    class AudioOperationError : public EngineError
    {
    public:
        explicit AudioOperationError(const std::string &operation, const std::string &msg) : EngineError(std::format("Audio operation failed: {} (({}))", operation, msg)) {}
    };
}
#endif