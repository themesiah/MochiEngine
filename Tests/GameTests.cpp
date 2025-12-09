// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <memory>

#include "Engine.h"
#include "Graphics/DummyRendering.h"
#include "Input/VirtualActionManager.h"
#include "Audio/DummyAudioManager.h"
#include "Layer.h"
#include "Graphics/AsepriteAnimationFactory.h"

#include "GameLayer.h"
#include "Enemies/Enemy.h"
#include "Player.h"
#include "Bullets/PlayerBulletPool.h"
#include "Bullets/EnemyBulletPoolFollow.h"

#include <iostream>

using namespace Mochi;

class TestLayer : public Shooter::GameLayer
{
public:
    float mTimer;
    std::vector<std::string> mExecutableCode;
    TestLayer(float timer) : Shooter::GameLayer(), mTimer(timer)
    {
    }

    virtual void InitLayer() override
    {
        Shooter::GameLayer::InitLayer();
        for (auto &code : mExecutableCode)
        {
            mScripting->Execute(code);
        }
    }

    void AddCodeToExecute(const std::string &code)
    {
        mExecutableCode.push_back(code);
    }

    virtual bool Update(const float &dt) override
    {
        Shooter::GameLayer::Update(dt);

        mTimer -= Time::TimeSystem::GetUnscaledDeltaTime();
        if (mTimer <= 0.0f)
            return false;
        return true;
    }

    std::shared_ptr<Shooter::Player> GetPlayer() const
    {
        return mPlayer;
    }

    void SetPlayerPosition(const Vector2f &pos)
    {
        mPlayer->GetTransform()->Position = pos;
    }

    std::shared_ptr<Shooter::Enemy> AddEnemy(const Vector2f &pos, const float &scale)
    {
        std::shared_ptr<Shooter::Enemy> enemy = std::make_shared<Shooter::Enemy>(mEventBus, mTextureFactory.get(), mAnimationFactory.get());
        enemy->GetTransform()->Position = pos;
        enemy->GetTransform()->Scale = scale;
        mEnemies.push_back(enemy);
        return enemy;
    }

    void SetEnemyPosition(const Vector2f &pos, int index)
    {
        mEnemies[index]->GetTransform()->Position = pos;
    }

    size_t EnemiesCount() const
    {
        return mEnemies.size();
    }

    void Execute(const std::string &code)
    {
        mScripting->Execute(code);
    }
};

void DontTruncate()
{
    std::cout << "CTEST_FULL_OUTPUT\n";
}

TEST_CASE("Game::1- Enemy dies")
{
    DontTruncate();
    // Init test
    Engine e = Engine();
    e.GetCatalog()->OpenPack("TestData");
    Input::VirtualActionManager *vam = new Input::VirtualActionManager();
    e.Setup(std::make_unique<Graphics::DummyRenderer>(), std::unique_ptr<Input::VirtualActionManager>(vam), std::make_unique<Audio::DummyAudioManager>());

    TestLayer *testLayer = new TestLayer(1.0f);
    e.PushLayer(testLayer);

    // Set specific state
    testLayer->SetPlayerPosition({-2.0f, 0.0f});
    testLayer->AddEnemy({2.0f, 0.0f}, 2.0f);
    vam->SetActionState("Shot", 1.0f);

    // Run!
    e.Run();
    CHECK(testLayer->EnemiesCount() == 0);
}

TEST_CASE("Game::2- Enemy doesn't die")
{
    DontTruncate();
    // Init test
    Engine e = Engine();
    e.GetCatalog()->OpenPack("TestData");
    Input::VirtualActionManager *vam = new Input::VirtualActionManager();
    e.Setup(std::make_unique<Graphics::DummyRenderer>(), std::unique_ptr<Input::VirtualActionManager>(vam), std::make_unique<Audio::DummyAudioManager>());
    TestLayer *testLayer = new TestLayer(1.0f);
    e.PushLayer(testLayer);

    // Set specific state
    testLayer->SetPlayerPosition({-2.0f, 3.0f});
    testLayer->AddEnemy({2.0f, 0.0f}, 2.0f);
    vam->SetActionState("Shot", 1.0f);

    // Run!
    e.Run();
    CHECK(testLayer->EnemiesCount() > 0);
}

TEST_CASE("Game::3- Player receives damage")
{
    DontTruncate();
    // Init test
    Engine e = Engine();
    e.GetCatalog()->OpenPack("TestData");
    Input::VirtualActionManager *vam = new Input::VirtualActionManager();
    e.Setup(std::make_unique<Graphics::DummyRenderer>(), std::unique_ptr<Input::VirtualActionManager>(vam), std::make_unique<Audio::DummyAudioManager>());
    TestLayer *testLayer = new TestLayer(1.0f);
    e.PushLayer(testLayer);

    int startingHealth = testLayer->GetPlayer()->GetCurrentHealth();

    // Set specific state
    testLayer->SetPlayerPosition({-2.0f, 3.0f});
    testLayer->AddCodeToExecute("ShotBullet(0, Vector2f:new(0,0))");

    // Run!
    e.Run();
    int endHealth = testLayer->GetPlayer()->GetCurrentHealth();
    CHECK(startingHealth > endHealth);
}

TEST_CASE("Game::4- Player dies")
{
    DontTruncate();

    // Init test
    Engine e = Engine();
    e.GetCatalog()->OpenPack("TestData");
    Input::VirtualActionManager *vam = new Input::VirtualActionManager();
    e.Setup(std::make_unique<Graphics::DummyRenderer>(), std::unique_ptr<Input::VirtualActionManager>(vam), std::make_unique<Audio::DummyAudioManager>());
    TestLayer *testLayer = new TestLayer(1.0f);
    e.PushLayer(testLayer);

    testLayer->GetPlayer()->SetHealth(1);
    int startingLives = testLayer->GetPlayer()->GetCurrentLives();

    // Set specific state
    testLayer->SetPlayerPosition({-2.0f, 3.0f});
    testLayer->AddCodeToExecute("ShotBullet(0, Vector2f:new(0,0))");

    // Run!
    e.Run();
    int endLives = testLayer->GetPlayer()->GetCurrentLives();
    CHECK(startingLives > endLives);
}

TEST_CASE("Game::5- Player reespawns")
{
    DontTruncate();

    // Init test
    Engine e = Engine();
    e.GetCatalog()->OpenPack("TestData");
    Input::VirtualActionManager *vam = new Input::VirtualActionManager();
    e.Setup(std::make_unique<Graphics::DummyRenderer>(), std::unique_ptr<Input::VirtualActionManager>(vam), std::make_unique<Audio::DummyAudioManager>());
    TestLayer *testLayer = new TestLayer(1.0f);
    e.PushLayer(testLayer);

    testLayer->GetPlayer()->SetHealth(1);
    testLayer->GetPlayer()->SetLives(1);

    // Set specific state
    testLayer->SetPlayerPosition({-2.0f, 3.0f});
    testLayer->AddCodeToExecute("ShotBullet(0, Vector2f:new(0,0))");

    // Run!
    e.Run();
    testLayer->GetPlayer()->Reespawn();
    int endLives = testLayer->GetPlayer()->GetCurrentLives();
    CHECK(endLives > 0);
}