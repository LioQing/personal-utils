#pragma once

#include <cmath>
#include <vector>
#include <string>
#include <set>
#include <stdexcept>
#include <iomanip>

namespace lio
{
    namespace
    {
        template <typename T>
        std::invalid_argument MatNotSq(size_t row_count, size_t col_count)
        {
            throw std::invalid_argument(std::string("lio::Mat<") + typeid(T).name() + "> (row_count = " + 
                std::to_string(row_count) + ", col_count = " + std::to_string(col_count) + ") is not a square matrix.");
        }

        template <typename T, typename U>
        std::invalid_argument MatNotSameSize(size_t row_count1, size_t col_count1, size_t row_count2, size_t col_count2)
        {
            throw std::invalid_argument(std::string("lio::Mat<") + typeid(T).name() + "> (row_count = " + 
                std::to_string(row_count1) + ", col_count = " + std::to_string(col_count1) + ") and " + 
                std::string("lio::Mat<") + typeid(U).name() + "> (row_count = " + 
                std::to_string(row_count2) + ", col_count = " + std::to_string(col_count2) + ") do not have the same size.");
        }

        template <typename T, typename U>
        std::invalid_argument MatNotMatchRowCol(size_t row_count1, size_t col_count1, size_t row_count2, size_t col_count2)
        {
            throw std::invalid_argument(std::string("col_count of lio::Mat<") + typeid(T).name() + "> (row_count = " + 
                std::to_string(row_count1) + ", col_count = " + std::to_string(col_count1) + ") is not equal to row_count of " + 
                std::string("lio::Mat<") + typeid(U).name() + "> (row_count = " + 
                std::to_string(row_count2) + ", col_count = " + std::to_string(col_count2) + ").");
        }

        template <typename T>
        std::invalid_argument MatInvertZeroDeterminant(size_t row_count, size_t col_count)
        {
            throw std::invalid_argument(std::string("lio::Mat<") + typeid(T).name() + "> (row_count = " + 
                std::to_string(row_count) + ", col_count = " + std::to_string(col_count) + ") is not invertible (zero determinant).");
        }

        template <typename T>
        std::invalid_argument MatLeftInvertSize(size_t row_count, size_t col_count)
        {
            throw std::invalid_argument(std::string("lio::Mat<") + typeid(T).name() + "> (row_count = " + 
                std::to_string(row_count) + ", col_count = " + std::to_string(col_count) + ") does not have a left inverse (row_count < col_count).");
        }

        template <typename T>
        std::invalid_argument MatRightInvertSize(size_t row_count, size_t col_count)
        {
            throw std::invalid_argument(std::string("lio::Mat<") + typeid(T).name() + "> (row_count = " + 
                std::to_string(row_count) + ", col_count = " + std::to_string(col_count) + ") does not have a right inverse (row_count > col_count).");
        }
    }

    /**
     * @brief A dynamically sized matrix class
     * 
     * @tparam T Type for storing the values
     */
    template <typename T>
    struct Mat
    {
        std::vector<std::vector<T>> matrix;

        size_t row_count, col_count;

        /**
         * @brief Construct an empty Mat
         * 
         */
        Mat() = default;

        /**
         * @brief Construct a new Mat from another Mat
         * 
         */
        Mat(const Mat&) = default;

        /**
         * @brief Construct a new Mat from an initializer list
         * 
         * @param m 
         */
        Mat(const std::initializer_list<std::initializer_list<T>>& m)
        {
            *this = Mat(m.begin(), m.end());
        }

        /**
         * @brief Construct a new Mat from iterators of a 2D container
         * 
         * @param begin The begin iterator
         * @param end The end iterator
         */
        template <typename Iter>
        Mat(Iter begin, Iter end)
            : row_count(0), col_count(0)
        {
            row_count = std::distance(begin, end);
            for (auto it = begin; it != end; ++it)
            {
                if (it->size() > col_count)
                    col_count = it->size();
            }
            
            matrix = std::vector<std::vector<T>>(row_count, std::vector<T>(col_count));

            size_t i = 0;
            for (auto it_i = begin; it_i != end; ++i, ++it_i)
            {
                size_t j = 0;
                for (auto it_j = std::begin(*it_i); it_j != std::end(*it_i); ++j, ++it_j)
                {
                    At(i, j) = *it_j;
                }
            }
        }

