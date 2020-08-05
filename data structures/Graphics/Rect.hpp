#pragma once

#include "Vec2.hpp"
#include "Transformable.hpp"

namespace lio
{
	template <typename T>
	class Rect : public Transformable<T>
	{
	private:

		Vec2<T> m_size;

	public:

		Rect(const Vec2<T>& position = Vec2<T>::Zero(), const Vec2<T>& size = Vec2<T>::One())
			: m_size(size), Transformable<T>(position) {}
		Rect(const Rect<T>& rect)
			: m_size(rect.m_size), Transformable<T>(rect.operator lio::Transformable<T>) {}

		template <typename U>
		operator Rect<U>() const
		{
			return Rect<U>(Transformable<T>::GetPosition(), m_size);
		}
		template <typename U>
		Rect<U> Cast() const
		{
			return Rect<U>(Transformable<T>::GetPosition(), m_size);
		}

		Vec2<T> GetSize() const { return m_size; }
		void SetSize(const Vec2<T>& size)
		{
			m_size = size;
		}
		void SetSize(T x, T y)
		{
			m_size.x = x;
			m_size.y = y;
		}

		T Area() const
		{
			return m_size.x * m_size.y;
		}
		double Diagonal() const
		{
			return m_size.Magnitude();
		}

		template <typename U>
		bool Overlap(const Rect<U>& rect) const
		{
			if (
				Transformable<T>::GetPosition().x + m_size.x >= rect.GetPosition().x &&
				rect.GetPosition().x + rect.m_size.x >= Transformable<T>::GetPosition().x &&
				Transformable<T>::GetPosition().y + m_size.y >= rect.GetPosition().y &&
				rect.GetPosition().y + rect.m_size.y >= Transformable<T>::GetPosition().y
				)
			{
				return true;
			}
			return false;
		}
	};
}