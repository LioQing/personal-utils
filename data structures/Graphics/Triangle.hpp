#pragma once

#include <array>
#include "Vec2.hpp"

namespace lio
{
	template <typename T>
	class Triangle
	{
	private:

		std::array<Vec2<T>, 3> p;

	public:

		Triangle(const Vec2<T>& p1 = lio::Vec2<T>::Zero(), const Vec2<T>& p2 = lio::Vec2<T>::Zero(),
			const Vec2<T>& p3 = lio::Vec2<T>::Zero())
			: p({ p1, p2, p3 }) {}
		Triangle(const Triangle<T>& tri)
			: p(tri.p) {}

		template <typename U>
		operator Triangle<U>() const
		{
			return Triangle<U>(*this);
		}
		template <typename U>
		Triangle<U> Cast() const
		{
			return Triangle<U>(*this);
		}

		const Vec2<T>& GetPoint(size_t i) const
		{
			return p.at(i);
		}
		const std::array<Vec2<T>, 3>& GetPoints() const
		{
			return p;
		}
		void SetPoint(size_t i, const Vec2<T>& val) const
		{
			p.at(i) = val;
		}
		void SetPoints(const Vec2<T>& p1, const Vec2<T>& p2, const Vec2<T>& p3)
		{
			p = { p1, p2, p3 }
		}
	};
}