        /**
         * @brief Construct a new Mat given the row and column counts
         * 
         * @param row_count Row count
         * @param col_count Column count
         */
        Mat(size_t row_count, size_t col_count) 
            : row_count(row_count), col_count(col_count), matrix(row_count, std::vector<T>(col_count))
        {
        }

        /**
         * @brief Construct a new Mat given the row and column counts and filled with the given value
         * 
         * @param row_count Row count
         * @param col_count Columne count
         * @param val The value to fill with
         */
        Mat(size_t row_count, size_t col_count, T val) 
            : row_count(row_count), col_count(col_count), matrix(row_count, std::vector<T>(col_count, val))
        {
        }

        /**
         * @brief Get the row count of this
         * 
         * @return size_t The row count
         */
        size_t RowCount() const
        {
            return row_count;
        }

        /**
         * @brief Get the column count of this
         * 
         * @return size_t The column count
         */
        size_t ColCount() const
        {
            return col_count;
        }

        /**
         * @brief Cast to Mat\<U\>
         * 
         * @tparam U The type to cast to
         * @return Mat\<U\> The casted Mat
         */
        template <typename U>
        operator Mat<U>() const
        {
            return Cast<U>();
        }

        /**
         * @brief Cast to Mat\<U\>
         * 
         * @tparam U The type to cast to
         * @return Mat\<U\> The casted Mat
         */
        template <typename U>
        Mat<U> Cast() const
        {
            Mat<U> m_ret(row_count, col_count);

            for (size_t i = 0; i < row_count; ++i)
            for (size_t j = 0; j < col_count; ++j)
            {
                m_ret(i, j) = At(i, j);
            }

            return m_ret;
        }

        /**
         * @brief Access the element at row and col
         * 
         * @param row The row
         * @param col The col
         * @return T& The element at row and col
         */
        T& operator()(size_t row, size_t col)
        {
            return matrix.at(row).at(col);
        }

        /**
         * @brief Access the element at row and col
         * 
         * @param row The row
         * @param col The col
         * @return T& The element at row and col
         */
        T& At(size_t row, size_t col)
        {
            return matrix.at(row).at(col);
        }

        /**
         * @brief Get the element at row and col
         * 
         * @param row The row
         * @param col The col
         * @return T The element at row and col
         */
        T operator()(size_t row, size_t col) const
        {
            return matrix.at(row).at(col);
        }

        /**
         * @brief Get the element at row and col
         * 
         * @param row The row
         * @param col The col
         * @return T The element at row and col
         */
        T At(size_t row, size_t col) const
        {
            return matrix.at(row).at(col);
        }

        /**
         * @brief Fill this with val
         * 
         * @param val The value to fill with
         */
        void Fill(T val)
        {
            for (auto& i : matrix)
            for (auto& j : i)
            {
                j = val;
            }
        }

        /**
         * @brief Get the Mat with row(s) and column(s) removed from m
         * 
         * @param m The Mat to be used
         * @param row The row(s)
         * @param col The column(s)
         * @return Mat The Mat with the removed row(s) and column(s)
         */
        template <typename S>
        static Mat Removed(const Mat& m, S row, S col)
        {
            return m.Removed(row, col);
        }

        /**
         * @brief Get the Mat with a row and a column removed from this
         * 
         * @param row The row
         * @param col The column
         * @return Mat The Mat with the removed row and column
         */
        Mat Removed(size_t row, size_t col) const
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

        /**
         * @brief Get the Mat with row(s) and a column(s) removed from this
         * 
         * @param row The row(s)
         * @param col The column(s)
         * @return Mat The Mat with the removed row(s) and column(s)
         */
        Mat Removed(const std::set<size_t>& rows, const std::set<size_t>& cols) const
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

