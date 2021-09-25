#pragma once

#include <iostream>
#include <cmath>

namespace lio
{
    template <typename T>
    struct Vec3
    {
        T x, y, z;

        Vec3(T x = 0.0, T y = 0.0, T z = 0.0) : x(x), y(y), z(z) {}

        template <typename U>
        inline operator Vec3<U>() const
        {
            return Vec3<U>(x, y, z);
        }
        template <typename U>
        inline Vec3<U> Cast() const
        {
            return Vec3<U>(x, y, z);
        }

        Vec3& Set(T x, T y, T z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            return *this;
        }

        inline static Vec3 Normalized(const Vec3& v) 
        {
            return v.Normalized();
        }
        Vec3 Normalized() const
        {
            if (Magnitude() == 0) return *this;
            return Vec3(*this / Magnitude());
        }
        Vec3& Normalize()
        {
            if (Magnitude() == 0) return *this;
            *this *= (1.0 / Magnitude());
            return *this;
        }

        template <typename U = T>
        inline static auto Distance(const Vec3& v1, const Vec3<U>& v2)
        {
            return v1.Distance(v2);
        }
        template <typename U = T>
        auto Distance(const Vec3<U>& v) const
        {
            Vec3<decltype(std::declval<U&>() - std::declval<T&>())> d(v.x - x, v.y - y, v.z - z);
            return d.Magnitude();
        }

        inline static auto SqrMagnitude(const Vec3& v)
        {
            return v.SqrMagnitude();
        }
        auto SqrMagnitude() const
        {
            return x * x + y * y + z * z;
        }
        
        inline static auto Magnitude(const Vec3& v)
        {
            return v.Magnitude();
        }
        inline auto Magnitude() const
        {
            return std::hypot(x, y, z);
        }

        inline static Vec3 Abs(const Vec3& v)
        {
            return v.Abs();
        }
        inline Vec3 Abs() const
        {
            return Vec3(x < 0 ? x * -1 : x, y < 0 ? y * -1 : y, z < 0 ? z * -1 : z);
        }

        template <typename U = T>
        static inline auto Dot(const Vec3& v1, const Vec3<U>& v2)
        {
            return v1.Dot(v2);
        } 
        template <typename U = T>
        inline auto Dot(const Vec3<U>& v) const
        {
            return x * v.x + y * v.y + z * v.z;
        }
        template <typename U = T>
        static inline auto Cross(const Vec3& v1, const Vec3<U>& v2)
        {
            return v1.Cross(v2);
        } 
        template <typename U = T>
        inline auto Cross(const Vec3<U>& v) const
        {
            return Vec3<decltype(std::declval<T&>() * std::declval<U&>())>
            (
                (y * v.z) - (z * v.y),
                (z * v.x) - (x * v.z),
                (x * v.y) - (y * v.x)
            );
        }

        template <typename U = T>
        static inline auto HadamardMultiplication(const Vec3& v1, const Vec3<U>& v2)
        {
            return v1.HadamardMultiplication(v2);
        }
        template <typename U = T>
        inline auto HadamardMultiplication(const Vec3<U>& v) const
        {
            return Vec3<decltype(std::declval<T&>() * std::declval<U&>())>(x * v.x, y * v.y, z * v.z);
        }

        template <typename U = T>
        static inline auto HadamardDivision(const Vec3& v1, const Vec3<U>& v2)
        {
            return v1.HadamardDivision(v2);
        }
        template <typename U = T>
        inline auto HadamardDivision(const Vec3<U>& v) const
        {
            return Vec3<decltype(std::declval<T&>() / std::declval<U&>())>(x / v.x, y / v.y, z / v.z);
        }

        template <typename U = T>
        static inline auto HadamardModulo(const Vec3& v1, const Vec3<U>& v2)
        {
            return v1.HadamardModulo(v2);
        }
        template <typename U = T>
        inline auto HadamardModulo(const Vec3<U>& v) const
        {
            return Vec3<decltype(std::fmod(std::declval<T&>(), std::declval<U&>()))>(std::fmod(x, v.x), std::fmod(y, v.y), std::fmod(z, v.z));
        }

