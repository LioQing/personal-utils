#pragma once

#include <iostream>
#include <cmath>

namespace lio
{
	template <typename T>
	class Vec2 
	{
	public:

		T x, y;

		Vec2(T x = 0.0, T y = 0.0) : x(x), y(y) {}
		Vec2(const Vec2& v) : x(v.x), y(v.y) {}

		template <typename U>
		operator Vec2<U>() const
		{
			return Vec2<U>(x, y);
		}
		template <typename U>
		Vec2<U> Cast() const
		{
			return Vec2<U>(x, y);
		}

		Vec2& operator=(const Vec2& v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}
		Vec2& operator=(T s)
		{
			x = s;
			y = 0.0;
			return *this;
		}

		Vec2& Set(T x, T y)
		{
			this->x = x;
			this->y = y;
			return *this;
		}

		Vec2<double> Rotated(double theta) const
		{
			double c = cos(theta);
			double s = sin(theta);
			double tx = c * x - s * y;
			double ty = s * x + c * y;

			return Vec2<double>(tx, ty);
		}
		Vec2& Rotate(double theta)
		{
			double c = cos(theta);
			double s = sin(theta);
			x = c * x - s * y;
			y = s * x + c * y;

			return *this;
		}

		Vec2<double> Normalized() const
		{
			if (Magnitude() == 0) return *this;
			return Vec2<double>(*this / Magnitude());
		}
		Vec2& Normalize()
		{
			if (Magnitude() == 0) return *this;
			*this *= (1.0 / Magnitude());
			return *this;
		}

		Vec2 Ortho() const
		{
			return Vec2(y, -x);
		}

		template <typename U>
		double Distance(const Vec2<U>& v) const
		{
			Vec2<double> d(v.x - x, v.y - y);
			return d.Magnitude();
		}

		double SqrMagnitude() const
		{
			return x * x + y * y;
		}
		double Magnitude() const
		{
			return std::hypot(x, y);
		}

		Vec2 Abs() const
		{
			return Vec2(x < 0 ? x * -1 : x, y < 0 ? y * -1 : y);
		}

		template <typename U>
		double Dot(const Vec2<U>& v) const
		{
			return x * v.x + y * v.y;
		}
		template <typename U>
		double Cross(const Vec2<U>& v) const
		{
			return (x * v.x) - (y * v.y);
		}

		// 0 --> collinear
		// >0 -> clockwise 
		// <0 -> counterclockwise 
		template <typename U, typename S>
		double OrientationEx(const Vec2<U>& v2, const Vec2<S>& v3) const
		{
			return (v2.y - y) * (v3.x - v2.x) - (v2.x - x) * (v3.y - v2.y);
		}
		// 0 --> collinear
		// 1 --> clockwise 
		// -1 -> counterclockwise 
		template <typename U, typename S>
		int Orientation(const Vec2<U>& v2, const Vec2<S>& v3) const
		{
			int val = OrientationEx(v2, v3);

			if (val == 0) return 0;
			return (val > 0) ? 1 : -1;
		}

		static Vec2 One()	{ return Vec2(1.0, 1.0); }
		static Vec2 Zero()	{ return Vec2(0.0, 0.0); }
		static Vec2 Up()	{ return Vec2(0.0, 1.0); }
		static Vec2 Down()	{ return Vec2(0.0, -1.0); }
		static Vec2 Left()	{ return Vec2(-1.0, 0.0); }
		static Vec2 Right()	{ return Vec2(1.0, 0.0); }
	};

