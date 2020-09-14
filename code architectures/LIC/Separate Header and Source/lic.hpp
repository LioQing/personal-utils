#pragma once

#include <iostream>
#include <array>
#include <memory>
#include <vector>
#include <bitset>
#include <algorithm>
#include <tuple>
#include <cstdint>
#include <concepts>

namespace lic
{
	// units
	using ComponentID = uint32_t;
	using EntityID = uint32_t;

	constexpr ComponentID MAX_COMPONENT = 32u;

	// forward declaration
	class Entity;
	class Component;

	// concepts
	template <typename T>
	concept IsComponent = std::is_base_of<Component, T>::value;

	// ...also forward declaration... with concepts
	template <IsComponent ...Ts>
	class View;


	/*-------------Manager--------------*/


	class Manager
	{
	private:

		// list of list of components
		std::array<std::vector<std::unique_ptr<Component>>, MAX_COMPONENT> m_components;

		// list of removed components
		std::array<std::vector<size_t>, MAX_COMPONENT> m_empty_component;

		// currently the highest entity id
		EntityID m_top_id;

		// list of removed entities
		std::vector<EntityID> m_empty_entity;

		// component checklist for entities
		std::vector<std::bitset<MAX_COMPONENT>> m_checklist;

		// next id for entity
		EntityID m_next_entity_id = 0u;

		// next id for component
		// mutable for const GetComponentID<T>() function
		mutable ComponentID m_next_component_id = 0u;

	public:

		// add entity
		Entity AddEntity();

		// destroy entity
		void DestroyEntity(EntityID entity);

		// get entity
		Entity GetEntity(EntityID entity);

		// get component id
		template <typename T>
		ComponentID GetComponentID() const
		{
			static ComponentID id = m_next_component_id++;
			return id;
		}

		// add component to entity
		template <IsComponent T, typename ...TArgs>
		T& AddComponent(EntityID entity, TArgs&& ...args)
		{
			if (HasComponent<T>(entity))
			{
#ifdef LIC_DEBUG
				std::cout << "Component " << typeid(T).name() << " already in Entity " << entity << "." << std::endl;
#endif
				return GetComponent<T>(entity);
			}

			T* cptr(new T(std::forward<TArgs>(args)...));
			cptr->entity = entity;
			cptr->manager = this;
			auto& ref = *cptr;

			ComponentID id = GetComponentID<T>();
			if (m_empty_component.at(id).empty())
			{
				std::unique_ptr<Component> u_ptr{ cptr };
				m_components.at(id).push_back(std::move(u_ptr));
			}
			else
			{
				m_components.at(id).at(m_empty_component.at(id).back()).reset(cptr);
				m_empty_component.at(id).pop_back();
			}

			m_checklist.at(entity).set(id);

#ifdef LIC_DEBUG
			std::cout << "Component " << typeid(T).name() << " added to Entity " << entity << "." << std::endl;
#endif
			return ref;
		}

		// remove component
		template <IsComponent T>
		void RemoveComponent(EntityID entity)
		{
			if (!HasComponent(entity, GetComponentID<T>()))
			{
#ifdef LIC_DEBUG
				std::cout << "No Component " << GetComponentID<T>() << " found in Entity " << entity << "." << std::endl;
#endif
				return;
			}

			for (auto& cptr : m_components.at(GetComponentID<T>()))
			{
				if (cptr != nullptr && cptr->entity == entity)
				{
					m_empty_component.at(GetComponentID<T>()).push_back(&cptr - &m_components.at(GetComponentID<T>())[0]);
					m_checklist.at(cptr->entity).set(GetComponentID<T>(), false);
					cptr.reset();
					break;
				}
			}

#ifdef LIC_DEBUG
			std::cout << "Component " << typeid(T).name() << " removed from Entity " << entity << "." << std::endl;
#endif
		}

		// remove component with component id
		// defined after entity class definition
		void RemoveComponent(EntityID entity, ComponentID cid);

