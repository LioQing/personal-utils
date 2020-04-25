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

	class ComponentManager
	{
	public:

		template <typename T>
		static std::size_t GetComponentTypeID()
		{
			static std::size_t id = nextComponentID++;
			return id;
		}
	};

	class Entity
	{
	private:

		bool active = true;

	public:

		uint32_t id;
		std::vector<std::unique_ptr<Component>> components;

		std::array<Component*, Max_Component> componentArray;
		std::bitset<Max_Component> componentBitSet;

		bool IsActive()
		{
			return active;
		}
		void Destroy()
		{
			active = false;
		}

		template <typename T, typename... TArgs>
		T& AddComponent(TArgs&&... mArgs)
		{
			T* c(new T(std::forward<TArgs>(mArgs)...));
			c->entity = this->id;
			std::unique_ptr<Component> uPtr{ c };
			components.emplace_back(std::move(uPtr));

			componentArray[ComponentManager::GetComponentTypeID<T>()] = c;
			componentBitSet[ComponentManager::GetComponentTypeID<T>()] = true;

			return *c;
		}

		template <typename T> 
		T& GetComponent() const
		{
			auto ptr(componentArray[ComponentManager::GetComponentTypeID<T>()]);
			return *static_cast<T*>(ptr);
		}

		template <typename T>
		bool HasComponent()
		{
			return componentBitSet[ComponentManager::GetComponentTypeID<T>()];
		}
	};

	class EntityManager
	{
	private:

		uint32_t nextID;

	public:

		std::vector<std::unique_ptr<Entity>> entities;

		Entity& AddEntity()
		{
			Entity* e(new Entity());
			e->id = nextID++;
			std::unique_ptr<Entity> uPtr{ e };
			entities[e->id] = std::move(uPtr);

			return *e;
		}

		template <typename T>
		std::vector<Entity*> With()
		{
			std::vector<Entity*> entitiesWith;
			for (auto& e : entities)
			{
				uint32_t id = e->id;
				if (e->HasComponent<T>()) entitiesWith.emplace_back(entities[id].get());
			}
			return entitiesWith;
		}
	};

	class System
	{
	public:

		virtual void Update(EntityManager* eMan) {}
	};

	class SystemManager
	{
	private:

		EntityManager* eManager;

	public:

		std::vector<std::unique_ptr<System>> systems;

		SystemManager(EntityManager* eMan) : eManager(eMan) {}

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
				s->Update(eManager);
			}
		}
	};
}