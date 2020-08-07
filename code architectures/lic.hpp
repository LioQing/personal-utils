#pragma once

#include <array>
#include <memory>
#include <vector>
#include <bitset>
#include <algorithm>
#include <tuple>
#include <cstdint>

namespace lic
{
	// units
	using ComponentID = uint32_t;
	using EntityID = uint32_t;

	constexpr ComponentID MAX_COMPONENT = 32u;

	// forward declaration
	class Entity;
	class Component;
	template <typename ...Ts>
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

		template <typename U, typename T, typename ...Ts>
		void _ProcessMultiFilter(View<T, Ts...>& view)
		{
			std::erase_if(view.m_components.m_vec, 
						  [](T*& c) -> bool 
						  { return !c->GetEntity().HasComponent<U>(); }
			);
		}

	public:

		// component filter
		template <typename T, typename ...Ts>
		View<T, Ts...> Filter()
		{
			View<T, Ts...> view;
			for (auto& c : m_components.at(GetComponentID<T>()))
				view.m_components.m_vec.push_back(static_cast<T*>(c.get()));

			int process[] = { 0, (_ProcessMultiFilter<Ts>(view), 0)... };
			(void)process;

			for (auto& c : view.m_components)
				view.m_entities.m_vec.push_back(&c.GetEntity());

			return view;
		}
	};


	/*--------COMPONENT----------*/


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


	/*-------------ENTITY-------------*/


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
		T& AddComponent(TArgs&& ...args) const
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


	/*-----------CONTAINERS------------*/


	template <typename T, typename ...Ts>
	class Container
	{
	private:

		friend class View<Ts...>;
		friend class Manager;

		size_t m_index;
		std::vector<T*> m_vec;

	public:

		Container() {}
		Container(std::vector<T*> vec, const size_t index = 0)
			: m_index(index), m_vec(vec) {}

		// operators for range-based for loop
		bool operator!=(const Container& itr) const
		{
			return m_index != itr.m_index;
		}

		const Container& operator++()
		{
			++m_index;
			return *this;
		}

		T& operator*() const
		{
			return *m_vec.at(m_index);
		}

		// begin and end methods for iterator
		Container<T, Ts...> begin()
		{
			return Container<T, Ts...>(m_vec, 0);
		}
		Container<T, Ts...> end()
		{
			return Container<T, Ts...>(m_vec, m_vec.size());
		}
	};

	// tuple list
	template <typename ...Ts>
	class TupleList
	{
	private:

		size_t m_index;
		std::vector<Entity*> m_vec;

	public:

		TupleList() {}
		TupleList(std::vector<Entity*> vec, const size_t index = 0)
			: m_index(index), m_vec(vec) {}

		// operators for range-based for loop
		bool operator!=(const TupleList& itr) const
		{
			return m_index != itr.m_index;
		}

		const TupleList& operator++()
		{
			++m_index;
			return *this;
		}

		std::tuple<Ts&...> operator*() const
		{
			return std::tie(m_vec.at(m_index)->GetComponent<Ts>()...);
		}

		// begin and end methods for iterator
		TupleList<Ts...> begin()
		{
			return TupleList<Ts...>(m_vec, 0);
		}
		TupleList<Ts...> end()
		{
			return TupleList<Ts...>(m_vec, m_vec.size());
		}
	};


	/*----------VIEW----------*/


	template <typename ...Ts>
	class View
	{
	public:

		friend class Manager;

		// entities and component T
		Container<Entity, Ts...> m_entities;
		Container<Ts...> m_components;

	public:

		View() {}

		// begin and end methods for entities iterator
		Container<Entity> begin()
		{
			return m_entities.begin();
		}
		Container<Entity> end()
		{
			return m_entities.end();
		}

		// get component T
		Container<Ts...> Component() const
		{
			return m_components;
		}

		// get each component T, Ts...
		TupleList<Ts...> Each()
		{
			return TupleList<Ts...>(m_entities.m_vec);
		}
	};
}