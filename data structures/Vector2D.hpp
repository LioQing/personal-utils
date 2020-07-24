#pragma once

#include <iostream>
#include <cmath>

template <typename T = double>
class Vec2 
{
public:

	T x, y;

	Vec2() : x(0.0), y(0.0) {}
	template <typename U> Vec2(const U s) : x(s), y(s) {}
	template <typename U> Vec2(const U x, const U y) : x(x), y(y) {}
	template <typename U> Vec2(const Vec2<U>& v) : x(v.x), y(v.y) {}

	template <typename U>
	Vec2& operator=(const Vec2<U>& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}
	template <typename U>
	Vec2& operator=(const U s)
	{
		x = s;
		y = s;
		return *this;
	}

	template<typename U>
	Vec2 operator+(const Vec2<U>& v) const
	{
		return Vec2(x + v.x, y + v.y);
	}
	template<typename U>
	Vec2 operator-(const Vec2<U>& v) const
	{
		return Vec2(x - v.x, y - v.y);
	}
	template<typename U>
	Vec2 operator*(const Vec2<U>& v) const
	{
		return Vec2(x * v.x, y * v.y);
	}
	template<typename U>
	Vec2 operator/(const Vec2<U>& v) const 
	{
		return Vec2(x / v.x, y / v.y);
	}
	template<typename U>
	Vec2 operator%(const Vec2<U>& v) const 
	{
		return Vec2(x % v.x, y % v.y);
	}

	template<typename U>
	Vec2& operator+=(const Vec2<U>& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	template<typename U>
	Vec2& operator-=(const Vec2<U>& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	template<typename U>
	Vec2& operator*=(const Vec2<U>& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}
	template<typename U>
	Vec2& operator/=(const Vec2<U>& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}
	template<typename U>
	Vec2& operator%=(const Vec2<U>& v)
	{
		x %= v.x;
		y %= v.y;
		return *this;
	}

	template<typename U>
	Vec2 operator+(const U s) const
	{
		return Vec2(x + s, y + s);
	}
	template<typename U>
	Vec2 operator-(const U s) const 
	{
		return Vec2(x - s, y - s);
	}
	template<typename U>
	Vec2 operator*(const U s) const 
	{
		return Vec2(x * s, y * s);
	}
	template<typename U>
	Vec2 operator/(const U s) const
	{
		return Vec2(x / s, y / s);
	}
	template<typename U>
	Vec2 operator%(const U s) const
	{
		return Vec2(x % s, y % s);
	}

	template<typename U>
	Vec2& operator+=(const U s)
	{
		x += s;
		y += s;
		return *this;
	}
	template<typename U>
	Vec2& operator-=(const U s)
	{
		x -= s;
		y -= s;
		return *this;
	}
	template<typename U>
	Vec2& operator*=(const U s)
	{
		x *= s;
		y *= s;
		return *this;
	}
	template<typename U>
	Vec2& operator/=(const U s)
	{
		x /= s;
		y /= s;
		return *this;
	}
	template<typename U>
	Vec2& operator%=(const U s)
	{
		x %= s;
		y %= s;
		return *this;
	}

	Vec2& operator++()
	{
		x += 1.0;
		y += 1.0;
		return *this;
	}
	Vec2& operator--()
	{
		x -= 1.0;
		y -= 1.0;
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

	template<typename U>
	void Set(U x, U y)
	{
		this->x = x;
		this->y = y;
	}

	template<typename S = T, typename U>
	Vec2<S> Rotated(U theta) const
	{
		double c = cos(theta);
		double s = sin(theta);
		S tx = static_cast<S>(x * c - y * s);
		S ty = static_cast<S>(x * s + y * c);

		return Vec2<S>(tx, ty);
	}
	template<typename U>
	Vec2& Rotate(U theta)
	{
		double c = cos(theta);
		double s = sin(theta);
		T tx = static_cast<T>(x * c - y * s);
		T ty = static_cast<T>(x * s + y * c);
		x = tx;
		y = ty;

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
		Vec2 d(v.x - x, v.y - y);
		return d.Magnitude();
	}

	double SqrMagnitude() const
	{
		return x * x + y * y;
	}
	double Magnitude() const
	{
		return std::sqrt(SqrMagnitude());
	}

	Vec2 Abs() const
	{
		return Vec2(x < 0 ? x * -1 : x, y < 0 ? y * -1 : y);
	}

	template <typename U>
	double Dot(const Vec2<U>& v)
	{
		return x * v.x + y * v.y;
	}
	template <typename U>
	double Cross(const Vec2<U>& v)
	{
		return (x * v.x) - (y * v.y);
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