        /**
         * @brief Get the Mat of m_dest with part replaced by m_src
         * 
         * @param m_dest The destination Mat
         * @param m_src The source Mat
         * @param row The row of the top left element of the part to be replaced
         * @param col The column of the top left element of the part to be replaced
         * @return Mat The replaced Mat
         */
        template <typename S = T>
        static Mat Replaced(const Mat& m_dest, const Mat<S>& m_src, size_t row = 0, size_t col = 0)
        {
            return m_dest.Replaced(m_src, row, col);
        }

        /**
         * @brief Get the Mat of this with part replaced by m
         * 
         * @param m The source Mat
         * @param row The row of the top left element of the part to be replaced
         * @param col The column of the top left element of the part to be replaced
         * @return Mat The replaced Mat
         */
        template <typename U = T>
        Mat Replaced(const Mat<U>& m, size_t row = 0, size_t col = 0) const
        {
            Mat m_ret(*this);
            return m_ret.Replace(row, col, m);
        }

        /**
         * @brief Replace part of this with m
         * 
         * @param m The source Mat
         * @param row The row of the top left element of the part to be replaced
         * @param col The column of the top left element of the part to be replaced
         * @return Mat This Mat
         */
        template <typename U = T>
        Mat& Replace(const Mat<U>& m, size_t row = 0, size_t col = 0) &
        {
            for (size_t i = 0; i < m.row_count; ++i)
            for (size_t j = 0; j < m.col_count; ++j)
            {
                At(row + i, col + j) = m(i, j);
            }

            return *this;
        }

        /**
         * @brief Get the inverse of m
         * 
         * @param m The Mat to be used
         * @return Mat The inverse
         */
        static Mat Inverse(const Mat& m)
        {
            return m.Inverse();
        }

        /**
         * @brief Get the inverse of this
         * 
         * @param determinant_check Check the determinant, throw on failed. (Default is false)
         * @return Mat The inverse
         */
        Mat Inverse(bool determinant_check = false) const
        {
            if (row_count != col_count)
                throw MatNotSq<T>(row_count, col_count);
            
            if (determinant_check && Determinant() == 0)
                throw MatInvertZeroDeterminant<T>(row_count, col_count);
            
            Mat m_ret(row_count, col_count * 2);
            m_ret.Replace(*this);
            m_ret.Replace(Identity(), 0, col_count);

            m_ret.GaussianEliminate();

            std::set<size_t> rm_col;
            for (size_t i = 0; i < col_count; ++i)
            {
                rm_col.insert(i);
            }

            return m_ret.Removed({}, rm_col);
        }

        /**
         * @brief Invert this
         * 
         * @return Mat& This Mat
         */
        Mat& Invert() &
        {
            *this = Inverse();
            return *this;
        }

        /**
         * @brief Get the left inverse of m
         * 
         * @param m The Mat to used
         * @return Mat The left inverse
         */
        static Mat LeftInverse(const Mat& m)
        {
            return m.LeftInverse();
        }

        /**
         * @brief Get the left inverse of this
         * 
         * @return Mat The left inverse
         */
        Mat LeftInverse() const
        {
            if (row_count < col_count)
                throw MatLeftInvertSize<T>(row_count, col_count);
            
            return Inverse(Transposed() * *this) * Transposed();
        }

        /**
         * @brief Left invert this
         * 
         * @return Mat& This Mat
         */
        Mat& LeftInvert() &
        {
            *this = LeftInverse();
            return *this;
        }

        /**
         * @brief Get the right inverse of m
         * 
         * @param m The Mat to used
         * @return Mat The right inverse
         */
        static Mat RightInverse(const Mat& m)
        {
            return m.RightInverse();
        }

        /**
         * @brief Get the right inverse of this
         * 
         * @return Mat The right inverse
         */
        Mat RightInverse() const
        {
            if (row_count > col_count)
                throw MatRightInvertSize<T>(row_count, col_count);

            return Transposed() * Inverse(*this * Transposed());
        }

