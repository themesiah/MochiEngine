
#ifndef HDEF_ASSERT
#define HDEF_ASSERT

#include <iostream>

/**
 * @defgroup Assert Assertion macros
 *
 * Utils/Assert.h
 * @{
 */

/**
 * \def ASSERT(message, x)
 * \brief Asserts and show a message if the predicate is false
 */
#ifndef DEBUG
#define ASSERT(message, x)
#else
#define ASSERT(message, x)                                     \
    if (!(x))                                                  \
    {                                                          \
        std::cout << "!!!!!ASSERTION FAILED!!!!!" << std::endl \
                  << "\"" << message << "\"" << std::endl      \
                  << #x << std::endl                           \
                  << "File: " << __FILE__ << std::endl         \
                  << "Line: " << __LINE__ << std::endl;        \
        exit(-1);                                              \
    }
#endif
#endif

/**@} */