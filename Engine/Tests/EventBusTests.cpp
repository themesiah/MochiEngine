// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Event/EventBus.h"

struct TestEvent
{
    std::string text;
    int number;
};

struct TestEvent2
{
};

TEST_CASE("Event bus")
{
    Mochi::Event::EventBus bus;

    auto handler1 = bus.Subscribe<TestEvent>([&](const TestEvent &e)
                                             { CHECK(e.number == 5); });

    bus.Publish<TestEvent>((TestEvent){"Hello world", 5});
    bus.Unsubscribe<TestEvent>(handler1);
    bus.Publish<TestEvent>((TestEvent){"Hello world", 4});

    handler1 = bus.Subscribe<TestEvent>([&](const TestEvent &e)
                                        { CHECK(e.number == 42); });
    auto handler2 = bus.Subscribe<TestEvent>([&](const TestEvent &e)
                                             { CHECK_FALSE(e.number == 43); });

    bus.Publish<TestEvent>((TestEvent){"Other test", 42});
}

TEST_CASE("Event bus handler recycling")
{
    Mochi::Event::EventBus bus;
    auto handler1 = bus.Subscribe<TestEvent>([&](const TestEvent &e) {});
    bus.Unsubscribe<TestEvent>(handler1);
    auto handler2 = bus.Subscribe<TestEvent>([&](const TestEvent &e) {});
    CHECK_EQ(handler1, handler2);

    Mochi::Event::EventBus bus2;
    handler1 = bus2.Subscribe<TestEvent>([&](const TestEvent &e) {});
    handler2 = bus2.Subscribe<TestEvent>([&](const TestEvent &e) {});
    bus2.Unsubscribe<TestEvent>(handler1);
    bus2.Unsubscribe<TestEvent>(handler1);
    bus2.Unsubscribe<TestEvent>(handler1);
    bus2.Unsubscribe<TestEvent>(handler1);
    auto handler3 = bus2.Subscribe<TestEvent>([&](const TestEvent &e) {});
    CHECK_EQ(handler1, handler3);
    bus2.Unsubscribe<TestEvent2>(handler3);
    auto handler4 = bus2.Subscribe<TestEvent>([&](const TestEvent &e) {});
    CHECK_NE(handler3, handler4);
}