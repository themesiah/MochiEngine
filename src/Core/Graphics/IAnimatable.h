#ifndef HDEF_IANIMATABLE
#define HDEF_IANIMATABLE

class IAnimatable
{
public:
    virtual ~IAnimatable() {}
    virtual void UpdateAnimation(const float &dt) = 0;
};

#endif