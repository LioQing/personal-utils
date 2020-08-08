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


	/*-------------Manager--------------*/


	class Manager
	{
	private:

		// list of list of components
		std::array<std::vector<std::unique_ptr<Component>>, MAX_COMPONENT> m_components;

		// list of removed components
		std::array<std::vector<size_t>, MAX_COMPONENT> m_empty_component;

		// list of entities
		std::vector<std::unique_ptr<Entity>> m_entities;

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
		Entity& AddEntity()
		{
			EntityID id;

			if (m_empty_entity.empty())
			{
				id = m_next_entity_id++;
				m_entities.push_back(std::make_unique<Entity>(this, id));
				m_checklist.emplace_back(false);
			}
			else
			{
				id = m_empty_entity.back();
				m_entities.at(id) = std::make_unique<Entity>(this, id);
			}

#ifdef LIC_DEBUG
			std::cout << "Entity " << id << " created." << std::endl;
#endif
			return *m_entities.at(id).get();
		}

		// destroy entity
		void DestroyEntity(EntityID entity)
		{
			// remove components
			for (ComponentID cid = 0u; cid < MAX_COMPONENT; ++cid)
			{
				if (HasComponent(entity, cid))
					RemoveComponent(entity, cid);
			}

			// reset checklist
			m_checklist.at(entity).reset();

			// destroy entity
			m_empty_entity.push_back(entity);
			m_entities.at(entity).reset();

#ifdef LIC_DEBUG
			std::cout << "Entity " << entity << " destroyed." << std::endl;
#endif
		}

		// get entity
		Entity& GetEntity(EntityID entity)
		{
			return *m_entities.at(entity).get();
		}
		const Entity& GetEntity(EntityID entity) const
		{
			return *m_entities.at(entity).get();
		}

		// get component id
		template <typename T>
		ComponentID GetComponentID() const
		{
			static ComponentID id = m_next_component_id++;
			return id;
		}

		// add component to entity
		template <typename T, typename ...TArgs>
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
		template <typename T>
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
		template <typename T>
		T& GetComponent(EntityID entity)
		{
			for (auto& cptr : m_components.at(GetComponentID<T>()))
			{
				if (cptr != nullptr && cptr->entity == entity)
					return *static_cast<T*>(cptr.get());
			}
		}
		template <typename T>
		const T& GetComponent(EntityID entity) const
		{
			for (auto& cptr : m_components.at(GetComponentID<T>()))
			{
				if (cptr != nullptr && cptr->entity == entity)
					return *static_cast<T*>(cptr.get());
			}
		}

		// has component
		template <typename T>
		bool HasComponent(EntityID entity) const
		{
			return m_checklist.at(entity).test(GetComponentID<T>());
		}
		// has component with component id
		bool HasComponent(EntityID entity, ComponentID cid) const
		{
			return m_checklist.at(entity).test(cid);
		}

	private:

		// filter out component U from view
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

			// get vector of component T
			for (auto& cptr : m_components.at(GetComponentID<T>()))
			{
				if (cptr == nullptr) continue;
				view.m_components.m_vec.push_back(static_cast<T*>(cptr.get()));
			}

			// filter out components Ts...
			int process[] = { 0, (_ProcessMultiFilter<Ts>(view), 0)... };
			(void)process;

			// push entities
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

		virtual ~Component() = default;

		// get entity
		Entity& GetEntity()
		{
			return manager->GetEntity(entity);
		}
		const Entity& GetEntity() const
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

		// conversion to id
		operator EntityID() const
		{
			return id;
		}

		// get entity id
		EntityID GetID() const
		{
			return id;
		}

		// destroy this entity
		void Destroy()
		{
			manager->DestroyEntity(id);
		}

		// add component
		template <typename T, typename ...TArgs>
		T& AddComponent(TArgs&& ...args)
		{
			return manager->AddComponent<T>(id, std::forward<TArgs>(args)...);
		}

		// remove component
		template <typename T>
		void RemoveComponent()
		{
			manager->RemoveComponent<T>(id);
		}

		// remove component with component id
		void RemoveComponent(ComponentID cid)
		{
			manager->RemoveComponent(id, cid);
		}

		// get component
		template <typename T>
		T& GetComponent()
		{
			return manager->GetComponent<T>(id);
		}
		template <typename T>
		const T& GetComponent() const
		{
			return manager->GetComponent<T>(id);
		}

		// has component
		template <typename T>
		bool HasComponent() const
		{
			return manager->HasComponent<T>(id);
		}

		// has component with component id
		bool HasComponent(ComponentID cid) const
		{
			return manager->HasComponent(id, cid);
		}
	};

	void Manager::RemoveComponent(EntityID entity, ComponentID cid)
	{
		if (!HasComponent(entity, cid))
		{
#ifdef LIC_DEBUG
			std::cout << "No Component " << cid << " found in Entity " << entity << "." << std::endl;
#endif
			return;
		}

		for (auto& cptr : m_components.at(cid))
		{
			if (cptr != nullptr && cptr->entity == entity)
			{
				m_empty_component.at(cid).push_back(&cptr - &m_components.at(cid)[0]);
				m_checklist.at(cptr->entity).set(cid, false);
				cptr.reset();
				break;
			}
		}

#ifdef LIC_DEBUG
		std::cout << "Component " << cid << " removed from Entity " << entity << "." << std::endl;
#endif
	}


	/*-----------CONTAINERS------------*/


	template <typename T, typename ...Ts>
	class Container
	{
	private:

		friend class View<T, Ts...>;
		friend class View<Ts...>;
		friend class Manager;

		size_t m_index;
		std::vector<T*> m_vec;

	public:

		Container() = default;
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
		Container<T, Ts...> begin() const
		{
			return Container<T, Ts...>(m_vec, 0);
		}
		Container<T, Ts...> end() const
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

		TupleList() = default;
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
		TupleList<Ts...> begin() const
		{
			return TupleList<Ts...>(m_vec, 0);
		}
		TupleList<Ts...> end() const
		{
			return TupleList<Ts...>(m_vec, m_vec.size());
		}
	};


	/*----------VIEW----------*/


	template <typename ...Ts>
	class View
	{
	private:

		friend class Manager;

		// entities and component T
		Container<Entity, Ts...> m_entities;
		Container<Ts...> m_components;

		// filter out non-component U from view
		template <typename U, typename S, typename ...Ss>
		void _ProcessMultiFilterOut(View<S, Ss...>& view) const
		{
			std::erase_if(view.m_components.m_vec,
						  [](S*& c) -> bool
						  { return c->GetEntity().HasComponent<U>(); }
			);
		}

	public:

		View() = default;

		// filter out component
		template <typename ...Us>
		View<Ts...> FilterOut()
		{
			int process[] = { 0, (_ProcessMultiFilterOut<Us, Ts...>(*this), 0)... };
			(void)process;

			m_entities.m_vec.clear();
			for (auto& c : m_components)
				m_entities.m_vec.push_back(&c.GetEntity());

			return *this;
		}
		template <typename ...Us>
		View<Ts...> FilterOut() const
		{
			View<Ts...> view(*this);

			int process[] = { 0, (_ProcessMultiFilterOut<Us, Ts...>(view), 0)... };
			(void)process;

			view.m_entities.m_vec.clear();
			for (auto& c : view.m_components)
				view.m_entities.m_vec.push_back(&c.GetEntity());

			return view;
		}

		// begin and end methods for entities iterator
		Container<Entity, Ts...> begin() const
		{
			return m_entities.begin();
		}
		Container<Entity, Ts...> end() const
		{
			return m_entities.end();
		}

		// get component T
		Container<Ts...> Component() const
		{
			return m_components;
		}

		// get entity
		Container<Entity, Ts...> Entity() const
		{
			return m_entities;
		}

		// get each component T, Ts...
		TupleList<Ts...> Each() const
		{
			return TupleList<Ts...>(m_entities.m_vec);
		}
	};
}