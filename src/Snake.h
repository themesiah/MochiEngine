#ifndef HDEF_SNAKE
#define HDEF_SNAKE

#include <memory>
#include <string>

#include "Graphics/AnimatedSprite.h"
#include "Entity/IUpdateable.h"

namespace Mochi
{
    namespace Input
    {
        class ActionManager;
    }
    namespace Graphics
    {
        class TextureFactory;
    }
}
class Snake : public Mochi::Graphics::AnimatedSprite, public Mochi::IUpdateable
{
public:
    Snake(std::shared_ptr<Mochi::Graphics::AnimationFactory>, std::shared_ptr<Mochi::Graphics::TextureFactory>, const std::string &animationPath, const std::string &mainAnimation);
    virtual ~Snake();
    virtual void Update(const float &dt, std::shared_ptr<Mochi::Input::ActionManager> mActionManager);
};

#endif