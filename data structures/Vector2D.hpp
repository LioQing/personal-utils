#pragma once

#include <cmath>
#if __cplusplus > 201103L
	#include <initializer_list>
#endif

template <typename T>
class Vec2 
{
public:

	T x, y;

	Vec2() : x(0), y(0) {}
	Vec2(const T x, const T y) : x(x), y(y) {}
	Vec2(const Vec2& v) : x(v.x), y(v.y) {}

	Vec2& operator=(const Vec2& v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}
	Vec2& operator=(const T s)
	{
		x = s;
		y = s;
		return *this;
	}
#if __cplusplus > 201103L
	Vec2& operator=(const std::initializer_list<T>& v)
	{
		x = *v.begin();
		y = *(std::next(v.begin()));
		return *this;
	}
#endif

	Vec2 operator+(const Vec2& v) const
	{
		return Vec2(x + v.x, y + v.y);
	}
	Vec2 operator-(const Vec2& v) const
	{
		return Vec2(x - v.x, y - v.y);
	}
	Vec2 operator*(const Vec2& v) const
	{
		return Vec2(x * v.x, y * v.y);
	}
	Vec2 operator/(const Vec2& v) const 
	{
		return Vec2(x / v.x, y / v.y);
	}
	Vec2 operator%(const Vec2& v) const 
	{
		return Vec2(x % v.x, y % v.y);
	}

	Vec2& operator+=(const Vec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}
	Vec2& operator-=(const Vec2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}
	Vec2& operator*=(const Vec2& v)
	{
		x *= v.x;
		y *= v.y;
		return *this;
	}
	Vec2& operator/=(const Vec2& v)
	{
		x /= v.x;
		y /= v.y;
		return *this;
	}
	Vec2& operator%=(const Vec2& v)
	{
		x %= v.x;
		y %= v.y;
		return *this;
	}

	Vec2 operator+(const T s) const
	{
		return Vec2(x + s, y + s);
	}
	Vec2 operator-(const T s) const 
	{
		return Vec2(x - s, y - s);
	}
	Vec2 operator*(const T s) const 
	{
		return Vec2(x * s, y * s);
	}
	Vec2 operator/(const T s) const
	{
		return Vec2(x / s, y / s);
	}
	Vec2 operator%(const T s) const
	{
		return Vec2(x % s, y % s);
	}

	Vec2& operator+=(const T s)
	{
		x += s;
		y += s;
		return *this;
	}
	Vec2& operator-=(const T s)
	{
		x -= s;
		y -= s;
		return *this;
	}
	Vec2& operator*=(const T s)
	{
		x *= s;
		y *= s;
		return *this;
	}
	Vec2& operator/=(const T s)
	{
		x /= s;
		y /= s;
		return *this;
	}
	Vec2& operator%=(const T s)
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

	friend bool operator==(const Vec2& v1, const Vec2& v2)
	{
		return v1.x == v2.x && v1.y == v2.y;
	}
	friend bool operator!=(const Vec2& v1, const Vec2& v2)
	{
		return !(v1.x == v2.x && v1.y == v2.y);
	}
#if __cplusplus > 201103L
	friend bool operator==(const Vec2& v1, const std::initializer_list<T> v2)
	{
		return v1.x == v2.begin() && v1.y == std::next(v2.y.begin());
	}
	friend bool operator!=(const Vec2& v1, const std::initializer_list<T> v2)
	{
		return !(v1.x == v2.begin() && v1.y == std::next(v2.y.begin()));
	}
#endif

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

	void Set(T x, T y)
	{
		this->x = x;
		this->y = y;
	}

	Vec2 Rotated(T theta) const
	{
		T c = cos(theta);
		T s = sin(theta);
		T tx = x * c - y * s;
		T ty = x * s + y * c;

		return Vec2(tx, ty);
	}
	Vec2& Rotate(T theta)
	{
		T c = cos(theta);
		T s = sin(theta);
		T tx = x * c - y * s;
		T ty = x * s + y * c;
		x = tx;
		y = ty;

		return *this;
	}

	Vec2 Normalized() const
	{
		if (Magnitude() == 0) return *this;
		return Vec2(*this / Magnitude());
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

	T Distance(const Vec2& v) const
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

	static T Dot(const Vec2& v1, const Vec2& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}
	static T Cross(const Vec2& v1, const Vec2& v2)
	{
		return (v1.x * v2.y) - (v1.y * v2.x);
	}

	static Vec2 One()
	{
		return Vec2(1, 1);
	}
	static Vec2 Zero()
	{
		return Vec2(0, 0);
	}
	static Vec2 Up()
	{
		return Vec2(0, 1);
	}
	static Vec2 Down()
	{
		return Vec2(0, -1);
	}
	static Vec2 Left()
	{
		return Vec2(-1, 0);
	}
	static Vec2 Right()
	{
		return Vec2(1, 0);
	}

	template <typename S>
	Vec2<S> Cast() const
	{
		return Vec2<S>(x, y);
	}
};

typedef Vec2<double> Vector2D;
typedef Vec2<float> Vector2Df;
typedef Vec2<int> Vector2Di;