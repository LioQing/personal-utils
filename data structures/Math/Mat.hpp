#pragma once

#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <stdexcept>

namespace lio
{
    namespace
    {
        template <typename T>
        inline std::invalid_argument MatNotSq(size_t row_count, size_t col_count)
        {
            throw std::invalid_argument(std::string("lio::Mat<") + typeid(T).name() + "> (row_count = " + 
                std::to_string(row_count) + ", col_count = " + std::to_string(col_count) + ") is not a square matrix.");
        }

        template <typename T, typename U>
        inline std::invalid_argument MatNotSameSize(size_t row_count1, size_t col_count1, size_t row_count2, size_t col_count2)
        {
            throw std::invalid_argument(std::string("lio::Mat<") + typeid(T).name() + "> (row_count = " + 
                std::to_string(row_count1) + ", col_count = " + std::to_string(col_count1) + ") and " + 
                std::string("lio::Mat<") + typeid(U).name() + "> (row_count = " + 
                std::to_string(row_count2) + ", col_count = " + std::to_string(col_count2) + ") do not have the same size.");
        }

        template <typename T, typename U>
        inline std::invalid_argument MatNotMatchRowCol(size_t row_count1, size_t col_count1, size_t row_count2, size_t col_count2)
        {
            throw std::invalid_argument(std::string("row_count of lio::Mat<") + typeid(T).name() + "> (row_count = " + 
                std::to_string(row_count1) + ", col_count = " + std::to_string(col_count1) + ") is not equal to col_count of " + 
                std::string("lio::Mat<") + typeid(U).name() + "> (row_count = " + 
                std::to_string(row_count2) + ", col_count = " + std::to_string(col_count2) + ").");
        }
    }

    template <typename T>
    struct Mat
    {
        std::vector<std::vector<T>> matrix;

        size_t row_count, col_count;

        Mat(size_t row_count = 0, size_t col_count = 0) 
            : row_count(row_count), col_count(col_count), matrix(row_count, std::vector<T>(col_count))
        {
        }

        Mat(size_t row_count, size_t col_count, T val) 
            : row_count(row_count), col_count(col_count), matrix(row_count, std::vector<T>(col_count, val))
        {
        }

        inline T& operator()(size_t row, size_t col)
        {
            return matrix.at(row).at(col);
        }
        inline T& At(size_t row, size_t col)
        {
            return matrix.at(row).at(col);
        }
        inline T operator()(size_t row, size_t col) const
        {
            return matrix.at(row).at(col);
        }
        inline T At(size_t row, size_t col) const
        {
            return matrix.at(row).at(col);
        }

        void Fill(T val)
        {
            for (auto& i : matrix)
            for (auto& j : i)
            {
                j = val;
            }
        }

        // submatrix

        Mat Remove(size_t row, size_t col) const
        {
            Mat m_ret(row_count - 1, col_count - 1);

            for (size_t i = 0, row_skipped = 0, col_skipped = 0; i < row_count; ++i, col_skipped = 0)
            {
                if (i == row)
                {
                    ++row_skipped;
                    continue;
                }

                for (size_t j = 0; j < col_count; ++j)
                {
                    if (j == col)
                    {
                        ++col_skipped;
                        continue;
                    }

                    m_ret(i - row_skipped, j - col_skipped) = At(i, j);
                }
            }

            return m_ret;
        }
        Mat Remove(const std::set<size_t>& rows, const std::set<size_t>& cols) const
        {
            Mat m_ret(row_count - rows.size(), col_count - cols.size());

            for (size_t i = 0, row_skipped = 0, col_skipped = 0; i < row_count; ++i, col_skipped = 0)
            {
                if (rows.find(i) != rows.end())
                {
                    ++row_skipped;
                    continue;
                }

                for (size_t j = 0; j < col_count; ++j)
                {
                    if (cols.find(j) != cols.end())
                    {
                        ++col_skipped;
                        continue;
                    }

                    m_ret(i - row_skipped, j - col_skipped) = At(i, j);
                }
            }

            return m_ret;
        }