	template <typename T, typename U>
	auto operator+(const Vec2<T>& v1, const Vec2<U>& v2)
	{
		return Vec2<decltype(std::declval<T&>() + std::declval<U&>())>(v1.x + v2.x, v1.y + v2.y);
	}
	template <typename T, typename U>
	auto operator-(const Vec2<T>& v1, const Vec2<U>& v2)
	{
		return Vec2<decltype(std::declval<T&>() - std::declval<U&>())>(v1.x - v2.x, v1.y - v2.y);
	}
	template <typename T, typename U>
	auto operator*(const Vec2<T>& v1, const Vec2<U>& v2)
	{
		return Vec2<decltype(std::declval<T&>() * std::declval<U&>())>(v1.x * v2.x, v1.y * v2.y);
	}
	template <typename T, typename U>
	auto operator/(const Vec2<T>& v1, const Vec2<U>& v2)
	{
		return Vec2<decltype(std::declval<T&>() / std::declval<U&>())>(v1.x / v2.x, v1.y / v2.y);
	}
	template <typename T, typename U>
	auto operator%(const Vec2<T>& v1, const Vec2<U>& v2)
	{
		return Vec2<decltype(fmod(std::declval<T&>(), std::declval<U&>()))>(fmod(v1.x, v2.x), fmod(v1.y, v2.y));
	}

	template <typename T, typename U>
	Vec2<T>& operator+=(Vec2<T>& v1, const Vec2<U>& v2)
	{
		v1.x += v2.x;
		v1.y += v2.y;
		return v1;
	}
	template <typename T, typename U>
	Vec2<T>& operator-=(Vec2<T>& v1, const Vec2<U>& v2)
	{
		v1.x -= v2.x;
		v1.y -= v2.y;
		return v1;
	}
	template <typename T, typename U>
	Vec2<T>& operator*=(Vec2<T>& v1, const Vec2<U>& v2)
	{
		v1.x *= v2.x;
		v1.y *= v2.y;
		return v1;
	}
	template <typename T, typename U>
	Vec2<T>& operator/=(Vec2<T>& v1, const Vec2<U>& v2)
	{
		v1.x /= v2.x;
		v1.y /= v2.y;
		return v1;
	}
	template <typename T, typename U>
	Vec2<T>& operator%=(Vec2<T>& v1, const Vec2<U>& v2)
	{
		v1.x = fmod(v1.x, v2.x);
		v1.y = fmod(v1.y, v2.y);
		return v1;
	}

	template <typename T>
	auto operator+(double s, const Vec2<T>& v)
	{
		if (v.Magnitude() == 0) return v;
		double scale = s / v.Magnitude();
		return Vec2<decltype(std::declval<double&>() * std::declval<T&>())>(v.x + abs(v.x) * scale, v.y + abs(v.y) * scale);
	}
	template <typename T>
	auto operator-(double s, const Vec2<T>& v)
	{
		if (v.Magnitude() == 0) return v;
		double scale = s / v.Magnitude();
		return Vec2<decltype(std::declval<double&>() * std::declval<T&>())>(v.x - abs(v.x) * scale, v.y - abs(v.y) * scale);
	}
	template <typename T>
	auto operator*(double s, const Vec2<T>& v)
	{
		return Vec2<decltype(s * std::declval<T&>())>(v.x * s, v.y * s);
	}
	template <typename T>
	auto operator/(double s, const Vec2<T>& v)
	{
		return Vec2<decltype(s / std::declval<T&>())>(v.x / s, v.y / s);
	}
	template <typename T>
	auto operator%(double s, const Vec2<T>& v)
	{
		return Vec2<decltype(fmod(s, std::declval<T&>()))>(fmod(v.x, s), fmod(v.y, s));
	}

