#pragma once

namespace lio
{
    namespace Vec
    {
        template <typename V, typename... TArgs>
        inline auto Set(V& v, TArgs... args)
        {
            return v.Set(args...);
        }

        template <typename V>
        inline auto Normalized(const V& v)
        {
            return v.Normalized();
        }

        template <typename V1, typename V2>
        inline auto Distance(const V1& v1, const V2& v2)
        {
            return v1.Distance(v2);
        }

        template <typename V>
        inline auto SqrMagnitude(const V& v)
        {
            return v.SqrMagnitude();
        }

        template <typename V>
        inline auto Magnitude(const V& v)
        {
            return v.Magnitude();
        }

        template <typename V>
        inline auto Abs(const V& v)
        {
            return v.Abs();
        }

        template <typename V1, typename V2>
        inline auto Dot(const V1& v1, const V2& v2)
        {
            return v1.Dot(v2);
        }

        template <typename V1, typename V2>
        inline auto Cross(const V1& v1, const V2& v2)
        {
            return v1.Cross(v2);
        }

        template <typename V1, typename V2>
        inline auto HadamardMultiplication(const V1& v1, const V2& v2)
        {
            return v1.HadamardMultiplication(v2);
        }

        template <typename V1, typename V2>
        inline auto HadamardDivision(const V1& v1, const V2& v2)
        {
            return v1.HadamardDivision(v2);
        }

        template <typename V1, typename V2>
        inline auto HadamardModulo(const V1& v1, const V2& v2)
        {
            return v1.HadamardModulo(v2);
        }
    }
}