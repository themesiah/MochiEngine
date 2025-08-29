#ifndef HDEF_CCONSTANTS
#define HDEF_CCONSTANTS
#ifdef CTEST
inline constexpr const char *CONST_DATA_FOLDER = "TestData";
#else
inline constexpr const char *CONST_DATA_FOLDER = "Data";
#endif

#endif