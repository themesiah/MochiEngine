// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Types/Types.hpp"
#include <SDL3/SDL.h>

using namespace Mochi;

TEST_CASE("Vector2f conversion with SDL_FPoint")
{
    SDL_FPoint p1{5, 10};
    Vector2f v1 = p1;
    CHECK_EQ(v1.x, 5);
    CHECK_EQ(v1.y, 10);

    Vector2f v2{7, 14};
    SDL_FPoint p2 = v2;
    CHECK_EQ(p2.x, 7);
    CHECK_EQ(p2.y, 14);
}

TEST_CASE("Vector2f arithmetic operations")
{
    Vector2f v1{5, 10};
    Vector2f v2{1, 3};
    Vector2f v3 = v1 + v2;
    CHECK(v1.x == 5);
    CHECK(v1.y == 10);
    CHECK(v2.x == 1);
    CHECK(v2.y == 3);
    CHECK(v3.x == 6);
    CHECK(v3.y == 13);

    Vector2f v4{1, 2};
    Vector2f v5{3, 4};
    v4 += v5;
    CHECK(v4.x == 4);
    CHECK(v4.y == 6);
    CHECK(v5.x == 3);
    CHECK(v5.y == 4);

    Vector2f v6{1, 1};
    Vector2f v7{5, 6};
    Vector2f v8 = v6 - v7;
    CHECK(v6.x == 1);
    CHECK(v6.y == 1);
    CHECK(v7.x == 5);
    CHECK(v7.y == 6);
    CHECK(v8.x == -4);
    CHECK(v8.y == -5);

    Vector2f v9{3, 7};
    Vector2f v10{3, 7};
    Vector2f v11{7, 3};
    CHECK(v9 == v10);
    CHECK(v10 == v9);
    CHECK(v9 == v9);
    CHECK_FALSE(v9 == v11);
    CHECK(v9 != v11);

    Vector2f v12{5, 8};
    Vector2f v13 = v12;
    CHECK(v12 == v13);
    v13.x = 1;
    CHECK(v12 != v13);
}

TEST_CASE("Vector2f Arithmetics with casts")
{
    Vector2f v1{1, 1};
    SDL_FPoint p1{2, 3};
    v1 += p1;
    CHECK(v1.x == 3);
    CHECK(v1.y == 4);

    Vector2f v2{1, 1};
    SDL_FPoint p2{2, 3};
    Vector2f v3 = p2 + v2;
    CHECK(v3.x == 3);
    CHECK(v3.y == 4);
    CHECK(v2.x == 1);
    CHECK(v2.y == 1);
}

TEST_CASE("Vector2f Distance")
{
    Vector2f v1{1, 0};
    CHECK_EQ(v1.Distance(), 1);
    Vector2f v2{-1, 0};
    CHECK_EQ(v2.Distance(), 1);
    Vector2f v3{5, -7};
    CHECK_EQ(v3.Distance(), sqrtf(74));
}

TEST_CASE("Vector2f Normalize")
{
    Vector2f v1{0, 0};
    Vector2f v2 = v1.Normalized();
    CHECK(v2.x == 0);
    CHECK(v2.y == 0);

    Vector2f v3{1, 1};
    auto v4 = v3.Normalized();
    CHECK(v4.x == 1 / sqrtf(2));
    CHECK(v4.y == 1 / sqrtf(2));

    Vector2f v5{1, 0};
    auto v6 = v5.Normalized();
    CHECK(v6.x == 1);
    CHECK(v6.y == 0);

    Vector2f v7{3, 6};
    v7.Normalize();
    CHECK_FALSE(v7.x == 3);
    CHECK_FALSE(v7.y == 6);
    CHECK(v7.x == 3 / sqrtf(45));
    CHECK(v7.y == 6 / sqrtf(45));
}

TEST_CASE("Vector2f Cosine proximity")
{
    Vector2f v1{1, 0};
    Vector2f v2{5, 0};
    float dot = v1.CosineProximity(v2);
    float dot2 = Vector2f::CosineProximity(v1, v2);
    CHECK_EQ(dot, dot2);
    CHECK(dot == 1);

    Vector2f v3{0, 45};
    float dot3 = Vector2f::CosineProximity(v1, v3);
    CHECK(dot3 == 0);

    Vector2f v4{0, -45};
    float dot4 = v1.CosineProximity(v4);
    CHECK(dot4 == 0);

    Vector2f v5{-3, 0};
    float dot5 = Vector2f::CosineProximity(v1, v5);
    CHECK(dot5 == -1);
}

TEST_CASE("Vector2f constants")
{
    Vector2f v1{1, 0};
    Vector2f v2 = Vector2f::Right;
    CHECK(v1 == v2);
    v2.x = 5;
    Vector2f v3 = Vector2f::Right;
    CHECK(v3.x == 1);
}

TEST_CASE("Rectf conversion with SDL_FRect")
{
    Rectf r1{0, 0, 1, 1};
    SDL_FRect sr1 = r1;
    CHECK(sr1.x == 0);
    CHECK(sr1.w == 1);

    SDL_FRect sr2{1, 1, 2, 2};
    Rectf r2 = sr2;
    CHECK(r2.x == 1);
    CHECK(r2.w == 2);
}

TEST_CASE("Rectf arithmetic operations")
{
    Rectf r1{0, 0, 1, 1};
    Vector2f v1{5, -3};
    r1 += v1;
    CHECK(r1.x == 5);
    CHECK(r1.y == -3);
    CHECK(r1.w == 1);
    CHECK(r1.h == 1);

    Rectf r2{0, 0, 1, 1};
    SDL_FPoint p1{3, -7};
    r2 -= p1;
    CHECK(r2.x == -3);
    CHECK(r2.y == 7);
    CHECK(r2.w == 1);
    CHECK(r2.h == 1);

    Rectf r3{0, 0, 1, 1};
    Vector2f v2{1, 1};
    SDL_FRect p2 = r3 + v2;
    CHECK(p2.x == 1);
    CHECK(p2.y == 1);
    CHECK(p2.w == 1);
    CHECK(p2.h == 1);
}

TEST_CASE("Rectf scale")
{
    Rectf r1{5, 10, 1, 2};
    r1.Scale(2);
    CHECK(r1.x == 5);
    CHECK(r1.y == 10);
    CHECK(r1.w == 2);
    CHECK(r1.h == 4);
}

TEST_CASE("Rectf position")
{
    Rectf r1{5, 10, 1, 1};
    auto pos = r1.GetPosition();
    CHECK(pos.x == 5);
    CHECK(pos.y == 10);
}

TEST_CASE("Rectf size")
{
    Rectf r1{5, 10, 1, 5};
    auto size = r1.GetSize();
    CHECK(size.x == 1);
    CHECK(size.y == 5);
}

TEST_CASE("Rectf Texture valid")
{
    Rectf r1{0, 0, 0, 1};
    Rectf r2{0, 0, 1, 0};
    Rectf r3{0, 0, -1, -1};
    Rectf r4{0, 0, 1, 1};

    CHECK_FALSE(r1.IsTextureValid());
    CHECK_FALSE(r2.IsTextureValid());
    CHECK_FALSE(r3.IsTextureValid());
    CHECK(r4.IsTextureValid());
}