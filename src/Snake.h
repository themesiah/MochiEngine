#ifndef HDEF_SNAKE
#define HDEF_SNAKE

#include <memory>
#include <string>

#include "Graphics/AnimatedSprite.h"
#include "Entity/IUpdateable.h"

class ActionManager;
class TextureFactory;
class Snake : public AnimatedSprite, public IUpdateable
{
public:
    Snake(std::shared_ptr<AnimationFactory>, std::shared_ptr<TextureFactory>, const std::string &animationPath, const std::string &mainAnimation);
    virtual ~Snake();
    virtual void Update(const float &dt, std::shared_ptr<ActionManager> mActionManager);
};

#endif