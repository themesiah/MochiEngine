#ifndef HDEF_LOGGER
#define HDEF_LOGGER

#include <iostream>

#ifndef DEBUG
#define LOG_INFO(x)
#define LOG_OK(x)
#define LOG_WARNING(x)
#define LOG_ERROR(x)
#define LOG_PANIC(x)
#else

inline constexpr const char *LOG_NOCOLOR = "\033[0m";
inline constexpr const char *LOG_COLOR_GREEN = "\x1B[32m";
inline constexpr const char *LOG_COLOR_YELLOW = "\x1B[33m";
inline constexpr const char *LOG_COLOR_RED = "\x1B[31m";
inline constexpr const char *LOG_COLOR_PANIC = "\x1B[91m";

#define LOG_INFO(x) (std::cout << LOG_NOCOLOR << x << " (" << __FILE__ << ":" << __LINE__ << ")" << LOG_NOCOLOR << std::endl)
#define LOG_OK(x) (std::cout << LOG_COLOR_GREEN << x << " (" << __FILE__ << ":" << __LINE__ << ")" << LOG_NOCOLOR << std::endl)
#define LOG_WARNING(x) (std::cout << LOG_COLOR_YELLOW << x << " (" << __FILE__ << ":" << __LINE__ << ")" << LOG_NOCOLOR << std::endl)
#define LOG_ERROR(x) (std::cout << LOG_COLOR_RED << x << " (" << __FILE__ << ":" << __LINE__ << ")" << LOG_NOCOLOR << std::endl)
#define LOG_PANIC(x) (std::cout << LOG_COLOR_PANIC << x << " (" << __FILE__ << ":" << __LINE__ << ")" << LOG_NOCOLOR << std::endl)
#endif
#endif