#pragma once

#include <cstdint>
#include <memory>
#include <array>
#include <vector>
#include <bitset>
#include <queue>

namespace lecs
{
	constexpr std::size_t MAX_LOG = 32;
	constexpr std::size_t MAX_COMPONENT = 32;

	class Component
	{
	public:

		uint32_t entity;
	};

	std::size_t next_component_id = 0;

	template <typename T>
	inline std::size_t get_component_type_id()
	{
		static std::size_t id = next_component_id++;
		return id;
	}

	class EntityManager;

	class Entity
	{
	private:

		EntityManager& entity_manager;
		bool active = true;

	public:

		uint32_t id;

		std::array<std::unique_ptr<Component>, MAX_COMPONENT> components;
		std::bitset<MAX_COMPONENT> component_bitset;

		Entity(EntityManager& entity_manager, uint32_t id) : entity_manager(entity_manager), id(id) {}

		bool is_active()
		{
			return active;
		}
		void destroy(bool immediate = false);

		template <typename T>
		T& add_component(std::unique_ptr<T> u_ptr)
		{
			u_ptr->entity = id;

			components[get_component_type_id<T>()] = std::move(u_ptr);
			component_bitset[get_component_type_id<T>()] = true;

			return *u_ptr.get();
		}

		template <typename T>
		T& add_component(const T& c)
		{
			c->entity = id;
			std::unique_ptr<Component> u_ptr{ c };

			components[get_component_type_id<T>()] = std::move(u_ptr);
			component_bitset[get_component_type_id<T>()] = true;

			return *c;
		}

		template <typename T, typename... TArgs>
		T& add_component(TArgs&&... mArgs)
		{
			T* c(new T(std::forward<TArgs>(mArgs)...));
			c->entity = id;
			std::unique_ptr<Component> u_ptr{ c };

			components[get_component_type_id<T>()] = std::move(u_ptr);
			component_bitset[get_component_type_id<T>()] = true;

			return *c;
		}

		template <typename T>
		T& remove_component()
		{
			T c = *static_cast<T*>(components[get_component_type_id<T>()].get());
			delete components[get_component_type_id<T>()].release();
			component_bitset[get_component_type_id<T>()] = false;
			return c;
		}

		template <typename T>
		T& get_component() const
		{
			auto ptr(components[get_component_type_id<T>()].get());
			return *static_cast<T*>(ptr);
		}

		template <typename T>
		bool has_component()
		{
			return component_bitset[get_component_type_id<T>()];
		}
	};

	class EntityContainer
	{
	private:

		EntityManager& entity_manager;

	public:

		std::vector<Entity*> entities;

		EntityContainer(EntityManager& entity_manager) : entity_manager(entity_manager) {}

		template <typename T>
		EntityContainer entity_filter();
	};

	class EntityManager
	{
	private:

		uint32_t next_id;

	public:

		std::vector<std::unique_ptr<Entity>> entities;
		std::vector<uint32_t> empty_id;

		void update()
		{
			for (auto& e : entities)
			{
				if (!e) continue;
				if (!e->is_active())
				{
					empty_id.push_back(e->id);
					delete e.release();
				}
			}
		}

		void immediate_destroy(uint32_t id)
		{
			empty_id.push_back(id);
			delete entities.at(id).release();
		}

		Entity& add_entity()
		{
			uint32_t new_id;
			bool is_empty = empty_id.empty();
			if (!is_empty)
			{
				new_id = empty_id.back();
				empty_id.pop_back();
			}
			else
			{
				new_id = next_id++;
			}

			Entity* e(new Entity(*this, new_id));
			std::unique_ptr<Entity> u_ptr{ e };
			if (is_empty) entities.resize(entities.size() + 1);
			entities.at(e->id) = std::move(u_ptr);

			return *e;
		}

		EntityContainer entity_filter()
		{
			EntityContainer en = EntityContainer(*this);
			for (auto& e : entities)
			{
				if (!e) continue;
				en.entities.emplace_back(entities.at(e->id).get());
			}
			return en;
		}

		template <typename T>
		EntityContainer entity_filter()
		{
			EntityContainer entities_with = EntityContainer(*this);
			for (auto& e : entities)
			{
				if (!e) continue;
				uint32_t id = e->id;
				if (e->has_component<T>()) entities_with.entities.emplace_back(entities.at(id).get());
			}
			return entities_with;
		}
	};

	void Entity::destroy(bool immediate)
	{
		active = false;
		if (immediate) entity_manager.immediate_destroy(id);
	}

	template <typename T>
	EntityContainer EntityContainer::entity_filter()
	{
		{
			EntityContainer entities_with = EntityContainer(entity_manager);
			for (auto& e : entities)
			{
				if (!e) continue;
				uint32_t id = e->id;
				if (e->has_component<T>()) entities_with.entities.emplace_back(entity_manager.entities.at(id).get());
			}
			return entities_with;
		}
	}

	class Event;
	class EventManager;

	class EventSubscriber
	{
	public:

		std::vector<std::size_t> subscribed;

		virtual void receive(Event* event, EventManager* event_manager) {}
	};

	class Event
	{
	public:

		EventManager* event_manager;
		std::size_t id;
		std::vector<EventSubscriber*> subscribers;

		template <typename T>
		bool is_event();
	};

	class EventManager
	{
	private:

		EntityManager* entity_manager;
		std::size_t next_event_id = 0;

	public:

		std::vector<std::unique_ptr<Event>> events;

