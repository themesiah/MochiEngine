#ifndef HDEF_TYPES
#define HDEF_TYPES

#include <SDL3/SDL.h>
#include <math.h>
#include <string>
#include <format>

#include "../Exception.hpp"

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

        inline Vector2f &operator*=(const Vector2f &rhs)
        {
            this->x *= rhs.x;
            this->y *= rhs.y;
            return *this;
        }

        inline Vector2f &operator*=(const float &rhs)
        {
            this->x *= rhs;
            this->y *= rhs;
            return *this;
        }

        inline Vector2f &operator/=(const Vector2f &rhs)
        {
            this->x /= rhs.x;
            this->y /= rhs.y;
            return *this;
        }

        inline Vector2f &operator/=(const float &rhs)
        {
            if (rhs == 0.0f)
                throw EngineError("Cannot divide a vector by 0");
            this->x /= rhs;
            this->y /= rhs;
            return *this;
        }

        inline bool operator==(const Vector2f &rhs)
        {
            return this->x == rhs.x && this->y == rhs.y;
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

        friend Vector2f operator*(Vector2f lhs, const Vector2f &rhs)
        {
            lhs *= rhs;
            return lhs;
        }

        friend Vector2f operator*(Vector2f lhs, const float &rhs)
        {
            lhs *= rhs;
            return lhs;
        }

        friend Vector2f operator/(Vector2f lhs, const Vector2f &rhs)
        {
            lhs /= rhs;
            return lhs;
        }

        friend Vector2f operator/(Vector2f lhs, const float &rhs)
        {
            if (rhs == 0.0f)
                throw EngineError("Cannot divide a vector by 0");
            lhs /= rhs;
            return lhs;
        }

        inline std::string ToString() const
        {
            return std::format("{}x, {}y", x, y);
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
            return (x * rhs.x + y * rhs.y);
        }

        static float Dot(Vector2f lhs, const Vector2f &rhs)
        {
            return lhs.Dot(rhs);
        }

        inline float CosineProximity(const Vector2f &rhs) const
        {
            auto n1 = this->Normalized();
            auto n2 = rhs.Normalized();
            return (n1.x * n2.x + n1.y * n2.y);
        }

        static float CosineProximity(Vector2f lhs, const Vector2f &rhs)
        {
            return lhs.CosineProximity(rhs);
        }

        static Vector2f MoveTowards(const Vector2f source, const Vector2f target, const float &maxMagnitude)
        {
            if ((source - target).Distance() <= maxMagnitude)
                return target;

            auto dir = (target - source).Normalized();
            return source + dir * maxMagnitude;
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

    struct Rectf
    {
        float x, y, w, h;

        Rectf(float _x = 0, float _y = 0, float _w = 0, float _h = 0) : x(_x), y(_y), w(_w), h(_h) {}
        Rectf(const Rectf &r) : x(r.x), y(r.y), w(r.w), h(r.h) {}
        Rectf(Rectf &&r) : x(r.x), y(r.y), w(r.w), h(r.h) {}
        Rectf(Vector2f pos, Vector2f size) : x(pos.x), y(pos.y), w(size.x), h(size.y) {}

        // SDL_FPoint conversion
        Rectf(const SDL_FRect &r) : x(r.x), y(r.y), w(r.w), h(r.h) {}
        inline operator SDL_FRect() const { return SDL_FRect(x, y, w, h); }

        // Arithmetic operations
        inline Rectf &operator=(const Rectf &other)
        {
            if (this == &other)
                return *this;

            x = other.x;
            y = other.y;
            w = other.w;
            h = other.h;
            return *this;
        }

        // Overloaded operators
        inline Rectf &operator=(const SDL_FRect &other)
        {
            x = other.x;
            y = other.y;
            w = other.w;
            h = other.h;
            return *this;
        }

        /// @brief Moves the position of the rect adding a vector
        /// @param rhs The movement vector
        /// @return Modified rect
        inline Rectf &operator+=(const Vector2f &rhs)
        {
            this->x += rhs.x;
            this->y += rhs.y;
            return *this;
        }

        /// @brief Moves the position of the rect subtracting a vector
        /// @param rhs The movement vector
        /// @return Modified rect
        inline Rectf &operator-=(const Vector2f &rhs)
        {
            this->x -= rhs.x;
            this->y -= rhs.y;
            return *this;
        }

        inline bool operator==(const Rectf &rhs)
        {
            return this->x == rhs.x && this->y == rhs.y && this->w == rhs.w && this->h == rhs.h;
        }

        /// @brief Moves the position of the rect adding a vector
        /// @param rhs The movement vector
        /// @return Resulting rect
        friend Rectf operator+(Rectf lhs, const Vector2f &rhs)
        {
            lhs += rhs;
            return lhs;
        }

        /// @brief Moves the position of the rect subtracting a vector
        /// @param rhs The movement vector
        /// @return Resulting rect
        friend Rectf operator-(Rectf lhs, const Vector2f &rhs)
        {
            lhs -= rhs;
            return lhs;
        }

        friend bool operator==(const Rectf &lhs, const Rectf &rhs)
        {
            return lhs.x == rhs.x && lhs.y == rhs.y && lhs.w == rhs.w && lhs.h == rhs.h;
        }

        // Utilities
        inline Vector2f GetPosition() const
        {
            return {x, y};
        }

        inline void SetPosition(const Vector2f &pos)
        {
            x = pos.x;
            y = pos.y;
        }

        inline void SetPosition(const float &newX, const float &newY)
        {
            x = newX;
            y = newY;
        }

        inline Vector2f GetSize() const
        {
            return {w, h};
        }

        inline void SetSize(const Vector2f &size)
        {
            w = size.x;
            h = size.y;
        }

        inline void SetSize(const float &newW, const float &newH)
        {
            w = newW;
            h = newH;
        }

        inline void Scale(const float &scale)
        {
            w *= scale;
            h *= scale;
        }

        inline bool IsTextureValid()
        {
            return w > 0 && h > 0;
        }
    };

    struct Color
    {
        unsigned int r;
        unsigned int g;
        unsigned int b;
        unsigned int a;

        Color() : r(255), g(255), b(255), a(255) {}
        Color(unsigned int _r, unsigned int _g, unsigned int _b, unsigned int _a) : r(_r), g(_g), b(_b), a(_a) {}
        Color(const Color &c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
        Color(Color &&c) : r(c.r), g(c.g), b(c.b), a(c.a) {}

        // SDL_Color conversion
        Color(const SDL_Color &c) : r(c.r), g(c.g), b(c.b), a(c.a) {}
        inline operator SDL_Color() const { return SDL_Color(r, g, b, a); }

        // Overloaded operators
        inline Color &operator=(const Color &other)
        {
            if (this == &other)
                return *this;

            r = other.r;
            g = other.g;
            b = other.b;
            a = other.a;
            return *this;
        }

        inline Color &operator=(const SDL_Color &other)
        {
            r = other.r;
            g = other.g;
            b = other.b;
            a = other.a;
            return *this;
        }
    };
}

#endif