	template <typename T>
	auto operator+(const Vec2<T>& v, double s)
	{
		if (v.Magnitude() == 0) return v;
		double scale = s / v.Magnitude();
		return Vec2<decltype(std::declval<double&>() * std::declval<T&>())>(v.x + abs(v.x) * scale, v.y + abs(v.y) * scale);
	}
	template <typename T>
	auto operator-(const Vec2<T>& v, double s)
	{
		if (v.Magnitude() == 0) return v;
		double scale = s / v.Magnitude();
		return Vec2<decltype(std::declval<double&>() * std::declval<T&>())>(v.x - abs(v.x) * scale, v.y - abs(v.y) * scale);
	}
	template <typename T>
	auto operator*(const Vec2<T>& v, double s)
	{
		return Vec2<decltype(s * std::declval<T&>())>(v.x * s, v.y * s);
	}
	template <typename T>
	auto operator/(const Vec2<T>& v, double s)
	{
		return Vec2<decltype(s / std::declval<T&>())>(v.x / s, v.y / s);
	}
	template <typename T>
	auto operator%(const Vec2<T>& v, double s)
	{
		return Vec2<decltype(fmod(s, std::declval<T&>()))>(fmod(v.x, s), fmod(v.y, s));
	}

	template <typename T>
	Vec2<T>& operator+=(Vec2<T>& v, double s)
	{
		if (v.Magnitude() == 0) return v;
		auto scale = s / v.Magnitude();
		v.x += abs(v.x) * scale;
		v.y += abs(v.y) * scale;
		return v;
	}
	template <typename T>
	Vec2<T>& operator-=(Vec2<T>& v, double s)
	{
		if (v.Magnitude() == 0) return v;
		double scale = s / v.Magnitude();
		v.x -= abs(v.x) * scale;
		v.y -= abs(v.y) * scale;
		return v;
	}
	template <typename T>
	Vec2<T>& operator*=(Vec2<T>& v, double s)
	{
		v.x *= s;
		v.y *= s;
		return v;
	}
	template <typename T>
	Vec2<T>& operator/=(Vec2<T>& v, double s)
	{
		v.x /= s;
		v.y /= s;
		return v;
	}
	template <typename T>
	Vec2<T>& operator%=(Vec2<T>& v, double s)
	{
		v.x = fmod(v.x, s);
		v.y = fmod(v.y, s);
		return v;
	}

	template <typename T>
	Vec2<T>& operator++(Vec2<T>& v)
	{
		if (v.Magnitude() == 0) return v;
		double scale = 1.0 / v.Magnitude();
		v.x += abs(v.x) * scale;
		v.y += abs(v.y) * scale;
		return v;
	}
	template <typename T>
	Vec2<T>& operator--(Vec2<T>& v)
	{
		if (v.Magnitude() == 0) return v;
		double scale = 1.0 / v.Magnitude();
		v.x -= abs(v.x) * scale;
		v.y -= abs(v.y) * scale;
		return v;
	}
	template <typename T>
	Vec2<T> operator++(Vec2<T>& v, int)
	{
		if (v.Magnitude() == 0) return v;
		Vec2<T> tmp(v.x, v.y);
		double scale = 1.0 / v.Magnitude();
		v.x += abs(v.x) * scale;
		v.y += abs(v.y) * scale;
		return tmp;
	}
	template <typename T>
	Vec2<T> operator--(Vec2<T>& v, int)
	{
		if (v.Magnitude() == 0) return v;
		Vec2<T> tmp(v.x, v.y);
		double scale = 1.0 / v.Magnitude();
		v.x -= abs(v.x) * scale;
		v.y -= abs(v.y) * scale;
		return tmp;
	}

	
	template <typename T, typename U>
	bool operator==(const Vec2<T>& v1, const Vec2<U>& v2)
	{
		return v1.x == v2.x && v1.y == v2.y;
	}
	template <typename T, typename U>
	bool operator!=(const Vec2<T>& v1, const Vec2<U>& v2)
	{
		return !(v1.x == v2.x && v1.y == v2.y);
	}

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const Vec2<T>& v)
	{
		os << v.x << ", " << v.y;
		return os;
	}
	template <typename T>
	std::istream& operator>>(std::istream& is, Vec2<T>& v)
	{
		is >> v.x >> v.y;
		return is;
	}

	typedef Vec2<double> Vec2d;
	typedef Vec2<float> Vec2f;
	typedef Vec2<int> Vec2i;
}