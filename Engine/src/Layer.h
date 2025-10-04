#ifndef HDEF_LAYER
#define HDEF_LAYER

namespace Mochi
{
    class Layer
    {
    public:
        Layer() {}
        ~Layer() {}
        virtual bool Update(const float &dt) = 0;
        virtual void Render() const = 0;
        virtual void GUI() const = 0;
    };
}

#endif