        static Mat Identity(size_t size)
        {
            Mat m_ret(size, size);

            for (size_t i = 0; i < size; ++i)
            {
                m_ret(i, i) = 1;
            }

            return m_ret;
        }
        Mat Identity() const
        {
            if (row_count != col_count)
                throw MatNotSq<T>(row_count, col_count);

            Mat m_ret(row_count, col_count);

            for (size_t i = 0; i < row_count; ++i)
            {
                m_ret(i, i) = 1;
            }

            return m_ret;
        }

        inline static Mat Transposed(const Mat& m)
        {
            return m.Transposed();
        }
        Mat Transposed() const
        {
            Mat m_ret(col_count, row_count);

            for (size_t i = 0; i < row_count; ++i)
            for (size_t j = 0; j < col_count; ++j)
            {
                m_ret(j, i) = At(i, j);
            }

            return m_ret;
        }
        inline Mat& Transpose()
        {
            *this = Transposed();
            return *this;
        }

        // Row Operations

        static inline Mat RowsSwapped(const Mat& m, size_t r1, size_t r2)
        {
            return m.RowsSwapped(r1, r2);
        }
        Mat RowsSwapped(size_t r1, size_t r2) const
        {
            Mat m_ret(*this);
            return m_ret.SwapRows(r1, r2);
        }
        Mat& SwapRows(size_t r1, size_t r2)
        {
            matrix.at(r1).swap(matrix.at(r2));
            return *this;
        }

        template <typename U>
        static inline Mat RowMultiplied(const Mat& m, size_t row, U s)
        {
            return m.RowMultiplied(row, s);
        }
        template <typename U>
        Mat RowMultiplied(size_t row, U s) const
        {
            Mat m_ret(*this);
            return m_ret.MultiplyRow(row, s);
        }
        template <typename U>
        Mat& MultiplyRow(size_t row, U s)
        {
            for (auto& j : matrix.at(row))
            {
                j *= s;
            }

            return *this;
        }

        template <typename U>
        static inline Mat RowAdded(const Mat& m, size_t r_dest, size_t r_src, U s)
        {
            return m.RowAdded(r_dest, r_src, s);
        }
        template <typename U>
        Mat RowAdded(size_t r_dest, size_t r_src, U s) const
        {
            Mat m_ret(*this);
            return m_ret.AddRow(r_dest, r_src, s);
        }
        template <typename U>
        Mat& AddRow(size_t r_dest, size_t r_src, U s)
        {
            for (size_t i = 0; i < col_count; ++i)
            {
                At(r_dest, i) += At(r_src, i) * s;
            }

            return *this;
        }

        static inline Mat GaussianEliminated(const Mat& m)
        {
            return m.GaussianEliminated();
        }
        Mat GaussianEliminated() const
        {
            Mat m_ret(*this);
            return m_ret.GaussianElimination();
        }
        Mat& GaussianEliminate()
        {
            for (size_t i = 0; i < std::min(row_count, col_count); ++i)
            {
                if (At(i, i) == 0)
                {
                    for (size_t j = 1; j < row_count - i; ++j)
                    {
                        if (At(i + j, i) != 0)
                        {
                            SwapRows(i, i + j);
                            break;
                        }
                    }
                }

                for (size_t j = 0; j < row_count; ++j)
                {
                    if (At(i, i) == 0 || j == i)
                    {
                        continue;
                    }
                    AddRow(j, i, -At(j, i) / At(i, i));
                }

                if (At(i, i) != 0)
                    MultiplyRow(i, 1 / At(i, i));
            }

            return *this;
        }
    };