		// get component
		template <IsComponent T>
		T& GetComponent(EntityID entity)
		{
			for (auto& cptr : m_components.at(GetComponentID<T>()))
			{
				if (cptr != nullptr && cptr->entity == entity)
					return *static_cast<T*>(cptr.get());
			}
		}
		template <IsComponent T>
		const T& GetComponent(EntityID entity) const
		{
			for (auto& cptr : m_components.at(GetComponentID<T>()))
			{
				if (cptr != nullptr && cptr->entity == entity)
					return *static_cast<T*>(cptr.get());
			}
		}

		// has component
		template <IsComponent T>
		bool HasComponent(EntityID entity) const
		{
			return m_checklist.at(entity).test(GetComponentID<T>());
		}
		// has component with component id
		bool HasComponent(EntityID entity, ComponentID cid) const;

	private:

		// filter out component U from view
		template <IsComponent U, IsComponent T, IsComponent ...Ts>
		void _ProcessMultiFilter(View<T, Ts...>& view, bool include_non_active)
		{
			std::erase_if(view.m_components.m_vec,
						  [](T*& c) -> bool
				{ 
					if (!c->GetEntity().HasComponent<U>())
						return true;
					else if (!c->GetEntity().GetComponent<U>().is_active)
						return true;
					else
						return false;
				}
			);
		}

	public:

		// component filter
		template <IsComponent T, IsComponent ...Ts>
		View<T, Ts...> Filter(bool include_non_active = false)
		{
			View<T, Ts...> view(*this);

			// get vector of component T
			for (auto& cptr : m_components.at(GetComponentID<T>()))
			{
				if (cptr == nullptr || !include_non_active && !cptr->is_active) 
					continue;

				view.m_components.m_vec.push_back(static_cast<T*>(cptr.get()));
			}

			// filter out components Ts...
			int process[] = { 0, (_ProcessMultiFilter<Ts>(view, include_non_active), 0)... };
			(void)process;

			// push entities
			for (auto& c : view.m_components)
				view.m_entities.m_vec.push_back(c.GetEntityID());

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

		virtual ~Component() = default;

		// active
		bool is_active = true;

		// get entity
		Entity GetEntity();
		Entity GetEntity() const;

		// get entity id
		EntityID GetEntityID() const;
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

		Entity() = default;
		Entity(Manager* manager, EntityID id)
			: manager(manager), id(id) {}

		// conversion to id
		operator EntityID() const;

		// get entity id
		EntityID GetID() const;

		// destroy this entity
		void Destroy();

		// add component
		template <IsComponent T, typename ...TArgs>
		T& AddComponent(TArgs&& ...args)
		{
			return manager->AddComponent<T>(id, std::forward<TArgs>(args)...);
		}

		// remove component
		template <IsComponent T>
		void RemoveComponent()
		{
			manager->RemoveComponent<T>(id);
		}

		// remove component with component id
		void RemoveComponent(ComponentID cid);

		// get component
		template <IsComponent T>
		T& GetComponent()
		{
			return manager->GetComponent<T>(id);
		}
		template <IsComponent T>
		const T& GetComponent() const
		{
			return manager->GetComponent<T>(id);
		}

		// has component
		template <IsComponent T>
		bool HasComponent() const
		{
			return manager->HasComponent<T>(id);
		}

		// has component with component id
		bool HasComponent(ComponentID cid) const;
	};


	/*-----------CONTAINERS------------*/


	//ccontainer iterator
	template <IsComponent T>
	class CContainerItr
	{
	private:

		// index for iteration
		size_t m_index;

		// vec ref
		std::vector<T*>& m_vec;

	public:

		CContainerItr(std::vector<T*>& vec, const size_t index = 0u)
			: m_index(index), m_vec(vec) {}

		// operators for range-based for loop
		bool operator!=(const CContainerItr& itr) const
		{
			return m_index != itr.m_index;
		}

		const CContainerItr& operator++()
		{
			++m_index;
			return *this;
		}

		T& operator*() const
		{
			return *m_vec.at(m_index);
		}
	};

	// component container
	template <IsComponent T, IsComponent ...Ts>
	class CContainer
	{
	private:

