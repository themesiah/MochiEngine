#ifndef HDEF_TYPES
#define HDEF_TYPES

#include <SDL3/SDL.h>
#include <math.h>

namespace Mochi
{

    struct Vector2f
    {
        float x, y;

        Vector2f(float _x = 0, float _y = 0) : x(_x), y(_y) {}
        Vector2f(const Vector2f &v) : x(v.x), y(v.y) {}
        Vector2f(Vector2f &&v) : x(v.x), y(v.y) {}

        // SDL_FPoint conversion
        Vector2f(const SDL_FPoint &p) : x(p.x), y(p.y) {}
        inline operator SDL_FPoint() const { return SDL_FPoint(x, y); }

        // Arithmetic operations
        inline Vector2f &operator=(const Vector2f &other)
        {
            if (this == &other)
                return *this;

            x = other.x;
            y = other.y;
            return *this;
        }

        // Overloaded operators
        inline Vector2f &operator=(const SDL_FPoint &other)
        {
            x = other.x;
            y = other.y;
            return *this;
        }

        inline Vector2f &operator+=(const Vector2f &rhs)
        {
            this->x += rhs.x;
            this->y += rhs.y;
            return *this;
        }

        inline Vector2f &operator-=(const Vector2f &rhs)
        {
            this->x -= rhs.x;
            this->y -= rhs.y;
            return *this;
        }

        friend Vector2f operator+(Vector2f lhs, const Vector2f &rhs)
        {
            lhs += rhs;
            return lhs;
        }

        friend Vector2f operator-(Vector2f lhs, const Vector2f &rhs)
        {
            lhs -= rhs;
            return lhs;
        }

        // Operations
        inline float Distance() const
        {
            return sqrtf(x * x + y * y);
        }

        /// @brief Returns a new copy of the vector, normlized. If vector was 0,0, returns 0,0.
        /// @return The normalized vector.
        inline Vector2f Normalized() const
        {
            auto d = Distance();
            if (d == 0)
            {
                return {0, 0};
            }
            return {x / d, y / d};
        }

        inline void Normalize()
        {
            *this = Normalized();
        }

        inline float Dot(const Vector2f &rhs) const
        {
            auto n1 = this->Normalized();
            auto n2 = rhs.Normalized();
            return (n1.x * n2.x + n1.y * n2.y);
        }

        static float Dot(Vector2f lhs, const Vector2f &rhs)
        {
            return lhs.Dot(rhs);
        }

        static const Vector2f Zero;
        static const Vector2f One;
        static const Vector2f Right;
        static const Vector2f Left;
        static const Vector2f Up;
        static const Vector2f Down;
    };

    inline const Vector2f Vector2f::Zero{0, 0};
    inline const Vector2f Vector2f::One{1, 1};
    inline const Vector2f Vector2f::Right{1, 0};
    inline const Vector2f Vector2f::Left{-1, 0};
    inline const Vector2f Vector2f::Up{0, 1};
    inline const Vector2f Vector2f::Down{0, -1};

    inline bool operator==(const Vector2f &lhs, const Vector2f &rhs)
    {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }

    inline bool operator!=(const Vector2f &lhs, const Vector2f &rhs)
    {
        return !(lhs == rhs);
    }
}

#endif