#pragma once

#include <vector>
#include <functional>

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

		struct EventListWrapper
		{
			struct List;
			List* list;
		};

		std::vector<EventListWrapper> listeners;

		template <IsEvent E>
		EventID GetNextEventID()
		{
			static EventID next_id = 0u;

			EventListWrapper wrapper;
			wrapper.list = (EventListWrapper::List*)new std::vector<std::function<void(E&)>>();
			listeners.push_back(wrapper);

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

		template <IsEvent E>
		bool Is() const
		{
			return id == GetEventID<E>();
		}
	};

	template <IsEvent E, typename T>
	void Listen(T& listener, void(T::*function)(E&))
	{
		auto list = (std::vector<std::function<void(E&)>>*)listeners.at(GetEventID<E>()).list;
		list->push_back([&listener, function](E& event) { (listener.*function)(event); });
	}

	template <IsEvent E, typename ...TArgs>
	void Emit(TArgs&& ...args)
	{
		E event(std::forward<TArgs>(args)...);
		event.id = GetEventID<E>();

		for (auto& function : *(std::vector<std::function<void(E&)>>*)listeners.at(GetEventID<E>()).list)
		{
			function(event);
		}
	}
}