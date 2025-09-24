// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Physics/Shapes.h"
#include "DoctestUtils.h"

TEST_CASE("Physics::1- PointVSPoint")
{
    Mochi::Physics::Point p1({0, 0});
    Mochi::Physics::Point p2({1, 1});
    Mochi::Physics::Point p3({0, 1});
    Mochi::Physics::Point p4({1, 0});
    Mochi::Physics::Point p5({0, 0});
    CHECK_FALSE(p1.Collides(p2));
    CHECK_FALSE(p1.Collides(p3));
    CHECK_FALSE(p1.Collides(p4));
    CHECK(p1.Collides(p5));
    CHECK_FALSE(p2.Collides(p5));
}

TEST_CASE("Physics::2- PointVSCircle")
{
    Mochi::Physics::Point p1({0, 0});
    Mochi::Physics::Point p2({1, 1});
    Mochi::Physics::Circle c1({2, 1}, 1.5f);

    CHECK_FALSE(p1.Collides(c1));
    CHECK_FALSE(c1.Collides(p1));
    CHECK(p2.Collides(c1));
    CHECK(c1.Collides(p2));
}

TEST_CASE("Physics::3- CircleVSCircle")
{
    Mochi::Physics::Circle c1({2, 2}, 1.0f);
    Mochi::Physics::Circle c2({3, 3}, 0.5f);
    Mochi::Physics::Circle c3({4, 2}, 0.5f);

    CHECK(c1.Collides(c2));
    CHECK_FALSE(c3.Collides(c1));
    CHECK_FALSE(c3.Collides(c2));
}

TEST_CASE("Physics::4- PointVSRectangle")
{
    Mochi::Physics::Point p1({0, 0});
    Mochi::Physics::Point p2({5, 4});
    Mochi::Physics::Point p3({4, 5});
    Mochi::Physics::Point p4({5, 8});
    Mochi::Physics::Rectangle r1({5.5f, 4}, {1.5f, 1});

    CHECK_FALSE(r1.Collides(p1));
    CHECK(r1.Collides(p2));
    CHECK(r1.Collides(p3));
    CHECK_FALSE(r1.Collides(p4));
}

TEST_CASE("Physics::5- RectangleVSRectangle")
{
    Mochi::Physics::Rectangle r1({5.5, 4}, {1.5f, 1});
    Mochi::Physics::Rectangle r2({5, 6}, {1, 1});
    Mochi::Physics::Rectangle r3({7.5f, 5.5f}, {0.5f, 0.5f});
    Mochi::Physics::Rectangle r4({5.5, 4}, {0.5f, 0.5f});

    CHECK(r1.Collides(r2));
    CHECK(r1.Collides(r3));
    CHECK_FALSE(r2.Collides(r3));
    CHECK(r1.Collides(r4));
}

TEST_CASE("Physics::6- CircleVSRectangle")
{
    Mochi::Physics::Rectangle r1({5.5, 4}, {1.5f, 1});
    Mochi::Physics::Circle c1({6, 6}, 1.0f);
    Mochi::Physics::Circle c2({6, 7}, 1.0f);
    Mochi::Physics::Circle c3({3, 5}, 2.0f);
    Mochi::Physics::Circle c4({5.5f, 4}, 0.5f);

    CHECK(r1.Collides(c1));
    CHECK_FALSE(r1.Collides(c2));
    CHECK(r1.Collides(c3));
    CHECK(r1.Collides(c4));
}

TEST_CASE("Physics::7- LineVSPoint")
{
    Mochi::Physics::Line l1({5, 0}, {5, 5});
    Mochi::Physics::Point p1({0, 0});
    Mochi::Physics::Point p2({5, 0});
    Mochi::Physics::Point p3({5, 2.5});
    Mochi::Physics::Point p4({2.5, 5});

    CHECK_FALSE(l1.Collides(p1));
    CHECK(l1.Collides(p2));
    CHECK(l1.Collides(p3));
    CHECK_FALSE(l1.Collides(p4));
}

TEST_CASE("Physics::8- LineVSCircle")
{
    Mochi::Physics::Line l1({5, 0}, {5, 5});
    Mochi::Physics::Line l2({6, 0}, {6, 5});
    Mochi::Physics::Line l3({4, 4}, {6, 6});
    Mochi::Physics::Line l4({4, 6}, {8, 6});
    Mochi::Physics::Circle c1({6, 6}, 1.0f);

    CHECK_FALSE(c1.Collides(l1));
    CHECK(c1.Collides(l2));
    CHECK(c1.Collides(l3));
    CHECK(c1.Collides(l4));
}

TEST_CASE("Physics::9- LineVSLine")
{
    Mochi::Physics::Line l1({5, 0}, {5, 5});
    Mochi::Physics::Line l2({4, 0}, {4, 5});
    Mochi::Physics::Line l3({3, 2}, {7, 2});
    Mochi::Physics::Line l4({5, 0}, {5, 5});
    Mochi::Physics::Line l5({5, 0}, {5, -5});

    CHECK_FALSE(l1.Collides(l2));
    CHECK(l1.Collides(l3));
    CHECK(l1.Collides(l4));
    CHECK(l2.Collides(l3));
    CHECK(l1.Collides(l5));
}

TEST_CASE("Physics::10- LineVSRectangle")
{
    Mochi::Physics::Rectangle r1({5.5, 4}, {1.5f, 1});
    Mochi::Physics::Line l1({5, 0}, {5, 5});
    Mochi::Physics::Line l2({4, 0}, {4, 5});
    Mochi::Physics::Line l3({3, 2}, {7, 2});
    Mochi::Physics::Line l4({6, 2}, {8, 4});
    Mochi::Physics::Line l5({4, 4}, {6, 6});

    CHECK(r1.Collides(l1));
    CHECK(r1.Collides(l2));
    CHECK_FALSE(r1.Collides(l3));
    CHECK(r1.Collides(l4));
    CHECK(r1.Collides(l5));
}

TEST_CASE("Physics::11- Exceptions")
{
    TEST_THROWS(Mochi::Physics::Circle c1({6, 6}, 0.0f));
    TEST_THROWS(Mochi::Physics::Circle c1({6, 6}, -1.0f));
    TEST_NOTHROWS(Mochi::Physics::Circle c1({0, 0}, 3.0f));
}