		EventManager() : entity_manager(nullptr) {}
		EventManager(EntityManager* entity_manager) : entity_manager(entity_manager) {}

		template <typename T>
		inline std::size_t get_event_id()
		{
			static std::size_t id = next_event_id++;
			return id;
		}

		template <typename T>
		void unsubscribe(EventSubscriber* subscriber)
		{
			add_event<T>();
			std::size_t id = get_event_id<T>();
			events.at(id)->subscribers.erase(std::remove_if(
				events.at(id)->subscribers.begin(), events.at(id)->subscribers.end(),
				[subscriber](EventSubscriber* s)
				{
					return s == subscriber;
				}),
				events.at(id)->subscribers.end());

			std::vector<std::size_t>* subscribed = &subscriber->subscribed;
			subscribed->erase(std::remove_if(subscribed->begin(), subscribed->end(),
				[id](const std::size_t& eID)
				{
					return eID == id;
				}),
				subscribed->end());
		}

		template <typename T>
		void subscribe(EventSubscriber* subscriber)
		{
			add_event<T>();
			events.at(get_event_id<T>())->subscribers.emplace_back(subscriber);
			subscriber->subscribed.emplace_back(get_event_id<T>());
		}

		template <typename T, typename... TArgs>
		void emit(TArgs... aArgs)
		{
			for (auto& sub : events.at(get_event_id<T>())->subscribers)
			{
				T* ev(new T(std::forward<TArgs>(aArgs)...));
				ev->event_manager = this;
				ev->id = get_event_id<T>();
				sub->receive(ev, this);
			}
		}

		template <typename T>
		void add_event()
		{
			if (get_event_id<T>() < events.size()) return;
			T* ev(new T());
			ev->id = get_event_id<T>();
			std::unique_ptr<T> u_ptr{ ev };
			events.resize(events.size() + 1);
			events.at(ev->id) = std::move(u_ptr);
		}
	};

	template <typename T>
	bool Event::is_event()
	{
		return id == event_manager->get_event_id<T>();
	}

	class System
	{
	public:

		virtual void update(EntityManager* entity_manager, EventManager* event_manager) {}
	};

	class SystemManager
	{
	private:

		EntityManager* entity_manager;
		EventManager* event_manager;

	public:

		std::vector<std::unique_ptr<System>> systems;

		SystemManager() : entity_manager(nullptr), event_manager(nullptr) {}
		explicit SystemManager(EntityManager* entity_manager, EventManager* event_manager)
			: entity_manager(entity_manager), event_manager(event_manager) {}

		template <typename T>
		T& add_system(const T& s)
		{
			std::unique_ptr<System> u_ptr{ s };
			systems.emplace_back(std::move(u_ptr));

			return *s;
		}

		template <typename T>
		T& add_system(std::unique_ptr<T> u_ptr)
		{
			systems.emplace_back(std::move(u_ptr));
			return *u_ptr.get();
		}

		template <typename T, typename... TArgs>
		T& add_system(TArgs&&... aArgs)
		{
			T* s(new T(std::forward<TArgs>(aArgs)...));
			std::unique_ptr<System> u_ptr{ s };
			systems.emplace_back(std::move(u_ptr));

			return *s;
		}

		void update()
		{
			for (auto& s : systems)
			{
				s->update(entity_manager, event_manager);
			}
		}
	};

	enum LogTag
	{
		LT_COMPONENT,
		LT_ENTITY,
		LT_SYSTEM,
		LT_ERROR,
		LT_CREATE,
		LT_DELETE,
		LT_DEBUG
	};

	class Logger
	{
	public:

		template <typename... T>
		void add_log(std::string log_msg, T... tag) 
		{
			LogTag tags[] = { tag... };
			std::pair<std::bitset<n_tag>, std::string> log;

			bool shown = false;
			log.second = log_msg;
			for (auto t : tags)
			{
				log.first[t] = true;
				log_per_tag[t] = log_msg;

				if (show[t] && !shown)
				{
					std::cout << log_msg << std::endl;
					shown = true;
				}
			}

			logs.push_front(log);

			if (logs.size() > MAX_LOG) logs.pop_back();
		}

		std::string get_logs(std::size_t n = MAX_LOG)
		{
			std::string log_msg = "";
			std::size_t begin = n > logs.size() ? logs.size() : n;
			for (std::size_t i = begin; i >= 1; --i)
			{
				log_msg += logs.at(i - 1).second + "\n";
			}
			return log_msg;
		}

		std::string get_log()
		{
			return logs.front().second;
		}

		std::string get_log(LogTag tag)
		{
			return log_per_tag[tag];
		}

		void always_show(bool always = true)
		{
			if (always) show.set();
			else show.reset();
		}

		template <typename... T>
		void always_show(bool always, T... tag)
		{
			LogTag tags[] = { tag... };
			for (auto t : tags) show.set(t, always);
		}

	private:

		static const std::size_t n_tag = 7;

		std::bitset<n_tag> show;
		std::deque<std::pair<std::bitset<n_tag>, std::string>> logs;
		std::array<std::string, n_tag> log_per_tag;
	};

	class ECSManagers
	{
	public:

		EntityManager entity_manager;
		SystemManager systemManager;
		EventManager event_manager;

		ECSManagers()
		{
			entity_manager = EntityManager();
			event_manager = EventManager(&entity_manager);
			systemManager = SystemManager(&entity_manager, &event_manager);
		}

		void update_ecs_managers()
		{
			entity_manager.update();
			systemManager.update();
		}
	};

	Logger logger;
}