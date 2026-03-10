#ifndef HDEF_SDLUTILS
#define HDEF_SDLUTILS

#define SDL_DELETER(Type, Func)                     \
    struct Type##Deleter                            \
    {                                               \
        void operator()(Type *p) const { Func(p); } \
    };

#endif