        /**
         * @brief Left invert this
         * 
         * @return Mat& This Mat
         */
        Mat& RightInvert() &
        {
            *this = RightInverse();
            return *this;
        }

        /**
         * @brief Get the transposed Mat of m
         * 
         * @param m The Mat to be used
         * @return Mat The transposed Mat
         */
        static Mat Transposed(const Mat& m)
        {
            return m.Transposed();
        }

        /**
         * @brief Get the transposed Mat of this
         * 
         * @return Mat The transposed Mat
         */
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

        /**
         * @brief Transpose this
         * 
         * @return Mat& This Mat
         */
        Mat& Transpose() &
        {
            *this = Transposed();
            return *this;
        }

        /**
         * @brief Get the Hadamard (element-wise) multiplication product of m1 and m2
         * 
         * @param m1 The first Mat
         * @param m2 The second Mat
         * @return Mat The Hadamard multiplication product
         */
        template <typename U = T>
        static auto HadamardMultiplication(const Mat& m1, const Mat<U>& m2)
        {
            return m1.HadamardMultiplication(m2);
        }

        /**
         * @brief Get the Hadamard (element-wise) multiplication product of this and m
         * 
         * @param m The Mat to be used
         * @return Mat The Hadamard multiplication product
         */
        template <typename U = T>
        auto HadamardMultiplication(const Mat<U>& m) const
        {
            if (row_count != m.row_count || col_count != m.col_count)
                throw MatNotSameSize<T, U>(row_count, col_count, m.row_count, m.col_count);

            Mat<decltype(std::declval<T&>() * std::declval<T&>())> m_ret(row_count, col_count);

            for (size_t i = 0; i < row_count; ++i)
            for (size_t j = 0; j < col_count; ++j)
            {
                m_ret(i, j) = At(i, j) * m(i, j);
            }

            return m_ret;
        }

        /**
         * @brief Get the Hadamard (element-wise) division quotient of m1 and m2
         * 
         * @param m1 The first Mat
         * @param m2 The second Mat
         * @return Mat The Hadamard division quotient
         */
        template <typename U = T>
        static auto HadamardDivision(const Mat& m1, const Mat<U>& m2)
        {
            return m1.HadamardDivision(m2);
        }

        /**
         * @brief Get the Hadamard (element-wise) division quotient of this and m
         * 
         * @param m The Mat to be used
         * @return Mat The Hadamard division quotient
         */
        template <typename U = T>
        auto HadamardDivision(const Mat<U>& m) const
        {
            if (row_count != m.row_count || col_count != m.col_count)
                throw MatNotSameSize<T, U>(row_count, col_count, m.row_count, m.col_count);

            Mat<decltype(std::declval<T&>() / std::declval<T&>())> m_ret(row_count, col_count);

            for (size_t i = 0; i < row_count; ++i)
            for (size_t j = 0; j < col_count; ++j)
            {
                m_ret(i, j) = At(i, j) / m(i, j);
            }

            return m_ret;
        }

        /**
         * @brief Get the Hadamard (element-wise) modulo remainder of m1 and m2
         * 
         * @param m1 The first Mat
         * @param m2 The second Mat
         * @return Mat The Hadamard modulo remainder
         */
        template <typename U = T>
        static auto HadamardModulo(const Mat& m1, const Mat<U>& m2)
        {
            return m1.HadamardModulo(m2);
        }

        /**
         * @brief Get the Hadamard (element-wise) modulo remainder of this and m
         * 
         * @param m The Mat to be used
         * @return Mat The Hadamard modulo remainder
         */
        template <typename U = T>
        auto HadamardModulo(const Mat<U>& m) const
        {
            if (row_count != m.row_count || col_count != m.col_count)
                throw MatNotSameSize<T, U>(row_count, col_count, m.row_count, m.col_count);

            Mat<decltype(std::fmod(std::declval<T&>(), std::declval<T&>()))> m_ret(row_count, col_count);

            for (size_t i = 0; i < row_count; ++i)
            for (size_t j = 0; j < col_count; ++j)
            {
                m_ret(i, j) = std::fmod(At(i, j), m(i, j));
            }

            return m_ret;
        }

