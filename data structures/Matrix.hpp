#pragma once

#include <cstdint>
#include <vector>

namespace lio
{
	template <typename T>
	class Matrix
	{
	public:

		std::vector<std::vector<T>> matrix;

		uint32_t width, height;

		Matrix() : width(0), height(0) {}

		Matrix(std::vector<std::vector<T>> matrix) : matrix(matrix)
		{
			width = matrix.at(0).size();
			height = matrix.size();
		}

		Matrix(uint32_t width, uint32_t height, T val) : width(width), height(height)
		{
			matrix.resize(height);
			for (auto& h : matrix) h.resize(width);

			Fill(val);
		}

		Matrix& operator=(const Matrix& m)
		{
			height = m.height;
			width = m.width;

			matrix.resize(height);
			for (auto& h : matrix) h.resize(width);

			for (auto h = 0u; h < height; ++h)
			{
				for (auto w = 0u; w < width; ++w)
				{
					At(w, h) = m.At(w, h);
				}
			}

			return *this;
		}

		void SetSize(uint32_t w, uint32_t h, T val)
		{
			matrix.resize(h);
			for (auto& h : matrix) h.resize(w);

			for (auto tmp_h = 0u; tmp_h < h; ++tmp_h)
			{
				for (auto tmp_w = 0u; tmp_w < w; ++tmp_w)
				{
					if (tmp_h < height || tmp_w < width) continue;
					At(tmp_w, tmp_h) = val;
				}
			}

			width = w;
			height = h;
		}

		std::vector<T>& Row(uint32_t r)
		{
			return matrix.at(r);
		}
		// to not affect the performance, returning pointers is necessary
		std::vector<T*> Column(uint32_t c)
		{
			std::vector<T*> col;
			for (auto h = 0u; h < height; ++h) col.push_back(&matrix.at(h).at(c));
			return col;
		}

		T& At(uint32_t x, uint32_t y)
		{
			return matrix.at(y).at(x);
		}
		T At(uint32_t x, uint32_t y) const
		{
			return matrix.at(y).at(x);
		}

		void Fill(T val)
		{
			for (auto& h : matrix)
			{
				for (auto& w : h)
				{
					w = val;
				}
			}
		}
	};

	typedef Matrix<int> Matrixi;
	typedef Matrix<float> Matrixf;
}