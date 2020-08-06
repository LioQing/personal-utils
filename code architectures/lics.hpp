#pragma once

#include <array>
#include <memory>
#include <vector>
#include <bitset>
#include <algorithm>
#include <cstdint>

namespace lics
{
	using ComponentID = uint32_t;
	using EntityID = uint32_t;

	constexpr ComponentID MAX_COMPONENT = 32u;

	// forward declaration
	class Entity;
	class Component;
	template <typename T>
	class View;

	class Manager
	{
	private:

		// list of list of components
		std::array<std::vector<std::unique_ptr<Component>>, MAX_COMPONENT> m_components;

		// list of entities
		std::vector<std::unique_ptr<Entity>> m_entities;

		// component check list for entities
		std::vector<std::bitset<MAX_COMPONENT>> m_checklist;

		// next id for entity and component
		EntityID m_next_entity_id = 0u;
		ComponentID m_next_component_id = 0u;

	public:

		// add entity
		Entity& AddEntity()
		{
			auto id = m_next_entity_id++;
			m_entities.push_back(std::make_unique<Entity>(this, id));
			m_checklist.emplace_back(false);
			return *m_entities.at(id).get();
		}

		// get entity
		Entity& GetEntity(EntityID entity) const
		{
			return *m_entities.at(entity).get();
		}

		// get component id
		template <typename T>
		ComponentID GetComponentID()
		{
			static ComponentID id = m_next_component_id++;
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

			m_components.at(GetComponentID<T>()).push_back(std::move(u_ptr));
			m_checklist.at(entity).set(GetComponentID<T>());
			return ref;
		}

		// get component
		template <typename T>
		T& GetComponent(EntityID entity)
		{
			for (auto& cptr : m_components.at(GetComponentID<T>()))
			{
				if (cptr->entity == entity)
					return *static_cast<T*>(cptr.get());
			}
		}

		// has component
		template <typename T>
		bool HasComponent(EntityID entity)
		{
			return m_checklist.at(entity)[GetComponentID<T>()];
		}

	private:

		template <typename U, typename T>
		void _ProcessMultiFilter(View<T>& view)
		{
			std::erase_if(view.m_components.m_vec, 
						  [](T*& c) -> bool 
						  { return !c->GetEntity().HasComponent<U>(); }
			);
		}

	public:

		// component filter
		template <typename T, typename ...Ts>
		View<T> Filter()
		{
			View<T> view;
			for (auto& c : m_components.at(GetComponentID<T>()))
				view.m_components.m_vec.push_back(static_cast<T*>(c.get()));

			int process[] = { 0, (_ProcessMultiFilter<Ts>(view), 0)... };
			(void)process;

			for (auto& c : view.m_components)
				view.m_entities.m_vec.push_back(&c.GetEntity());

			return view;
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

		// get entity
		Entity& GetEntity() const
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
		T& GetComponent() const
		{
			return manager->GetComponent<T>(id);
		}

		// has component
		template <typename T>
		bool HasComponent() const
		{
			return manager->HasComponent<T>(id);
		}
	};

	template <typename T>
	class Container;

	template <typename T>
	class ContainerIteratorType
	{
	private:

		size_t m_index;
		Container<T>& m_container;

	public:

		ContainerIteratorType(Container<T>& container, const size_t index)
			: m_index(index), m_container(container) {}

		// operators for range-based for loop
		bool operator!=(const ContainerIteratorType& itr) const
		{
			return m_index != itr.m_index;
		}

		const ContainerIteratorType& operator++()
		{
			++m_index;
			return *this;
		}

		T& operator*() const
		{
			return *((std::vector<T*>)m_container).at(m_index);;
		}
	};

	// alias
	template <typename T>
	using ContainerIterator = ContainerIteratorType<T>;
	template <typename T>
	using ConstContainerIterator = ContainerIteratorType<const T>;

	template <typename T>
	class Container
	{
	private:

		friend class Manager;

		// the backing vector of T
		std::vector<T*> m_vec;

	public:

		operator std::vector<T*>()
		{
			return m_vec;
		}

		// begin and end methods for iterator
		ContainerIterator<T> begin()
		{
			return ContainerIterator<T>(*this, 0);
		}
		ContainerIterator<T> end()
		{
			return ContainerIterator<T>(*this, m_vec.size());
		}
		ConstContainerIterator<T> begin() const
		{
			return ConstContainerIterator<T>(*this, 0);
		}
		ConstContainerIterator<T> end() const
		{
			return ConstContainerIterator<T>(*this, m_vec.size());
		}
	};

	template <typename T>
	class View
	{
	private:

		friend class Manager;

		// entities and component T
		Container<Entity> m_entities;
		Container<T> m_components;

	public:

		// begin and end methods for entities iterator
		ContainerIterator<Entity> begin()
		{
			return m_entities.begin();
		}
		ContainerIterator<Entity> end()
		{
			return m_entities.end();
		}
		ConstContainerIterator<Entity> begin() const
		{
			return m_entities.begin();
		}
		ConstContainerIterator<Entity> end() const
		{
			return m_entities.end();
		}

		// get component T
		Container<T> Components() const
		{
			return m_components;
		}
	};
}