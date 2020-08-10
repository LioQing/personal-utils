#pragma once

#include <map>
#include <typeinfo>

namespace lev
{
	struct Event
	{
		virtual ~Event() = default;
	};

	template <typename T>
	concept IsEvent = std::is_base_of<Event, T>::value;

	template <IsEvent E>
	static std::multimap<const std::type_info*, void (*)(const E&)> m_event_map;

	template <IsEvent E>
	void On(void (*fn)(const E&))
	{
		m_event_map<E>.emplace(&typeid(E), fn);
	}

	template <IsEvent E>
	void Emit(const E& event)
	{
		auto range = m_event_map<E>.equal_range(&typeid(event));
		for (auto it = m_event_map<E>.begin(); it != m_event_map<E>.end(); ++it)
		{
			it->second(event);
		}
	}
}