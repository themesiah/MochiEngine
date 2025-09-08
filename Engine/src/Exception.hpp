#ifndef HDEF_EXCEPTION
#define HDEF_EXCEPTION

#include <stdexcept>
#include <string>
#include <format>

namespace Mochi
{
    class EngineError : public std::runtime_error
    {
    public:
        explicit EngineError(const std::string &msg) : std::runtime_error(msg) {};
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
}
#endif