        // Row Operations

        /**
         * @brief Get the Mat with m with row r1 swapped with row r2
         * 
         * @param m The Mat to be used
         * @param r1 The first row
         * @param r2 The second row
         * @return Mat The swapped Mat
         */
        static Mat RowsSwapped(const Mat& m, size_t r1, size_t r2)
        {
            return m.RowsSwapped(r1, r2);
        }

        /**
         * @brief Get the Mat with this with row r1 swapped with row r2
         * 
         * @param r1 The first row
         * @param r2 The second row
         * @return Mat The swapped Mat
         */
        Mat RowsSwapped(size_t r1, size_t r2) const
        {
            Mat m_ret(*this);
            return m_ret.SwapRows(r1, r2);
        }

        /**
         * @brief Swap the row r1 with row r2
         * 
         * @param r1 The first row
         * @param r2 The second row
         * @return Mat& This Mat
         */
        Mat& SwapRows(size_t r1, size_t r2) &
        {
            matrix.at(r1).swap(matrix.at(r2));
            return *this;
        }

        /**
         * @brief Get the Mat with m with elements in row multiplied by s
         * 
         * @param m The Mat to be used
         * @param row The row
         * @param s The factor to be multiplied in
         * @return Mat The multiplied Mat
         */
        template <typename U>
        static Mat RowMultiplied(const Mat& m, size_t row, U s)
        {
            return m.RowMultiplied(row, s);
        }

        /**
         * @brief Get the Mat with this with elements in row multiplied by s
         * 
         * @param row The row
         * @param s The factor to be multiplied in
         * @return Mat The multiplied Mat
         */
        template <typename U>
        Mat RowMultiplied(size_t row, U s) const
        {
            Mat m_ret(*this);
            return m_ret.MultiplyRow(row, s);
        }

        /**
         * @brief Multiply the elements in row by s
         * 
         * @param row The row
         * @param s The factor to be multiplied in
         * @return Mat& This Mat
         */
        template <typename U>
        Mat& MultiplyRow(size_t row, U s) &
        {
            for (auto& j : matrix.at(row))
            {
                j *= s;
            }

            return *this;
        }

        /**
         * @brief Get the Mat with m with elements in row r_dest added with elements in row r_src multiplied with s
         * 
         * @param m The Mat to be used
         * @param r_dest The destination row
         * @param r_src The source row
         * @param s The factor to be multiplied in
         * @return Mat The added Mat
         */
        template <typename U>
        static Mat RowAdded(const Mat& m, size_t r_dest, size_t r_src, U s)
        {
            return m.RowAdded(r_dest, r_src, s);
        }

        /**
         * @brief Get the Mat with this with elements in row r_dest added with elements in row r_src multiplied with s
         * 
         * @param r_dest The destination row
         * @param r_src The source row
         * @param s The factor to be multiplied in
         * @return Mat The added Mat
         */
        template <typename U>
        Mat RowAdded(size_t r_dest, size_t r_src, U s) const
        {
            Mat m_ret(*this);
            return m_ret.AddRow(r_dest, r_src, s);
        }

        /**
         * @brief Add elements in row r_src to elements in row r_dest and multiplied with s
         * 
         * @param r_dest The destination row
         * @param r_src The source row
         * @param s The factor to be multiplied in
         * @return Mat& This Mat
         */
        template <typename U>
        Mat& AddRow(size_t r_dest, size_t r_src, U s) &
        {
            for (size_t i = 0; i < col_count; ++i)
            {
                At(r_dest, i) += At(r_src, i) * s;
            }

            return *this;
        }

        // end of row operations section

        /**
         * @brief Get the Mat of m with Gaussian elimination performed
         * 
         * @param m The Mat to be used
         * @return Mat The Gaussian eliminated Mat
         */
        static Mat GaussianEliminated(const Mat& m)
        {
            return m.GaussianEliminated();
        }

