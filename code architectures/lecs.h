#pragma once

#include <cstdint>
#include <memory>
#include <array>
#include <vector>
#include <bitset>

namespace lecs
{
	constexpr std::size_t Max_Component = 32;

	class Component
	{
	public:

		uint32_t entity;
	};

	std::size_t nextComponentID = 0;

	template <typename T>
	inline std::size_t GetComponentTypeID()
	{
		static std::size_t id = nextComponentID++;
		return id;
	}

	class EntityManager;

	class Entity
	{
	private:

		EntityManager& entityManager;
		bool active = true;

	public:

		uint32_t id;
		std::vector<std::unique_ptr<Component>> components;

		std::array<Component*, Max_Component> componentArray;
		std::bitset<Max_Component> componentBitSet;

		Entity(EntityManager& entityManager, uint32_t eID) : entityManager(entityManager), id(eID) {}

		bool IsActive()
		{
			return active;
		}
		void Destroy(bool immediate = false);

		template <typename T>
		T& AddComponent(std::unique_ptr<T> uPtr)
		{
			uPtr->entity = id;
			components.emplace_back(std::move(uPtr));

			componentArray[GetComponentTypeID<T>()] = uPtr.get();
			componentBitSet[GetComponentTypeID<T>()] = true;

			return *uPtr.get();
		}

		template <typename T>
		T& AddComponent(const T& c)
		{
			c->entity = id;
			std::unique_ptr<Component> uPtr{ c };
			components.emplace_back(std::move(uPtr));

			componentArray[GetComponentTypeID<T>()] = c;
			componentBitSet[GetComponentTypeID<T>()] = true;

			return *c;
		}

		template <typename T, typename... TArgs>
		T& AddComponent(TArgs&&... mArgs)
		{
			T* c(new T(std::forward<TArgs>(mArgs)...));
			c->entity = id;
			std::unique_ptr<Component> uPtr{ c };
			components.emplace_back(std::move(uPtr));

			componentArray[GetComponentTypeID<T>()] = c;
			componentBitSet[GetComponentTypeID<T>()] = true;

			return *c;
		}

		template <typename T>
		T& GetComponent() const
		{
			auto ptr(componentArray[GetComponentTypeID<T>()]);
			return *static_cast<T*>(ptr);
		}

		template <typename T>
		bool HasComponent()
		{
			return componentBitSet[GetComponentTypeID<T>()];
		}
	};

	class EntityContainer
	{
	private:

		EntityManager& entityManager;

	public:

		std::vector<Entity*> entities;

		EntityContainer(EntityManager& entityManager) : entityManager(entityManager) {}

		template <typename T>
		EntityContainer Entities();
	};

	class EntityManager
	{
	private:

		uint32_t nextID;

	public:

		std::vector<std::unique_ptr<Entity>> entities;
		std::vector<uint32_t> emptyID;

		void Update()
		{
			for (auto& e : entities)
			{
				if (!e) continue;
				if (!e->IsActive())
				{
					emptyID.push_back(e->id);
					delete e.release();
				}
			}
		}

		void ImmediatelyDestroy(uint32_t id)
		{
			emptyID.push_back(id);
			delete entities.at(id).release();
		}

		Entity& AddEntity()
		{
			uint32_t nID;
			bool isEmpty = emptyID.empty();
			if (!isEmpty)
			{
				nID = emptyID.back();
				emptyID.pop_back();
			}
			else
			{
				nID = nextID++;
			}

			Entity* e(new Entity(*this, nID));
			std::unique_ptr<Entity> uPtr{ e };
			if (isEmpty) entities.resize(entities.size() + 1);
			entities.at(e->id) = std::move(uPtr);

			return *e;
		}

		EntityContainer Entities()
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
		EntityContainer Entities()
		{
			EntityContainer entitiesWith = EntityContainer(*this);
			for (auto& e : entities)
			{
				if (!e) continue;
				uint32_t id = e->id;
				if (e->HasComponent<T>()) entitiesWith.entities.emplace_back(entities.at(id).get());
			}
			return entitiesWith;
		}
	};

	void Entity::Destroy(bool immediate)
	{
		active = false;
		if (immediate) entityManager.ImmediatelyDestroy(id);
	}