		friend class View<T, Ts...>;
		friend class View<Ts...>;
		friend class Manager;

		//backing vector
		std::vector<T*> m_vec;

	public:

		CContainer() = default;

		// begin and end methods for iterator
		auto begin()
		{
			return CContainerItr<T>(m_vec, 0);
		}
		auto end()
		{
			return CContainerItr<T>(m_vec, m_vec.size());
		}
	};

	// econtainer iterator
	template <typename T, IsComponent ...Ts>
	class EContainerItr
	{
	private:

		Manager& manager;

		// index for iteration
		size_t m_index;

		// vec ref
		std::vector<EntityID>& m_vec;

	public:

		EContainerItr(Manager& manager, std::vector<EntityID>& vec, const size_t index)
			: manager(manager), m_index(index), m_vec(vec) {}

		// operators for range-based for loop
		bool operator!=(const EContainerItr& itr) const
		{
			return m_index != itr.m_index;
		}

		const EContainerItr& operator++()
		{
			++m_index;
			return *this;
		}

		auto operator*() const
		{
			if constexpr (std::is_same<T, std::tuple<Ts...>>::value == true)
				return std::tie(manager.GetComponent<Ts>(m_vec.at(m_index))...);
			if constexpr (std::is_same<T, Entity>::value == true)
				return Entity(&manager, m_vec.at(m_index));
		}
	};

	// entity container
	template <typename T, IsComponent ...Ts>
	class EContainer
	{
	private:

		friend class View<Ts...>;
		friend class Manager;

		Manager& manager;

		// backing vector
		std::vector<EntityID> m_vec;

	public:

		EContainer(Manager& manager) : manager(manager) {}
		EContainer(Manager& manager, std::vector<EntityID> vec)
			: manager(manager), m_vec(vec) {}

		// begin and end methods for iterator
		auto begin()
		{
			return EContainerItr<T, Ts...>(manager, m_vec, 0u);
		}
		auto end()
		{
			return EContainerItr<T, Ts...>(manager, m_vec, m_vec.size());
		}
	};


	/*----------VIEW----------*/


	template <IsComponent ...Ts>
	class View
	{
	private:

		friend class Manager;

		Manager& manager;

		// entities and component T
		EContainer<Entity, Ts...> m_entities;
		CContainer<Ts...> m_components;

		// filter out non-component U from view
		template <IsComponent U, IsComponent S, IsComponent ...Ss>
		void _ProcessMultiFilterOut(View<S, Ss...>& view) const
		{
			std::erase_if(view.m_components.m_vec,
						  [](S*& c) -> bool
						  { return c->GetEntity().HasComponent<U>(); }
			);
		}

	public:

		View(Manager& manager) : manager(manager), m_entities(manager) {}

		// filter out component
		template <IsComponent ...Us>
		View<Ts...> FilterOut()
		{
			int process[] = { 0, (_ProcessMultiFilterOut<Us, Ts...>(*this), 0)... };
			(void)process;

			m_entities.m_vec.clear();
			for (auto& c : m_components)
				m_entities.m_vec.push_back(c.GetEntityID());

			return *this;
		}
		template <IsComponent ...Us>
		View<Ts...> FilterOut() const
		{
			View<Ts...> view(*this);

			int process[] = { 0, (_ProcessMultiFilterOut<Us, Ts...>(view), 0)... };
			(void)process;

			view.m_entities.m_vec.clear();
			for (auto& c : view.m_components)
				view.m_entities.m_vec.push_back(c.GetEntityID());

			return view;
		}

		// begin and end methods for entities iterator
		auto begin() const
		{
			return m_entities.begin();
		}
		auto end() const
		{
			return m_entities.end();
		}

		// get component T
		auto Component() const
		{
			return m_components;
		}

		// get entity
		auto Entity() const
		{
			return m_entities;
		}

		// get each component T, Ts...
		EContainer<std::tuple<Ts...>, Ts...> Each() const
		{
			return EContainer<std::tuple<Ts...>, Ts...>(manager, m_entities.m_vec);
		}
	};
}