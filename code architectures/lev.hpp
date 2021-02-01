#pragma once

#include <vector>
#include <functional>
#include <any>

namespace lev
{
	class Event;
	struct Listener;

	template <typename T>
	concept IsEvent = std::is_base_of<Event, T>::value;

	namespace
	{
		using EventID = uint32_t;

		template <IsEvent E>
		EventID GetEventID();

		std::vector<std::any> listeners;

		template <IsEvent E>
		EventID GetNextEventID()
		{
			static EventID next_id = 0u;

			std::any functions = std::vector<std::function<void(E&)>>();
			listeners.push_back(functions);

			return next_id++;
		}

		template <IsEvent E>
		EventID GetEventID()
		{
			static EventID id = GetNextEventID<E>();
			return id;
		}
	}

	class Event
	{
	private:

		template <IsEvent E, typename ...TArgs>
		friend void Emit(TArgs&& ...args);

		EventID id;

	public:

		virtual ~Event() = default;
	};

	template <IsEvent E, typename T>
	void Listen(T& listener, void(T::*function)(E&))
	{
		std::any_cast<std::vector<std::function<void(E&)>>&>(listeners.at(GetEventID<E>()))
			.push_back([&listener, function](E& event) { (listener.*function)(event); });
	}

	template <IsEvent E, typename ...TArgs>
	void Emit(TArgs&& ...args)
	{
		E event(std::forward<TArgs>(args)...);
		event.id = GetEventID<E>();

		for (auto& function : std::any_cast<std::vector<std::function<void(E&)>>&>(listeners.at(GetEventID<E>())))
		{
			function(event);
		}
	}

	template <IsEvent E>
	void Emit(E& event)
	{
		for (auto& function : std::any_cast<std::vector<std::function<void(E&)>>&>(listeners.at(GetEventID<E>())))
		{
			function(event);
		}
	}
}