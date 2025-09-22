#ifndef HDEF_PLAYER
#define HDEF_PLAYER

#include <memory>
#include "Entity/IUpdateable.h"
#include "Graphics/RenderCommand.h"
#include "Graphics/Spritesheet.h"
#include "Types/Types.hpp"

namespace Mochi::Input
{
    class ActionManager;
}
class Player : public Mochi::Graphics::Spritesheet, public Mochi::IUpdateable
{
private:
    float mSpeed;
    float mTilt;
    float mTiltSpeed;

public:
    Player(std::shared_ptr<Mochi::Graphics::AnimationFactory> animationFactory, std::shared_ptr<Mochi::Graphics::TextureFactory> textureFactory);
    ~Player();
    virtual void Update(const float &dt, std::shared_ptr<Mochi::Input::ActionManager>);
};

#endif