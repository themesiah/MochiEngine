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
namespace Mochi::Graphics
{
    class Camera;
}
class Player : public Mochi::Graphics::Spritesheet, public Mochi::IUpdateable
{
private:
    float mSpeed;
    float mTilt;
    float mTiltSpeed;
    std::shared_ptr<Mochi::Graphics::Camera> mCamera;
    Mochi::Rectf mBounds;

public:
    Player(std::shared_ptr<Mochi::Graphics::AnimationFactory> animationFactory,
           std::shared_ptr<Mochi::Graphics::TextureFactory> textureFactory,
           std::shared_ptr<Mochi::Graphics::Camera> camera);
    ~Player();
    virtual void Update(const float &dt, std::shared_ptr<Mochi::Input::ActionManager>);
};

#endif