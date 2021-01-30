#pragma once

#include <iostream>
#include <vector>
#include <algorithm>
#include "Vec2.hpp"

#undef max()
#undef min()

namespace lio
{
	template <typename T>
	class LineSeg
	{
	public:

		Vec2<T> p1, p2;

		LineSeg(T x1 = 0.0, T y1 = 0.0, T x2 = 0.0, T y2 = 0.0) : p1(x1, y1), p2(x2, y2) {}
		LineSeg(const Vec2<T>& p1, const Vec2<T>& p2 = Vec2<T>::Zero()) : p1(p1), p2(p2) {}
		LineSeg(const LineSeg& l) : p1(l.p1), p2(l.p2) {}

		template <typename U>
		operator LineSeg<U>() const
		{
			return LineSeg<U>(p1, p2);
		}
		template <typename U>
		LineSeg<U> Cast() const
		{
			return LineSeg<U>(p1, p2);
		}

		LineSeg& operator=(const LineSeg& l)
		{
			p1 = l.p1;
			p2 = l.p2;
			return *this;
		}

		LineSeg& Set(const Vec2<T>& p1, const Vec2<T>& p2 = Vec2<T>::Zero())
		{
			this->p1 = p1;
			this->p2 = p2;
			return *this;
		}
		LineSeg& Set(T x1, T y1, T x2, T y2)
		{
			p1.x = x1;
			p1.y = y1;
			p2.x = x2;
			p2.y = x2;
			return *this;
		}

		template <typename U, typename S = T>
		LineSeg<T> Rotated(U theta, const Vec2<S>& pt = Vec2<S>::Zero()) const
		{
			Vec2<T> p1 = (this->p1 - pt).Rotated(theta) + pt;
			Vec2<T> p2 = (this->p2 - pt).Rotated(theta) + pt;

			return LineSeg<T>(p1, p2);
		}
		template <typename U, typename S = T>
		LineSeg& Rotate(U theta, const Vec2<S>& pt = Vec2<S>::Zero())
		{
			return *this = this->Rotated(theta, pt);
		}

		auto Length() const
		{
			return p1.Distance(p2);
		}

		template <typename U>
		bool Lies(const Vec2<U>& pt) const
		{
			if (pt.x <= std::max(p1.x, p2.x) && pt.x >= std::min(p1.x, p2.x) &&
				pt.y <= std::max(p1.y, p2.y) && pt.y >= std::min(p1.y, p2.y) &&
				p1.Orientation(pt, p2) == 0)
				return true;

			return false;
		}

		template <typename U>
		bool Intersect(const LineSeg<U>& l) const
		{
			if (p1.Orientation(p2, l.p1) != p1.Orientation(p2, l.p2) &&
				l.p1.Orientation(l.p2, p1) != l.p1.Orientation(l.p2, p2))
				return true;

			if (Lies(l.p1)) return true;
			if (Lies(l.p2)) return true;
			if (l.Lies(p1)) return true;
			if (l.Lies(p2)) return true;

			return false;
		}
		template <typename U>
		auto IntersectPt(const LineSeg<U>& l) const -> Vec2<decltype(std::declval<T&>() + std::declval<U&>())>
		{
			double a1 = p2.y - p1.y;
			double b1 = p1.x - p2.x;
			double c1 = a1 * (p1.x) + b1 * (p1.y);

			double a2 = l.p2.y - l.p1.y;
			double b2 = l.p1.x - l.p2.x;
			double c2 = a2 * (l.p1.x) + b2 * (l.p1.y);

			double determinant = a1 * b2 - a2 * b1;

			if (determinant == 0)
			{
				return Vec2<double>(DBL_MAX, DBL_MAX);
			}
			else
			{
				double x = (b2 * c1 - b1 * c2) / determinant;
				double y = (a1 * c2 - a2 * c1) / determinant;
				return Vec2<double>(x, y);
			}
		}

