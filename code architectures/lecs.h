#pragma once

#include <cstdint>
#include <memory>
#include <array>
#include <vector>
#include <bitset>

namespace lecs
{
	class EntityManager;

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

		EntityManager& entityManager;
		bool active = true;

	public:

		uint32_t id;
		std::vector<std::unique_ptr<Component>> components;

		std::array<Component*, Max_Component> componentArray;
		std::bitset<Max_Component> componentBitSet;

		Entity(EntityManager& eMan, uint32_t eID) : entityManager(eMan), id(eID) {}

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

	class EntityContainer
	{
	private:

		EntityManager& entityManager;

	public:

		std::vector<Entity*> entities;

		EntityContainer(EntityManager& eMan) : entityManager(eMan) {}

		template <typename T>
		EntityContainer With();
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
				if (!e->IsActive()) delete e.release();
			}
		}

		Entity& AddEntity()
		{
			Entity* e(new Entity(*this, nextID++));

			std::unique_ptr<Entity> uPtr{ e };
			entities.resize(entities.size() + 1);
			entities.at(e->id) = std::move(uPtr);

			return *e;
		}

		template <typename T>
		EntityContainer With()
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

	template <typename T>
	EntityContainer EntityContainer::With()
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

	class System
	{
	public:

		virtual void Update(EntityManager* entityManager) {}
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

	class ECSManager
	{
	public:

		EntityManager* entityManager;
		SystemManager* systemManager;

		ECSManager()
		{
			entityManager = new EntityManager();
			systemManager = new SystemManager(entityManager);
		}

		void UpdateECSManagers()
		{
			entityManager->Update();
			systemManager->Update();
		}
	};
}