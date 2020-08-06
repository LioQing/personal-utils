#pragma once

#include <array>
#include <memory>
#include <vector>
#include <cstdint>

namespace lics
{
	using ComponentID = uint32_t;
	using EntityID = uint32_t;

	constexpr ComponentID MAX_COMPONENT = 32u;

	// forward declaration
	class Entity;
	class Component;

	class Manager
	{
	private:

		// list of list of components
		std::array<std::vector<std::unique_ptr<Component>>, MAX_COMPONENT> components;

		// list of entities
		std::vector<std::unique_ptr<Entity>> entities;

		// next id for entity and component
		EntityID next_entity_id = 0u;
		ComponentID next_component_id = 0u;

	public:

		// add entity
		Entity& AddEntity()
		{
			auto id = next_entity_id++;
			entities.push_back(std::make_unique<Entity>(this, id));
			return *entities.at(id).get();
		}

		// get entity
		Entity& GetEntity(EntityID entity)
		{
			return *entities.at(entity).get();
		}

		// get entity from component
		template <typename T>
		Entity& GetEntity(const T& c)
		{
			return *entities.at(c.entity).get();
		}

		// get component id
		template <typename T>
		ComponentID GetComponentID()
		{
			static ComponentID id = next_component_id++;
			return id;
		}

		// add component to entity
		template <typename T, typename ...TArgs>
		T& AddComponent(EntityID entity, TArgs&& ...args)
		{
			T* c(new T(std::forward<TArgs>(args)...));
			c->entity = entity;
			c->manager = this;

			std::unique_ptr<Component> u_ptr{ c };
			auto& ref = *c;

			components.at(GetComponentID<T>()).push_back(std::move(u_ptr));
			return ref;
		}

		// get component
		template <typename T>
		T& GetComponent(EntityID entity)
		{
			for (auto& cptr : components.at(GetComponentID<T>()))
			{
				if (cptr->entity == entity)
				{
					return *static_cast<T*>(cptr.get());
				}
			}
		}
	};

	class Component
	{
	private:

		friend class Manager;

		// related manager
		Manager* manager;

		// id
		EntityID entity;

	public:

		virtual ~Component() {}

		Entity& GetEntity()
		{
			return manager->GetEntity(entity);
		}
	};

	class Entity
	{
	private:

		friend class Manager;

		// related manager
		Manager* manager;

		// id
		EntityID id;

	public:

		Entity(Manager* manager, EntityID id)
			: manager(manager), id(id) {}

		// get entity id
		EntityID GetID() const
		{
			return id;
		}

		// add component
		template <typename T, typename ...TArgs>
		T& AddComponent(TArgs&& ...args)
		{
			return manager->AddComponent<T>(id, std::forward<TArgs>(args)...);
		}

		// get component
		template <typename T>
		T& GetComponent()
		{
			return manager->GetComponent<T>(id);
		}
	};
}