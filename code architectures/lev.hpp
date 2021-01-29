#pragma once

#include <vector>

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

	namespace
	{
		std::vector<std::vector<Listener*>> listeners;

		EventID GetNextEventID()
		{
			static EventID next_id = 0u;
			listeners.push_back(std::vector<Listener*>());
			return next_id++;
		}

		template <IsEvent E>
		EventID GetEventID()
		{
			static EventID id = GetNextEventID();
			return id;
		}
	}

	struct Listener
	{
		virtual ~Listener() = default;

		template <IsEvent E>
		void Listen()
		{
			listeners.at(GetEventID<E>()).push_back(this);
		}

		template <IsEvent E>
		void StopListen()
		{
			std::erase_if(listeners.at(GetEventID<E>()), [this](Listener* ptr)
				{
					return ptr == this;
				});
		}

		virtual void On(const Event& event) = 0;
	};

	template <IsEvent E, typename ...TArgs>
	void Emit(TArgs&& ...args)
	{
		E event(std::forward<TArgs>(args)...);
		event.id = GetEventID<E>();
		const auto& ev = static_cast<const Event&>(event);

		for (auto& listener : listeners.at(GetEventID<E>()))
		{
			listener->On(ev);
		}
	}
}