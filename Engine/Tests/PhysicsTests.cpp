// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Physics/Shapes.h"

TEST_CASE("Physics::1- PointVSPoint")
{
    Mochi::Physics::Point p1({0, 0}, 0, 0);
    Mochi::Physics::Point p2({1, 1}, 0, 0);
    Mochi::Physics::Point p3({0, 1}, 0, 0);
    Mochi::Physics::Point p4({1, 0}, 0, 0);
    Mochi::Physics::Point p5({0, 0}, 0, 0);
    CHECK_FALSE(p1.Collides(p2));
    CHECK_FALSE(p1.Collides(p3));
    CHECK_FALSE(p1.Collides(p4));
    CHECK(p1.Collides(p5));
    CHECK_FALSE(p2.Collides(p5));
}

TEST_CASE("Physics::2- PointVSCircle")
{
    Mochi::Physics::Point p1({0, 0}, 0, 0);
    Mochi::Physics::Point p2({1, 1}, 0, 0);
    Mochi::Physics::Circle c1({2, 1}, 0, 0, 1.5f);

    CHECK_FALSE(p1.Collides(c1));
    CHECK_FALSE(c1.Collides(p1));
    CHECK(p2.Collides(c1));
    CHECK(c1.Collides(p2));
}

TEST_CASE("Physics::3- CircleVSCircle")
{
    Mochi::Physics::Circle c1({2, 2}, 0, 0, 1.0f);
    Mochi::Physics::Circle c2({3, 3}, 0, 0, 0.5f);
    Mochi::Physics::Circle c3({4, 2}, 0, 0, 0.5f);

    CHECK(c1.Collides(c2));
    CHECK_FALSE(c3.Collides(c1));
    CHECK_FALSE(c3.Collides(c2));
}

TEST_CASE("Physics::4- PointVSRectangle")
{
    Mochi::Physics::Point p1({0, 0}, 0, 0);
    Mochi::Physics::Point p2({5, 4}, 0, 0);
    Mochi::Physics::Point p3({4, 5}, 0, 0);
    Mochi::Physics::Point p4({5, 8}, 0, 0);
    Mochi::Physics::Rectangle r1({5.5f, 4}, 0, 0, {1.5f, 1});

    CHECK_FALSE(r1.Collides(p1));
    CHECK(r1.Collides(p2));
    CHECK(r1.Collides(p3));
    CHECK_FALSE(r1.Collides(p4));
}

TEST_CASE("Physics::5- RectangleVSRectangle")
{
    Mochi::Physics::Rectangle r1({5.5, 4}, 0, 0, {1.5f, 1});
    Mochi::Physics::Rectangle r2({5, 6}, 0, 0, {1, 1});
    Mochi::Physics::Rectangle r3({7.5f, 5.5f}, 0, 0, {0.5f, 0.5f});
    Mochi::Physics::Rectangle r4({5.5, 4}, 0, 0, {0.5f, 0.5f});

    CHECK(r1.Collides(r2));
    CHECK(r1.Collides(r3));
    CHECK_FALSE(r2.Collides(r3));
    CHECK(r1.Collides(r4));
}

TEST_CASE("Physics::6- CircleVSRectangle")
{
    Mochi::Physics::Rectangle r1({5.5, 4}, 0, 0, {1.5f, 1});
    Mochi::Physics::Circle c1({6, 6}, 0, 0, 1.0f);
    Mochi::Physics::Circle c2({6, 7}, 0, 0, 1.0f);
    Mochi::Physics::Circle c3({3, 5}, 0, 0, 2.0f);
    Mochi::Physics::Circle c4({5.5f, 4}, 0, 0, 0.5f);

    CHECK(r1.Collides(c1));
    CHECK_FALSE(r1.Collides(c2));
    CHECK(r1.Collides(c3));
    CHECK(r1.Collides(c4));
}

TEST_CASE("Physics::7- LineVSPoint")
{
    Mochi::Physics::Line l1({5, 0}, 0, 0, {5, 5});
    Mochi::Physics::Point p1({0, 0}, 0, 0);
    Mochi::Physics::Point p2({5, 0}, 0, 0);
    Mochi::Physics::Point p3({5, 2.5}, 0, 0);
    Mochi::Physics::Point p4({2.5, 5}, 0, 0);

    CHECK_FALSE(l1.Collides(p1));
    CHECK(l1.Collides(p2));
    CHECK(l1.Collides(p3));
    CHECK_FALSE(l1.Collides(p4));
}