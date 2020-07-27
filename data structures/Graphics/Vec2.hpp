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
		template <typename U> Vec2(const Vec2<U>& v) : x(v.x), y(v.y) {}

		template <typename U>
		Vec2& operator=(const Vec2<U>& v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}
		Vec2& operator=(T s)
		{
			x = s;
			y = s;
			return *this;
		}

		template <typename U>
		Vec2 operator+(const Vec2<U>& v) const
		{
			return Vec2(x + v.x, y + v.y);
		}
		template <typename U>
		Vec2 operator-(const Vec2<U>& v) const
		{
			return Vec2(x - v.x, y - v.y);
		}
		template <typename U>
		Vec2 operator*(const Vec2<U>& v) const
		{
			return Vec2(x * v.x, y * v.y);
		}
		template <typename U>
		Vec2 operator/(const Vec2<U>& v) const 
		{
			return Vec2(x / v.x, y / v.y);
		}
		template <typename U>
		Vec2 operator%(const Vec2<U>& v) const 
		{
			return Vec2(fmod(x, v.x), fmod(y, v.y));
		}

		template <typename U>
		Vec2& operator+=(const Vec2<U>& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}
		template <typename U>
		Vec2& operator-=(const Vec2<U>& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}
		template <typename U>
		Vec2& operator*=(const Vec2<U>& v)
		{
			x *= v.x;
			y *= v.y;
			return *this;
		}
		template <typename U>
		Vec2& operator/=(const Vec2<U>& v)
		{
			x /= v.x;
			y /= v.y;
			return *this;
		}
		template <typename U>
		Vec2& operator%=(const Vec2<U>& v)
		{
			x = fmod(x, v.x);
			y = fmod(y, v.y);
			return *this;
		}

		template <typename U>
		friend Vec2<U> operator+(T s, const Vec2<U>& v)
		{
			double scale = s / v.Magnitude();
			return Vec2<U>(v.x + abs(v.x) * scale, v.y + abs(v.y) * scale);
		}
		template <typename U>
		friend Vec2<U> operator-(T s, const Vec2<U>& v)
		{
			double scale = s / v.Magnitude();
			return Vec2<U>(v.x - abs(v.x) * scale, v.y - abs(v.y) * scale);
		}
		template <typename U>
		friend Vec2<U> operator*(T s, const Vec2<U>& v)
		{
			return Vec2<U>(v.x * s, v.y * s);
		}
		template <typename U>
		friend Vec2<U> operator/(T s, const Vec2<U>& v)
		{
			return Vec2<U>(v.x / s, v.y / s);
		}
		template <typename U>
		friend Vec2<U> operator%(T s, const Vec2<U>& v)
		{
			return Vec2<U>(v.x % s, v.y % s);
		}

		Vec2 operator+(T s) const
		{
			double scale = s / Magnitude();
			return Vec2(x + abs(x) * scale, y + abs(y) * scale);
		}
		Vec2 operator-(T s) const
		{
			double scale = s / Magnitude();
			return Vec2(x - abs(x) * scale, y - abs(y) * scale);
		}
		Vec2 operator*(T s) const
		{
			return Vec2(x * s, y * s);
		}
		Vec2 operator/(T s) const 
		{
			return Vec2(x / s, y / s);
		}
		Vec2 operator%(T s) const
		{
			return Vec2(fmod(x, s), fmod(x, s));
		}

		Vec2& operator+=(T s)
		{
			double scale = s / Magnitude();
			x += abs(x) * scale;
			y += abs(y) * scale;
			return *this;
		}
		Vec2& operator-=(T s)
		{
			double scale = s / Magnitude();
			x -= abs(x) * scale;
			y -= abs(y) * scale;
			return *this;
		}
		Vec2& operator*=(T s)
		{
			x *= s;
			y *= s;
			return *this;
		}
		Vec2& operator/=(T s)
		{
			x /= s;
			y /= s;
			return *this;
		}
		Vec2& operator%=(T s)
		{
			x = fmod(x, s);
			y = fmod(y, s);
			return *this;
		}

		Vec2& operator++()
		{
			double scale = 1.0 / Magnitude();
			x += abs(x) * scale;
			y += abs(y) * scale;
			return *this;
		}
		Vec2& operator--()
		{
			double scale = 1.0 / Magnitude();
			x -= abs(x) * scale;
			y -= abs(y) * scale;
			return *this;
		}
		Vec2 operator++(int)
		{
			Vec2 tmp(x, y);
			x += 1.0;
			y += 1.0;
			return tmp;
		}
		Vec2 operator--(int)
		{
			Vec2 tmp(x, y);
			x -= 1.0;
			y -= 1.0;
			return tmp;
		}

		template <typename U>
		friend bool operator==(const Vec2& v1, const Vec2<U>& v2)
		{
			return v1.x == v2.x && v1.y == v2.y;
		}
		template <typename U>
		friend bool operator!=(const Vec2& v1, const Vec2<U>& v2)
		{
			return !(v1.x == v2.x && v1.y == v2.y);
		}

		friend std::ostream& operator<<(std::ostream& os, const Vec2& v)
		{
			os << v.x << ", " << v.y;
			return os;
		}
		friend std::istream& operator>>(std::istream& is, Vec2& v)
		{
			is >> v.x >> v.y;
			return is;
		}

		Vec2& Set(T x = 0.0, T y = 0.0)
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
			return Vec2<double>(this->Cast<double>() / Magnitude());
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

		// 0 --> Collinear
		// 1 --> Clockwise 
		// -1 -> Counterclockwise 
		template <typename U, typename S>
		int Orientation(const Vec2<U>& v2, const Vec2<S>& v3) const
		{
			int val = (v2.y - y) * (v3.x - v2.x) - (v2.x - x) * (v3.y - v2.y);

			if (val == 0) return 0;
			return (val > 0) ? 1 : -1;
		}

		static Vec2 One()	{ return Vec2(1.0, 1.0); }
		static Vec2 Zero()	{ return Vec2(0.0, 0.0); }
		static Vec2 Up()	{ return Vec2(0.0, 1.0); }
		static Vec2 Down()	{ return Vec2(0.0, -1.0); }
		static Vec2 Left()	{ return Vec2(-1.0, 0.0); }
		static Vec2 Right()	{ return Vec2(1.0, 0.0); }

		template <typename U>
		Vec2<U> Cast() const
		{
			return Vec2<U>(x, y);
		}
	};

	typedef Vec2<double> Vec2d;
	typedef Vec2<float> Vec2f;
	typedef Vec2<int> Vec2i;
}