    template <typename T, typename U>
    auto operator+(const Mat<T>& m1, const Mat<U>& m2)
    {
        if (m1.row_count != m2.row_count || m1.col_count != m2.col_count)
            throw MatNotSameSize<T, U>(m1.row_count, m1.col_count, m2.row_count, m2.col_count);

        Mat<decltype(std::declval<T&>() + std::declval<U&>())> m_ret(m1.row_count, m1.col_count);

        for (size_t i = 0; i < m1.row_count; ++i)
        for (size_t j = 0; j < m1.col_count; ++j)
        {
            m_ret(i, j) = m1(i, j) + m2(i, j);
        }

        return m_ret;
    }
    template <typename T, typename U>
    auto operator-(const Mat<T>& m1, const Mat<U>& m2)
    {
        if (m1.row_count != m2.row_count || m1.col_count != m2.col_count)
            throw MatNotSameSize<T, U>(m1.row_count, m1.col_count, m2.row_count, m2.col_count);

        Mat<decltype(std::declval<T&>() - std::declval<U&>())> m_ret(m1.row_count, m1.col_count);

        for (size_t i = 0; i < m1.row_count; ++i)
        for (size_t j = 0; j < m1.col_count; ++j)
        {
            m_ret(i, j) = m1(i, j) - m2(i, j);
        }

        return m_ret;
    }
    template <typename T, typename U>
    auto operator*(const Mat<T>& m1, const Mat<U>& m2)
    {
        if (m1.row_count != m2.col_count)
            throw MatNotMatchRowCol<T, U>(m1.row_count, m1.col_count, m2.row_count, m2.col_count);
        
        Mat<decltype(std::declval<T&>() - std::declval<U&>())> m_ret(m1.row_count, m2.col_count);

        for (size_t i = 0; i < m_ret.row_count; ++i)
        for (size_t j = 0; j < m_ret.col_count; ++j)
        for (size_t k = 0; k < m1.col_count; ++k)
        {
            m_ret(i, j) += m1(i, k) * m2(k, j);
        }

        return m_ret;
    }
    // todo: m1 / m2

    template <typename T, typename U>
    inline Mat<T>& operator+=(Mat<T>& m1, const Mat<U>& m2)
    {
        return m1 = m1 + m2;
    }
    template <typename T, typename U>
    inline Mat<T>& operator-=(Mat<T>& m1, const Mat<U>& m2)
    {
        return m1 = m1 - m2;
    }
    template <typename T, typename U>
    inline Mat<T>& operator*=(Mat<T>& m1, const Mat<U>& m2)
    {
        return m1 = m1 * m2;
    }
    // todo: m1 /= m2

    template <typename T, typename U>
    auto operator*(U s, const Mat<T>& m)
    {
        Mat<decltype(s * std::declval<T&>())> m_ret(m);

        for (auto& i : m_ret.matrix)
        for (auto& j : i)
            j *= s;

        return m_ret;
    }
    // todo: s / m

    template <typename T, typename U>
    inline auto operator*(const Mat<T>& m, U s)
    {
        return s * m;
    }
    template <typename T, typename U>
    auto operator/(const Mat<T>& m, U s)
    {
        Mat<decltype(std::declval<T&>() / s)> m_ret(m);

        for (auto& i : m_ret.matrix)
        for (auto& j : i)
            j /= s;

        return m_ret;
    }
    template <typename T, typename U>
    auto operator%(const Mat<T>& m, U s)
    {
        Mat<decltype(std::fmod(std::declval<T&>(), s))> m_ret(m);

        for (auto& i : m_ret.matrix)
        for (auto& j : i)
            j = std::fmod(j, s);

        return m_ret;
    }

    template <typename T, typename U>
    inline Mat<T>& operator*=(Mat<T>& m, U s)
    {
        return m = m * s;
    }
    template <typename T, typename U>
    inline Mat<T>& operator/=(Mat<T>& m, U s)
    {
        return m = m / s;
    }
    template <typename T, typename U>
    inline Mat<T>& operator%=(Mat<T>& m, U s)
    {
        return m = m % s;
    }
    
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Mat<T>& m)
    {
        for (size_t i = 0; i < m.row_count; ++i)
        {
            os << (i == 0 ? "((" : " (");
            for (size_t j = 0; j < m.col_count; ++j)
            {
                os << m(i, j);
                if (j != m.col_count - 1)
                    os << ", ";
            }
            os << (i == m.row_count - 1 ? "))" : "),\n");
        }

        return os;
    }

    typedef Mat<int> Mati;
    typedef Mat<float> Matf;
    typedef Mat<double> Matd;
}