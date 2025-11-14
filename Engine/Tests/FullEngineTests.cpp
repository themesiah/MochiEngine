// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Engine.h"
#include "Graphics/DummyRendering.h"
#include "Input/VirtualActionManager.h"
#include "Audio/DummyAudioManager.h"
#include "Layer.h"

#include <iostream>

using namespace Mochi;

class TestLayer : public Layer
{
private:
public:
    TestLayer() : mTimer(0.2f), mFrames(0) {}
    virtual ~TestLayer() {}
    virtual bool Update(const float &dt)
    {
        mFrames++;
        mTimer -= dt;
        if (mTimer <= 0.0f)
            return false;
        return true;
    }

    virtual void InitLayer() override {}
    virtual void Render() const {}
    virtual void GUI() const {}
    float mTimer;
    int mFrames;
};

TEST_CASE("Engine::Regresssion::1- First delta time")
{
    // There was an error on which the first delta time of the engine was the timestamp.
    Engine e = Engine();
    e.Setup(std::make_unique<Graphics::DummyRenderer>(), std::make_unique<Input::VirtualActionManager>(), std::make_unique<Audio::DummyAudioManager>());
    TestLayer *testLayer = new TestLayer();
    e.PushLayer(testLayer);
    e.Run();
    CHECK(testLayer->mTimer <= 0.0f);
    CHECK(testLayer->mFrames > 1); // if there is just one frame when finishing, it means the delta time was too big
}