		Vec2<T> MidPt() const
		{
			return Vec2<T>((p1.x + p2.x) / 2, (p1.y + p2.y) / 2);
		}
		Vec2<T> Section(double r1, double r2) const
		{
			auto x = (r1 * p2.x + r2 * p1.x) / r1 + r2;
			auto y = (r1 * p2.y + r2 * p1.y) / r1 + r2;
			return Vec2<T>(x, y);
		}

		std::vector<Vec2<int>>::iterator Plot(std::vector<Vec2<int>>& out) const
		{
			auto it = out.end();
			int x1 = p1.x, y1 = p1.y;
			int x2 = p2.x, y2 = p2.y;

			int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;

			dx = x2 - x1; dy = y2 - y1;
			dx1 = abs(dx); dy1 = abs(dy);
			px = 2 * dy1 - dx1;	py = 2 * dx1 - dy1;

			if (dy1 <= dx1)
			{
				if (dx >= 0)
				{
					x = x1; y = y1; xe = x2;
				}
				else
				{
					x = x2; y = y2; xe = x1;
				}

				out.emplace_back(x, y);

				for (i = 0; x < xe; i++)
				{
					x = x + 1;
					if (px < 0)
						px = px + 2 * dy1;
					else
					{
						if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y = y + 1; else y = y - 1;
						px = px + 2 * (dy1 - dx1);
					}
					out.emplace_back(x, y);
				}
			}
			else
			{
				if (dy >= 0)
				{
					x = x1; y = y1; ye = y2;
				}
				else
				{
					x = x2; y = y2; ye = y1;
				}

				out.emplace_back(x, y);

				for (i = 0; y < ye; i++)
				{
					y = y + 1;
					if (py <= 0)
						py = py + 2 * dx1;
					else
					{
						if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x = x + 1; else x = x - 1;
						py = py + 2 * (dx1 - dy1);
					}
					out.emplace_back(x, y);
				}
			}

			return it;
		}
		std::vector<Vec2<int>> Plot() const
		{
			std::vector<Vec2<int>> results;
			Plot(results);
			return results;
		}
	};

	template <typename T, typename U>
	auto operator+(const Vec2<U>& v, const LineSeg<T>& l)
	{
		return LineSeg<decltype(std::declval<T&>() + std::declval<U&>())>(l.p1 + v, l.p2 + v);
	}
	template <typename T, typename U>
	auto operator*(const Vec2<U>& v, const LineSeg<T>& l)
	{
		return LineSeg<decltype(std::declval<T&>() * std::declval<U&>())>(l.p1 * v, l.p2 * v);
	}
	template <typename T, typename U>
	auto operator/(const Vec2<U>& v, const LineSeg<T>& l)
	{
		return LineSeg<decltype(std::declval<T&>() / std::declval<U&>())>(v / l.p1, v / l.p2);
	}
	template <typename T, typename U>
	auto operator%(const Vec2<U>& v, const LineSeg<T>& l)
	{
		return LineSeg<decltype(fmod(std::declval<U&>(), std::declval<T&>()))>(v % l.p1, v % l.p2);
	}

	template <typename T, typename U>
	auto operator+(const LineSeg<T>& l, const Vec2<U>& v)
	{
		return LineSeg<decltype(std::declval<T&>() + std::declval<U&>())>(l.p1 + v, l.p2 + v);
	}
	template <typename T, typename U>
	auto operator-(const LineSeg<T>& l, const Vec2<U>& v)
	{
		return LineSeg<decltype(std::declval<T&>() - std::declval<U&>())>(l.p1 - v, l.p2 - v);
	}
	template <typename T, typename U>
	auto operator*(const LineSeg<T>& l, const Vec2<U>& v)
	{
		return LineSeg<decltype(std::declval<T&>() * std::declval<U&>())>(l.p1 * v, l.p2 * v);
	}
	template <typename T, typename U>
	auto operator/(const LineSeg<T>& l, const Vec2<U>& v)
	{
		return LineSeg<decltype(std::declval<T&>() / std::declval<U&>())>(l.p1 / v, l.p2 / v);
	}
	template <typename T, typename U>
	auto operator%(const LineSeg<T>& l, const Vec2<U>& v)
	{
		return LineSeg<decltype(fmod(std::declval<T&>(), std::declval<U&>()))>(l.p1 % v, l.p2 % v);
	}

