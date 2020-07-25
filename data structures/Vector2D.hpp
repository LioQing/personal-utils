#pragma once

#include <iostream>
#include <cmath>

namespace lio
{
	template <typename T>
	class Vec2 
	{
	private:

		double _mag;
		double _rot;

	public:

		T x, y;

		Vec2() { Set(0.0); }
		Vec2(double mag) { Set(mag); }
		Vec2(T x, T y) { Set(x, y); }
		template <typename U> Vec2(const Vec2<U>& v) { Set(v); }

		template <typename U> Vec2& operator=(const Vec2<U>& v) { return Set(v); }
		Vec2& operator=(T s) { return Set(s); }

		template<typename U> Vec2 operator+(const Vec2<U>& v) const { return Vec2(x + v.x, y + v.y); }
		template<typename U> Vec2 operator-(const Vec2<U>& v) const { return Vec2(x - v.x, y - v.y); }
		template<typename U> Vec2 operator*(const Vec2<U>& v) const { return Vec2(x * v.x, y * v.y); }
		template<typename U> Vec2 operator/(const Vec2<U>& v) const { return Vec2(x / v.x, y / v.y); }
		template<typename U> Vec2 operator%(const Vec2<U>& v) const { return Vec2(x % v.x, y % v.y); }

		template<typename U> Vec2& operator+=(const Vec2<U>& v) { return Set(*this + v); }
		template<typename U> Vec2& operator-=(const Vec2<U>& v) { return Set(*this - v); }
		template<typename U> Vec2& operator*=(const Vec2<U>& v) { return Set(*this * v); }
		template<typename U> Vec2& operator/=(const Vec2<U>& v) { return Set(*this / v); }
		template<typename U> Vec2& operator%=(const Vec2<U>& v) { return Set(*this % v); }

		Vec2 operator+(double s) const { return _MagRot(_mag + s, _rot); }
		Vec2 operator-(double s) const { return _MagRot(_mag - s, _rot); }
		Vec2 operator*(double s) const { return Vec2(x * s, y * s); }
		Vec2 operator/(double s) const { return Vec2(x / s, y / s); }
		Vec2 operator%(double s) const { return Vec2(x % s, y % s); }

		Vec2& operator+=(double s) { return _SetMagRot(_mag + s, _rot); }
		Vec2& operator-=(double s) { return _SetMagRot(_mag - s, _rot); }
		Vec2& operator*=(T s) { return Set(*this * s); }
		Vec2& operator/=(T s) { return Set(*this / s); }
		Vec2& operator%=(T s) { return Set(*this % s); }

		Vec2 operator-() const { return Vec2(-x, -y); }
		Vec2 operator+() const { return Vec2(x, y); }

		Vec2& operator++() { return _SetMagRot(_mag + 1.0, _rot); }
		Vec2& operator--() { return _SetMagRot(_mag - 1.0, _rot); }
		Vec2 operator++(int)
		{
			Vec2 tmp = *this;
			_MagRot(_mag + 1.0, _rot);
			return tmp;
		}
		Vec2 operator--(int)
		{
			Vec2 tmp = *this;
			_MagRot(_mag - 1.0, _rot);
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

		double _Rot() const { return _rot; }

 		template <typename U>
		Vec2& Set(const Vec2<U>& v)
		{
			static_assert(std::is_convertible<U, T>::value, "not convertible type");
			_mag = v.Magnitude();
			_rot = v._Rot();
			x = v.x;
			y = v.y;
			return *this;
		}
		Vec2& Set(double mag)
		{
			_mag = mag;
			_rot = 0.0;
			x = mag;
			y = 0.0;
			return *this;
		} 
		Vec2& Set(T x, T y)
		{
			_mag = _Magnitude(x, y);
			_rot = _Rotation(x, y);
			this->x = x;
			this->y = y;
			return *this;
		}

	private:

		Vec2(double mag, double rot, T x, T y) : _mag(mag), _rot(rot), x(x), y(y) {}

		double _Magnitude(double x, double y) const
		{
			return std::hypot(x, y);
		}
		double _Rotation(double x, double y) const
		{
			return atan2(y, x);
		}

		template <typename U>
		Vec2 _MagRot(const U mag, const U rot) const
		{
			U tx = mag * cos(rot);
			U ty = mag * sin(rot);

			return Vec2(mag, rot, tx, ty);
		}
		template <typename U>
		Vec2& _SetMagRot(const U mag, const U rot)
		{
			_mag = mag;
			_rot = rot;
			x = mag * cos(rot);
			y = mag * sin(rot);

			return *this;
		}

	public:

		Vec2<float> Rotated(float theta) { return _MagRot<float>(_mag, _rot + theta); }
		Vec2<float> Rotatedf(float theta) { return _MagRot<float>(_mag, _rot + theta); }
		Vec2<double> Rotated(double theta) { return _MagRot<double>(_mag, _rot + theta); }
		Vec2<long double> Rotated(long double theta) { return _MagRot<long double>(_mag, _rot + theta); }
		Vec2<long double> Rotatedl(long double theta) { return _MagRot<long double>(_mag, _rot + theta); }
		template <typename U>
		Vec2<double> Rotated(U theta) 
		{ 
			return _MagRot<double>(_mag, _rot + theta); 
		}

		template<typename U>
		Vec2& Rotate(U theta)
		{
			return _SetMagRot<double>(_mag, _rot + theta);
		}

		Vec2<double> Normalized() const
		{
			if (Magnitude() == 0) return *this;
			return _MagRot(1.0, _rot);
		}
		Vec2& Normalize()
		{
			if (Magnitude() == 0) return *this;
			return _SetMagRot(1.0, _rot);
		}

		Vec2 Ortho() const
		{
			return Vec2(y, -x);
		}

		template <typename U>
		double Distance(const Vec2<U>& v) const
		{
			Vec2<double> d(static_cast<double>(v.x) - x, static_cast<double>(v.y) - y);
			return d.Magnitude();
		}

		double Magnitude() const
		{
			return _mag;
		}

		Vec2 Abs() const
		{
			return Vec2(x < 0 ? x * -1 : x, y < 0 ? y * -1 : y);
		}

		template <typename U>
		double Dot(const Vec2<U>& v) const
		{
			return static_cast<double>(x) * v.x + static_cast<double>(y) * v.y;
		}
		template <typename U>
		double Cross(const Vec2<U>& v) const
		{
			return (static_cast<double>(x) * v.x) - (static_cast<double>(y) * v.y);
		}

		// 0 --> Collinear
		// 1 --> Clockwise 
		// -1 -> Counterclockwise 
		template <typename U, typename S>
		int Orientation(const Vec2<U>& v2, const Vec2<S>& v3) const
		{
			int val = (static_cast<double>(v2.y) - y) * (static_cast<double>(v3.x) - v2.x) - 
			(static_cast<double>(v2.x) - x) * (static_cast<double>(v3.y) - v2.y);

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