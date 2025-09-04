#ifndef HDEF_IANIMATABLE
#define HDEF_IANIMATABLE

struct IAnimatable
{
    virtual ~IAnimatable() {}
    virtual void UpdateAnimation(const float &dt) = 0;
};

#endif