        /**
         * @brief Get the Mat of this with Gaussian elimination performed
         * 
         * @return Mat The Gaussian eliminated Mat
         */
        Mat GaussianEliminated() const
        {
            Mat m_ret(*this);
            return m_ret.GaussianElimination();
        }

        /**
         * @brief Perform Gaussian elimination on this
         * 
         * @return Mat& This Mat
         */
        Mat& GaussianEliminate() &
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

        /**
         * @brief Get the determinant of m
         * 
         * @param m The Mat to get the determinant
         * @return auto The determinant
         */
        static auto Determinant(const Mat& m)
        {
            return m.Determinant();
        }

        /**
         * @brief Get the determinant of this
         * 
         * @return auto The determinant
         */
        template <typename U = T>
        U Determinant() const
        {
            if (row_count != col_count)
                throw MatNotSq<T>(row_count, col_count);
            
            U det = 0;

            for (size_t j = 0; j < col_count; ++j)
            {
                U pos_diag = 1;
                U neg_diag = 1;

                for (size_t i = 0; i < row_count; ++i)
                {
                    pos_diag *= At(i, (j + i) % col_count);
                    neg_diag *= At(row_count - i - 1, (j + i) % col_count);
                }

                det += pos_diag - neg_diag;
            }

            return det;
        }

        /**
         * @brief Get an identity matrix of given size
         * 
         * @param size The size
         * @return Mat The identity matrix
         */
        static Mat Identity(size_t size)
        {
            Mat m_ret(size, size);

            for (size_t i = 0; i < size; ++i)
            {
                m_ret(i, i) = 1;
            }

            return m_ret;
        }

        /**
         * @brief Get an identity matrix same size as this
         * 
         * @return Mat The identity matrix
         */
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
        if (m1.col_count != m2.row_count)
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

    template <typename T, typename U>
    Mat<T>& operator+=(Mat<T>& m1, const Mat<U>& m2)
    {
        return m1 = m1 + m2;
    }
    template <typename T, typename U>
    Mat<T>& operator-=(Mat<T>& m1, const Mat<U>& m2)
    {
        return m1 = m1 - m2;
    }
    template <typename T, typename U>
    Mat<T>& operator*=(Mat<T>& m1, const Mat<U>& m2)
    {
        return m1 = m1 * m2;
    }

    template <typename T, typename U>
    auto operator*(U s, const Mat<T>& m)
    {
        Mat<decltype(s * std::declval<T&>())> m_ret(m);

        for (auto& i : m_ret.matrix)
        for (auto& j : i)
            j *= s;

        return m_ret;
    }

    template <typename T, typename U>
    auto operator*(const Mat<T>& m, U s)
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
    Mat<T>& operator*=(Mat<T>& m, U s)
    {
        return m = m * s;
    }
    template <typename T, typename U>
    Mat<T>& operator/=(Mat<T>& m, U s)
    {
        return m = m / s;
    }
    template <typename T, typename U>
    Mat<T>& operator%=(Mat<T>& m, U s)
    {
        return m = m % s;
    }
    
    template <typename T>
    std::ostream& operator<<(std::ostream& os, const Mat<T>& m)
    {
        std::ios_base::fmtflags f(std::cout.flags());
        std::vector<int> col_size(m.col_count, 0);

        for (size_t j = 0; j < m.col_count; ++j)
        for (size_t i = 0; i < m.row_count; ++i)
        {
            std::stringstream ss;
            ss << m(i, j);
            if (ss.str().size() > col_size[j])
                col_size[j] = ss.str().size();
        }

        for (size_t i = 0; i < m.row_count; ++i)
        {
            os << (i == 0 ? "(" : " ");
            for (size_t j = 0; j < m.col_count; ++j)
            {
                os << std::setw(col_size[j]) << m(i, j);
                if (j != m.col_count - 1)
                    os << " ";
            }
            os << (i == m.row_count - 1 ? ")" : "\n");
        }

        os.flags(f);

        return os;
    }

    typedef Mat<int> Mati;
    typedef Mat<float> Matf;
    typedef Mat<double> Matd;
}