	template <typename T, typename U>
	LineSeg<T>& operator+=(LineSeg<T>& l, const Vec2<U>& v)
	{
		l.p1 += v;
		l.p2 += v;
		return l;
	}
	template <typename T, typename U>
	LineSeg<T>& operator-=(LineSeg<T>& l, const Vec2<U>& v)
	{
		l.p1 -= v;
		l.p2 -= v;
		return l;
	}
	template <typename T, typename U>
	LineSeg<T>& operator*=(LineSeg<T>& l, const Vec2<U>& v)
	{
		l.p1 *= v;
		l.p2 *= v;
		return l;
	}
	template <typename T, typename U>
	LineSeg<T>& operator/=(LineSeg<T>& l, const Vec2<U>& v)
	{
		l.p1 /= v;
		l.p2 /= v;
		return l;
	}
	template <typename T, typename U>
	LineSeg<T>& operator%=(LineSeg<T>& l, const Vec2<U>& v)
	{
		l.p1 %= v;
		l.p2 %= v;
		return l;
	}

	template <typename T>
	auto operator*(double s, const LineSeg<T>& l)
	{
		return LineSeg<decltype(s * std::declval<T&>())>(l.p1 * s, l.p2 * s);
	}
	template <typename T>
	auto operator/(double s, const LineSeg<T>& l)
	{
		return LineSeg<decltype(s / std::declval<T&>())>(s / l.p1, s / l.p2);
	}
	template <typename T>
	auto operator%(double s, const LineSeg<T>& l)
	{
		return LineSeg<decltype(fmod(std::declval<T&>(), s))>(s % l.p1, s % l.p2);
	}

	template <typename T>
	auto operator*(const LineSeg<T>& l, double s)
	{
		return LineSeg<decltype(s * std::declval<T&>())>(l.p1 * s, l.p2 * s);
	}
	template <typename T>
	auto operator/(const LineSeg<T>& l, double s)
	{
		return LineSeg<decltype(s / std::declval<T&>())>(l.p1 / s, l.p2 / s);
	}
	template <typename T>
	auto operator%(const LineSeg<T>& l, double s)
	{
		return LineSeg<decltype(fmod(s, std::declval<T&>()))>(l.p1 % s, l.p2 % s);
	}

	template <typename T>
	LineSeg<T>& operator*=(LineSeg<T>& l, double s)
	{
		l.p1 *= s;
		l.p2 *= s;
		return l;
	}
	template <typename T>
	LineSeg<T>& operator/=(LineSeg<T>& l, double s)
	{
		l.p1 /= s;
		l.p2 /= s;
		return l;
	}
	template <typename T>
	LineSeg<T>& operator%=(LineSeg<T>& l, double s)
	{
		l.p1 %= s;
		l.p2 %= s;
		return l;
	}

	template <typename T, typename U>
	bool operator==(const LineSeg<T>& l1, const LineSeg<U>& l2)
	{
		return l1.p1 == l2.p1 && l1.p2 == l2.p2;
	}
	template <typename T, typename U>
	bool operator!=(const LineSeg<T>& l1, const LineSeg<U>& l2)
	{
		return !(l1.p1 == l2.p1 && l1.p2 == l2.p2);
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const LineSeg<T>& l)
	{
		os << "(" << l.p1 << "), (" << l.p2 << ")";
		return os;
	}

	typedef LineSeg<double> LineSegd;
	typedef LineSeg<float> LineSegf;
	typedef LineSeg<int> LineSegi;
}