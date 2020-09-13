#pragma once

#include "Vec2.hpp"

namespace lio
{
	template <typename T>
	class Transformable
	{
	private:

		Vec2<T> m_position;
		
	public:

		Transformable(const Vec2<T>& position = Vec2<T>::Zero())
			: m_position(position) {}
		Transformable(const Transformable& transform)
			: m_position(transform.m_position) {}

		virtual ~Transformable() {}

		template <typename U>
		operator Transformable<U>() const
		{
			return std::move(Transformable<U>(m_position));
		}
		template <typename U>
		Transformable<U>&& Cast() const
		{
			return std::move(Transformable<U>(m_position));
		}

		const Vec2<T>& GetPosition() const { return m_position; }
		void SetPosition(const Vec2<T>& pos)
		{
			m_position = pos;
		}
		void SetPosition(T x, T y)
		{
			m_position.x = x;
			m_position.y = y;
		}

		void Move(const Vec2<T>& offset)
		{
			SetPosition(m_position.x + offset.x, m_position.y + offset.y);
		}
		void Move(T offset_x, T offset_y)
		{
			SetPosition(m_position.x + offset_x, m_position.y + offset_y);
		}
	};
}