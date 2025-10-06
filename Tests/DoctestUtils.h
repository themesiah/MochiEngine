#ifndef HDEF_DOCTESTUTILS
#define HDEF_DOCTESTUTILS

#include "doctest.h"

#define TEST_THROWS(expression) \
    try                         \
    {                           \
        expression;             \
        CHECK(false);           \
    }                           \
    catch (...)                 \
    {                           \
        CHECK(true);            \
    }
#define TEST_NOTHROWS(expression) \
    try                           \
    {                             \
        expression;               \
        CHECK(true);              \
    }                             \
    catch (...)                   \
    {                             \
        CHECK(false);             \
    }

#endif