#pragma once

#include "Mat.hpp"
#include "Vec2.hpp"
#include "Vec3.hpp"

namespace lio
{
    template <typename T>
    inline Vec2<T> MatToVec2(const Mat<T>& m, size_t col = 0)
    {
        return Vec2<T>(m(0, col), m(1, col));
    }

    template <typename T>
    inline Vec3<T> MatToVec3(const Mat<T>& m, size_t col = 0)
    {
        return Vec3<T>(m(0, col), m(1, col), m(2, col));
    }

    template <typename T>
    inline Mat<T> Vec2ToMat(const Vec2<T>& v)
    {
        return Mat<T>({ { v.x }, { v.y } });
    }

    template <typename T>
    inline Mat<T> Vec3ToMat(const Vec3<T>& v)
    {
        return Mat<T>({ { v.x }, { v.y }, { v.z } });
    }
}