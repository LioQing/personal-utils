#pragma once

#ifndef M_PI
#define M_PI 3.1415926535897932384
#endif

#include "Vec2.hpp"
#include "Transformable.hpp"

namespace lio
{
	template <typename T, typename _Ty = T>
	class Circle : public Transformable<T>
	{
	private:

		_Ty m_radius;

	public:

		Circle(_Ty radius = 0.0, const Vec2<T>& position = Vec2<T>::Zero())
			: m_radius(radius), Transformable<T>(position) {}
		Circle(const Circle& c)
			: m_radius(c.m_radius), Transformable<T>(c) {}

		template <typename U, typename _Uy>
		operator Circle<U, _Uy>() const
		{
			return Circle<U, _Uy>(m_radius, Transformable<T>::GetPosition());
		}
		template <typename U, typename _Uy>
		Circle<U, _Uy> Cast() const
		{
			return Circle<U, _Uy>(m_radius, Transformable<T>::GetPosition());
		}

		const _Ty& GetRadius() const { return m_radius; }
		void SetRadius(_Ty radius)
		{
			m_radius = radius;
		}

		Vec2<T> GetCenter() const { return Transformable<T>::GetPosition() + Vec2<_Ty>(m_radius, m_radius); }
		void SetCenter(const Vec2<T>& center)
		{
			Transformable<T>::SetPosition(center - Vec2<_Ty>(m_radius, m_radius));
		}
		void SetCenter(T x, T y)
		{
			Transformable<T>::SetPosition(x - m_radius, y - m_radius);
		}

		double Diameter() const
		{
			return 2 * m_radius;
		}
		double Circumference() const
		{
			return 2 * M_PI * m_radius;
		}
		double Area() const
		{
			return M_PI * m_radius * m_radius;
		}

		// <0 -> overlapped
		// 0 --> touch each other
		// >0 -> not overlapped
		template <typename U, typename _Uy>
		double OverlapEx(const Circle<U, _Uy>& c) const
		{
			return GetCenter().Distance(c.GetCenter()) - m_radius - c.GetRadius();
		}
		// -1 -> overlapped
		// 0 --> touch each other
		// 1 --> not overlapped
		template <typename U, typename _Uy>
		int Overlap(const Circle<U, _Uy>& c) const
		{
			int val = OverlapEx(c);

			if (val == 0) return 0;
			return (val > 0) ? 1 : -1;
		}

		// <0 -> lies inside circle
		// 0 --> lies on circle
		// >0 -> lies outside circle
		template <typename U>
		double LiesEx(const Vec2<U>& v) const
		{
			return GetCenter().Distance(v) - m_radius;
		}
		// -1 -> lies inside circle
		// 0 --> lies on circle
		// 1 --> lies outside circle
		template <typename U>
		int Lies(const Vec2<U>& v) const
		{
			int val = LiesEx(v);

			if (val == 0) return 0;
			return (val > 0) ? 1 : -1;
		}
	};
}