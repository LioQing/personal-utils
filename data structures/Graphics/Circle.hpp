#include "Vec2.hpp"

namespace lio
{
	template <typename T, typename _Ty = T>
	class Circle
	{
	public:

		Vec2<T> center;
		_Ty radius;

		Circle(_Ty r = 0.0, T x = 0.0, T y = 0.0) : radius(r), center(x, y) {}
		template <typename U> Circle(_Ty r, const Vec2<U>& ctr) : radius(r), center(ctr.x, ctr.y) {}
		template <typename U, typename _Uy> Circle(const Circle<U, _Uy>& c) : radius(c.radius), center(c.center) {}

		template <typename U, typename _Uy>
		Circle& operator=(const Circle<U, _Uy>& c)
		{
			center = c.center;
			radius = c.radius;
			return *this;
		}
		Circle& operator=(_Ty r)
		{
			center = Vec2<T>::Zero();
			radius = r;
			return *this;
		}

		template <typename U, typename _Uy>
		friend bool operator==(const Circle& c1, const Circle<U, _Uy>& c2)
		{
			return c1.center == c2.center && c1.radius == c2.radius;
		}
		template <typename U, typename _Uy>
		friend bool operator!=(const Circle& c1, const Circle<U, _Uy>& c2)
		{
			return !(c1.center == c2.center && c1.radius == c2.radius);
		}

		friend std::ostream& operator<<(std::ostream& os, const Circle& c)
		{
			os << "center(" << c.center << "), radius = " << c.radius;
			return os;
		}
		friend std::istream& operator>>(std::istream& is, Circle& c)
		{
			is >> c.radius >> c.center;
			return is;
		}

		template <typename U>
		Circle& Set(_Ty r, const Vec2<U>& ctr)
		{
			radius = r;
			center.x = ctr.x;
			center.y = ctr.y;
			return *this;
		}
		Circle& Set(_Ty r = 0.0, T x = 0.0, T y = 0.0)
		{
			radius = r;
			center.x = x;
			center.y = y;
			return *this;
		}
	};
}