	template <typename T>
	EntityContainer EntityContainer::Entities()
	{
		{
			EntityContainer entitiesWith = EntityContainer(entityManager);
			for (auto& e : entities)
			{
				if (!e) continue;
				uint32_t id = e->id;
				if (e->HasComponent<T>()) entitiesWith.entities.emplace_back(entityManager.entities.at(id).get());
			}
			return entitiesWith;
		}
	}

	class Event;
	class EventManager;

	class EventSubscriber
	{
	public:

		std::vector<std::size_t> subscribed;

		virtual void Receive(Event* event, EventManager* eventManager) {}
	};

	class Event
	{
	public:

		EventManager* eventManager;
		std::size_t id;
		std::vector<EventSubscriber*> subscribers;

		template <typename T>
		bool IsEvent();
	};

	class EventManager
	{
	private:

		EntityManager* entityManager;
		std::size_t nextEventID = 0;

	public:

		std::vector<std::unique_ptr<Event>> events;

		EventManager() : entityManager(nullptr) {}
		EventManager(EntityManager* entityManager) : entityManager(entityManager) {}

		template <typename T>
		inline std::size_t GetEventID()
		{
			static std::size_t id = nextEventID++;
			return id;
		}

		template <typename T>
		void Unsubscribe(EventSubscriber* subscriber)
		{
			AddEvent<T>();
			std::size_t id = GetEventID<T>();
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
		void Subscribe(EventSubscriber* subscriber)
		{
			AddEvent<T>();
			events.at(GetEventID<T>())->subscribers.emplace_back(subscriber);
			subscriber->subscribed.emplace_back(GetEventID<T>());
		}

		template <typename T, typename... TArgs>
		void Emit(TArgs... aArgs)
		{
			for (auto& sub : events.at(GetEventID<T>())->subscribers)
			{
				T* ev(new T(std::forward<TArgs>(aArgs)...));
				ev->eventManager = this;
				ev->id = GetEventID<T>();
				sub->Receive(ev, this);
			}
		}

		template <typename T>
		void AddEvent()
		{
			if (GetEventID<T>() < events.size()) return;
			T* ev(new T());
			ev->id = GetEventID<T>();
			std::unique_ptr<T> uPtr{ ev };
			events.resize(events.size() + 1);
			events.at(ev->id) = std::move(uPtr);
		}
	};

	template <typename T>
	bool Event::IsEvent()
	{
		return id == eventManager->GetEventID<T>();
	}

	class System
	{
	public:

		virtual void Update(EntityManager* entityManager, EventManager* eventManager) {}
	};

	class SystemManager
	{
	private:

		EntityManager* entityManager;
		EventManager* eventManager;

	public:

		std::vector<std::unique_ptr<System>> systems;

		SystemManager() : entityManager(nullptr), eventManager(nullptr) {}
		explicit SystemManager(EntityManager* entityManager, EventManager* eventManager) 
			: entityManager(entityManager), eventManager(eventManager) {}

		template <typename T>
		T& AddSystem(const T& s)
		{
			std::unique_ptr<System> uPtr{ s };
			systems.emplace_back(std::move(uPtr));

			return *s;
		}

		template <typename T>
		T& AddSystem(std::unique_ptr<T> uPtr)
		{
			systems.emplace_back(std::move(uPtr));
			return *uPtr.get();
		}

		template <typename T, typename... TArgs>
		T& AddSystem(TArgs&&... aArgs)
		{
			T* s(new T(std::forward<TArgs>(aArgs)...));
			std::unique_ptr<System> uPtr{ s };
			systems.emplace_back(std::move(uPtr));

			return *s;
		}

		void Update()
		{
			for (auto& s : systems)
			{
				s->Update(entityManager, eventManager);
			}
		}
	};

	class ECSManagers
	{
	public:

		EntityManager entityManager;
		SystemManager systemManager;
		EventManager eventManager;

		ECSManagers()
		{
			entityManager = EntityManager();
			eventManager = EventManager(&entityManager);
			systemManager = SystemManager(&entityManager, &eventManager);
		}

		void UpdateECSManagers()
		{
			entityManager.Update();
			systemManager.Update();
		}
	};
}