        template <typename U = T, typename S = T>
        static inline auto CollinearEx(const Vec3& v1, const Vec3<U>& v2, const Vec3<S>& v3)
        {
            return v1.CollinearEx(v2, v3);
        }
        template <typename U = T, typename S = T>
        inline auto CollinearEx(const Vec3<U>& v2, const Vec3<S>& v3) const
        {
            return v3.Cross(v2 - *this);
        }
        template <typename U = T, typename S = T>
        static inline bool Collinear(const Vec3& v1, const Vec3<U>& v2, const Vec3<S>& v3)
        {
            return v1.Collinear(v2, v3);
        }
        template <typename U = T, typename S = T>
        inline bool Collinear(const Vec3<U>& v2, const Vec3<S>& v3) const
        {
            return v3.Cross(v2 - *this).SqrMagnitude() == 0;
        }
    };

    template <typename T, typename U>
    inline auto operator+(const Vec3<T>& v1, const Vec3<U>& v2)
    {
        return Vec3<decltype(std::declval<T&>() + std::declval<U&>())>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    }
    template <typename T, typename U>
    inline auto operator-(const Vec3<T>& v1, const Vec3<U>& v2)
    {
        return Vec3<decltype(std::declval<T&>() - std::declval<U&>())>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    }

    template <typename T, typename U>
    inline Vec3<T>& operator+=(Vec3<T>& v1, const Vec3<U>& v2)
    {
        return v1 = v1 + v2;
    }
    template <typename T, typename U>
    inline Vec3<T>& operator-=(Vec3<T>& v1, const Vec3<U>& v2)
    {
        return v1 = v1 - v2;
    }

    template <typename T, typename U>
    inline auto operator*(U s, const Vec3<T>& v)
    {
        return Vec3<decltype(s * std::declval<T&>())>(v.x * s, v.y * s, v.z * s);
    }
    template <typename T, typename U>
    inline auto operator/(U s, const Vec3<T>& v)
    {
        return Vec3<decltype(s / std::declval<T&>())>(s / v.x, s / v.y, s / v.z);
    }
    template <typename T, typename U>
    inline auto operator%(U s, const Vec3<T>& v)
    {
        return Vec3<decltype(std::fmod(std::declval<T&>(), s))>(std::fmod(s, v.x), std::fmod(s, v.y), std::fmod(s, v.z));
    }

    template <typename T, typename U>
    inline auto operator*(const Vec3<T>& v, U s)
    {
        return Vec3<decltype(s * std::declval<T&>())>(v.x * s, v.y * s, v.z * s);
    }
    template <typename T, typename U>
    inline auto operator/(const Vec3<T>& v, U s)
    {
        return Vec3<decltype(s / std::declval<T&>())>(v.x / s, v.y / s, v.z / s);
    }
    template <typename T, typename U>
    inline auto operator%(const Vec3<T>& v, U s)
    {
        return Vec3<decltype(std::fmod(s, std::declval<T&>()))>(std::fmod(v.x, s), std::fmod(v.y, s), std::fmod(v.z, s));
    }

    template <typename T, typename U>
    inline Vec3<T>& operator*=(Vec3<T>& v, U s)
    {
        return v = v * s;
    }
    template <typename T, typename U>
    inline Vec3<T>& operator/=(Vec3<T>& v, U s)
    {
        return v = v / s;
    }
    template <typename T, typename U>
    inline Vec3<T>& operator%=(Vec3<T>& v, U s)
    {
        return v = v % s;
    }

    template <typename T>
    inline Vec3<T> operator-(const Vec3<T>& v)
    {
        return Vec3<T>(-v.x, -v.y, -v.z);
    }
    template <typename T>
    inline Vec3<T> operator+(const Vec3<T>& v)
    {
        return Vec3<T>(v.x, v.y, v.z);
    }

    template <typename T, typename U>
    inline bool operator==(const Vec3<T>& v1, const Vec3<U>& v2)
    {
        return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
    }
    template <typename T, typename U>
    inline bool operator!=(const Vec3<T>& v1, const Vec3<U>& v2)
    {
        return !(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Vec3<T>& v)
    {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }

    typedef Vec3<double> Vec3d;
    typedef Vec3<float> Vec3f;
    typedef Vec3<int> Vec3i;
}